/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Assignment 3
    October 23 2020
*/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <memory>
#include "Sprite.h"

enum class ObjType { Player, Pushable, Particle };

class GameObject{
    private:
        SDL_Renderer *renderer;
        Sprite *sprite;
        SDL_Rect boxCollider;
        int alpha;

    protected:
        ObjType objType; 
        //i don't want to make a public setter to set type in child class since it shouldn't change after being set

    public:
        GameObject();
        virtual ~GameObject();

        void Init(SDL_Renderer *ren, const char *file);
        
        void Render(double angle, SDL_Point *center, SDL_RendererFlip flip);
        void RenderBoxCollider();

        Sprite* GetSprite();
        ObjType GetType();
        SDL_Rect GetBoxCollider();
        SDL_Renderer * GetRenderer(){ return renderer; };
        int GetAlpha();

        void SetBoxCollider(int x, int y, int w, int h);
        void SetBoxColliderPos(int x, int y);
        void SetBoxCollider(SDL_Rect *r);
        void SetAlpha(int a);
        
        void ChangeAlpha(int var);

        void MoveX(int i);
        void MoveY(int i);

        //to be overridden by child classes
        virtual void Update();
        virtual void Render();
        virtual void DoCollisionResponse(std::shared_ptr<GameObject> objCollidedWith);
        virtual void SetIdle();
        virtual std::string PrintObjType();
};

#endif