/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Fall 2020
*/

#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GameObject.h"

enum class PlayerState { IDLE, MOVE_LEFT, MOVE_RIGHT, JUMP, FALL };

class Player: public GameObject{
    private:
        int moveSpeed;
        double ySpeed;
        PlayerState playerState;
    public:
        //Player(SDL_Renderer *ren, const char *file):GameObject(ren, file){};
        Player();
        PlayerState GetPlayerState();
        int GetMovementSpeed();
        int GetYSpeed();
        void SetMovementSpeed(int speed);
        void SetYSpeed(int speed);
        void SetPlayerState(PlayerState state);

        //overrides
        void Update() override;
        void DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith) override;
        void SetIdle() override;
        std::string PrintObjType() override;

};

#endif