/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Fall 2020
*/

#include "PushableObj.h"
#include "GameEngine.h"

#include <string>
#include <SDL2/SDL_image.h>
#include <iostream>

PushableObj::PushableObj(){
    objType = ObjType::Pushable;
    objState = PushableObjState::IDLE;
    pushForce = 0;
};

GameEngine* game2 = GameEngine::GetInstance();

PushableObjState PushableObj::GetObjState(){return objState;}
double PushableObj::GetPushForce(){return pushForce;}


/*
    state = one of the valid states that Player can have. Valid states (defined in header file) are IDLE, PUSHED_FROM_LEFT, PUSHED_FROM_RIGHT
*/
void PushableObj::SetObjState(PushableObjState state){objState = state;}

void PushableObj::SetPushForce(int force){pushForce = force;}


//Updates object's position based on object's state
double velocity = 0;
void PushableObj::Update(){
    double dampen = 0.15; 

    switch(objState){
        case PushableObjState::IDLE: {
            //Dampening effect
            if(velocity < 0){ velocity += dampen; }
            else if(velocity > 0){ velocity -= dampen; }
            else velocity = 0;

            //std::cout << velocity << std::endl;
            break;
        }

        case PushableObjState::PUSHED_FROM_LEFT: {
            velocity = pushForce * -1;
    
            break;
        }

         case PushableObjState::PUSHED_FROM_RIGHT: {
            velocity = pushForce;
              
            break;
        }
    }

    MoveX(velocity);
   
    //prevent object from crossing left bound (left edge of screen)
    if(this->GetSprite()->GetX() < 0){this->GetSprite()->SetX(0);} 

    //prevent object from crossing right bound (right edge of screen)
    if(this->GetSprite()->GetX() > game2->GetScreenWidth() - this->GetSprite()->GetW()){
        this->GetSprite()->SetX(game2->GetScreenWidth() - this->GetSprite()->GetW());
    }

    //link box collider position to object position
    this->SetBoxColliderPos(this->GetSprite()->GetX(), this->GetSprite()->GetY());
    return;
}  