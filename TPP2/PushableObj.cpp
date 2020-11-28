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
#include "Player.h"

PushableObj::PushableObj(){
    objType = ObjType::Pushable;
    objState = PushableObjState::IDLE;
    pushForce = 0;
    velocity = 0;
};

GameEngine* game2 = GameEngine::GetInstance();

PushableObjState PushableObj::GetObjState(){return objState;}
double PushableObj::GetPushForce(){return pushForce;}
double PushableObj::GetVelocity(){return velocity;}


/*
    state = one of the valid states that Player can have. Valid states (defined in header file) are IDLE, PUSHED_FROM_LEFT, PUSHED_FROM_RIGHT
*/
void PushableObj::SetObjState(PushableObjState state){objState = state;}

void PushableObj::SetPushForce(int force){pushForce = force;}


//Updates object's position based on object's state
//double velocity = 0;
void PushableObj::Update(){
    //dampen = amount of slow-down when the object goes from moving to idle
    // closer to 0 means more abrupt stop
    double dampen = 0.15; 

    switch(objState){
        case PushableObjState::IDLE: {
            //Dampening effect
            if(velocity < 0){ velocity += dampen; }
            else if(velocity > 0){ velocity -= dampen; }
            else velocity = 0;

            MoveX(velocity);
            //std::cout << velocity << std::endl;
            break;
        }

        case PushableObjState::PUSHED_FROM_LEFT: {
            velocity = pushForce * -1;
            MoveX(velocity);
            break;
        }

         case PushableObjState::PUSHED_FROM_RIGHT: {
            velocity = pushForce;
            MoveX(velocity);
            break;
        }
    }
   
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

void PushableObj::DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith){
    switch(objCollidedWith->GetType()){
        case ObjType::Player: {
            //cast to Player
            auto player = std::dynamic_pointer_cast<Player>(objCollidedWith);
            if(player->GetPlayerState() == PlayerState::MOVE_LEFT){
                SetObjState(PushableObjState::PUSHED_FROM_LEFT);
            }
            else if(player->GetPlayerState() == PlayerState::MOVE_RIGHT){
                SetObjState(PushableObjState::PUSHED_FROM_RIGHT);
            }
            
            break;
        }

        default: {
            break;
        }
    }
}

void PushableObj::SetIdle(){
    SetObjState(PushableObjState::IDLE);
}

std::string PushableObj::PrintObjType(){ return "PushableObj"; }