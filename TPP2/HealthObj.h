/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Assignment 4
*/

#ifndef HEALTHOBJ_H
#define HEALTHOBJ_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GameObject.h"
#include "ParticleEmitter.h"

enum class HealthObjState { NOT_COLLECTED, COLLECTED };
enum class HealthType { SANITIZER, WIPES };

class HealthObj: public GameObject{
    private:
        std::shared_ptr<ParticleEmitter> particleEmitter;
        HealthObjState objState;
        HealthType healthType;
    public:
        HealthObj();

        HealthObjState GetObjState();
        HealthType GetHealthType();
        std::shared_ptr<ParticleEmitter> GetParticleEmitter(); 
     
        void SetObjState(HealthObjState state);
        void SetHealthType(HealthType type);

        //overrides
        void Update() override;
        void Render() override;
        void DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith) override;
        std::string PrintObjType() override;
};

#endif