/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Fall 2020
*/

#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "GameObject.h"

enum class EnemyState { IDLE, MOVE_LEFT, MOVE_RIGHT};

class Enemy: public GameObject{
    private:
        int moveSpeed;
        EnemyState enemyState;
        double damagePerHit;

    public:
        Enemy();
        EnemyState GetEnemyState();
        int GetMovementSpeed();
        double GetDamagePerHit();

        void SetMovementSpeed(int speed);
        void SetEnemyState(EnemyState state);
        void SetDamagePerHit(double d);

        int UpdateFrame(int frame_duration, int start_frame, int totalFrameCount);

        //overrides
        void Update() override;
        void DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith) override;
        void SetIdle() override;
        std::string PrintObjType() override;

};

#endif
