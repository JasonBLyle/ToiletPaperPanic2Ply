/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Assignment 3
    October 23 2020
*/

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "Player.h"
#include "PushableObj.h"
#include "HealthObj.h"
#include "Text.h"
#include "MenuOptions.h"
#include "Background.h"
#include "Enemy.h"
#include "ToiletPaper.h"

class GameEngine{
    private:
        static GameEngine *instance;
        GameEngine();

        SDL_Renderer *renderer;
        SDL_Window *window;
        int screenW; //width of the game window
        int screenH; //height of the game window
        int floorY; //the y coord of the floor

        Background GameBG;//Background change
        Background TitleBG;
        SDL_Rect camera;//Background change

        bool runningState;
        bool paused;
        bool showTitleScreen;
        bool gameOver;
        bool win;
        void InitText(SDL_Renderer *renderer, int screenW, int screenH);
        void InitMenus(SDL_Renderer *renderer, int screenW, int screenH);
        void InitObjects();

        //The music that will be played
        Mix_Music *menuMusic;
        Mix_Music *gameMusic;
        Mix_Music *gameOverMusic;



    public:
        static GameEngine *GetInstance();

        int GetCameraX();//Background change
        int GetCameraY();//Background change
        int GetCameraWidth();//Background change
        int GetCameraHeight();//Background change
        int getBgWidth();//Background change
        int getBgHeight();//Background change
        void setCamera(int,int,int,int);//Background change
        void setCameraX(int);//Background change
        void setCameraY(int);//Background change
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
