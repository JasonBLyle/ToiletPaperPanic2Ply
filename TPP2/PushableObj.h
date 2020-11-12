/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Assignment 4
    November 9 2020
*/

#ifndef PUSHABLEPOBJ_H
#define PUSHABLEPOBJ_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GameObject.h"
#include "Player.h"

enum class PushableObjState { IDLE, PUSHED_FROM_LEFT, PUSHED_FROM_RIGHT };

class PushableObj: public GameObject{
    private:
        int pushForce;
        PushableObjState objState;
    public:
        //Player(SDL_Renderer *ren, const char *file):GameObject(ren, file){};
        PushableObj();
        PushableObjState GetObjState();
        double GetPushForce();

        void SetPushForce(int force);
        void SetObjState(PushableObjState state);

        void Update() override;
};

#endif