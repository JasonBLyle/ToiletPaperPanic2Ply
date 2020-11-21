/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Assignment 4
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
        double velocity;
    public:
        PushableObj();
        PushableObjState GetObjState();
        double GetPushForce();

        void SetPushForce(int force);
        void SetObjState(PushableObjState state);

        //overrides
        void Update() override;
        void DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith) override;
        void SetIdle() override;
        std::string PrintObjType() override;
        std::string PrintState();

};

#endif