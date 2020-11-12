/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Fall 2020
*/

#include "Player.h"
#include "GameEngine.h"

#include <string>
#include <SDL2/SDL_image.h>
#include <iostream>

Player::Player(){
    objType = ObjType::Player;
    playerState = PlayerState::IDLE;
};

GameEngine* game = GameEngine::GetInstance();

PlayerState Player::GetPlayerState(){return playerState;}
int Player::GetMovementSpeed(){return moveSpeed;}

/*
    state = one of the valid states that Player can have. Valid states (defined in header file) are IDLE, MOVE_LEFT, MOVE_RIGHT, JUMP
    (jump state can be set but it doesn't do anything yet in this build)
*/
void Player::SetPlayerState(PlayerState state){playerState = state;}

void Player::SetMovementSpeed(int speed){moveSpeed = speed;}


//Updates player's position and sprite animation frame depending on player's state
void Player::Update(){
    int moveAnimYOffset = this->GetSprite()->GetSrcH();; //used to set sprite's src_rect to the next row of sprites in the sprite sheet
    int moveAnimTotalFrames = 4;
    int moveAnimStartFrame = 0;

    int animSpeed = 200;

    int idleAnimTotalFrames = 4;
    int idleAnimStartFrame = 0;

    //link collider to player pos
    this->SetBoxColliderPos(this->GetSprite()->GetX(), this->GetSprite()->GetY());

    switch(playerState){
        case PlayerState::IDLE: {
            this->GetSprite()->SetSrcY(0);
            this->GetSprite()->UpdateFrame(animSpeed, idleAnimStartFrame, idleAnimTotalFrames);
            break;
        }

        case PlayerState::MOVE_LEFT: {
            this->GetSprite()->SetSrcY(moveAnimYOffset);
            this->GetSprite()->UpdateFrame(animSpeed, moveAnimStartFrame, moveAnimTotalFrames);

            this->MoveX(moveSpeed * -1);
        
            this->GetSprite()->SetFlip(SDL_FLIP_HORIZONTAL);
            break;
        }

        case PlayerState::MOVE_RIGHT: {
            this->GetSprite()->SetSrcY(moveAnimYOffset);
            this->GetSprite()->UpdateFrame(animSpeed, moveAnimStartFrame, moveAnimTotalFrames);

            this->MoveX(moveSpeed);
          
            this->GetSprite()->SetFlip(SDL_FLIP_NONE);
            break;            
        }
        
        case PlayerState::JUMP: {
            //TODO

            break;
        }
    }
    
    //adhere to screen bounds
    if(this->GetSprite()->GetX() < 0){this->GetSprite()->SetX(0);}

    if(this->GetSprite()->GetX() > game->GetScreenWidth() - this->GetSprite()->GetW()){
        this->GetSprite()->SetX(game->GetScreenWidth() - this->GetSprite()->GetW());
    }

    return;
}