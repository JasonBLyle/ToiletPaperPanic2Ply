/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Assignment 3
    October 23 2020
*/

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL2/SDL.h>
#include "Player.h"
#include "PushableObj.h"
#include "HealthObj.h"
#include "Text.h"
#include "MenuOptions.h"

class GameEngine{
    private:
        static GameEngine *instance;
        GameEngine();

        SDL_Renderer *renderer;
        SDL_Window *window;
        int screenW; //width of the game window
        int screenH; //height of the game window
        int floorY; //the y coord of the floor
        bool runningState;
        bool paused;
        bool showTitleScreen;
        bool gameOver;
        void InitText(SDL_Renderer *renderer, int screenW, int screenH);
        void InitMenus(SDL_Renderer *renderer, int screenW, int screenH);

    public:
        static GameEngine *GetInstance();
        
        int GetScreenWidth();
        int GetScreenHeight();
        int GetFloorY();
        SDL_Renderer* GetRenderer();
        SDL_Window* GetWindow();
        bool GetRunningState();

        void SetRunningState(bool state);

        void Init(const int w, const int h);
        void HandleEvents();
        void Update();
        void Render();
        void Quit();

        bool IsColliding(SDL_Rect a, SDL_Rect b);
};

#endif