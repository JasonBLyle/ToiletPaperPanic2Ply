/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Assignment 3
    October 23 2020
*/

#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GameObject.h"

enum class PlayerState { IDLE, MOVE_LEFT, MOVE_RIGHT, JUMP };

class Player: public GameObject{
    private:
        int moveSpeed;
        PlayerState playerState;
    public:
        //Player(SDL_Renderer *ren, const char *file):GameObject(ren, file){};
        Player();
        PlayerState GetPlayerState();
        int GetMovementSpeed();
        void SetMovementSpeed(int speed);
        void SetPlayerState(PlayerState state);
        void Update() override;
};

#endif