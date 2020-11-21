/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Fall 2020
*/

#include "GameEngine.h"
#include "ParticleEmitter.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>

#define DEBUG_SHOWCOLLIDERS

/* GAME OBJECTS */
//Player* player = NULL;
auto cart = std::make_shared<PushableObj>();
auto cart2 = std::make_shared<PushableObj>();
auto player = std::make_shared<Player>();
auto sanitizer = std::make_shared<GameObject>();
//GameObject *sanitizer = NULL;

/* PARTICLE EMITTER */
auto pe = std::make_unique<ParticleEmitter>();

/* PUSHABLE OBJECTS CONTAINER */
std::vector<std::shared_ptr<GameObject>> objs;

GameEngine::GameEngine(){
    screenW = 0;
    screenH = 0;
    window = NULL;
    renderer = NULL;
    runningState = true;
};

GameEngine *GameEngine::instance = NULL;

GameEngine* GameEngine::GetInstance(){
    if(instance == NULL) instance = new GameEngine();
    return instance;
}

int GameEngine::GetScreenWidth(){return screenW;}
int GameEngine::GetScreenHeight(){return screenH;}
SDL_Renderer* GameEngine::GetRenderer(){return renderer;}
SDL_Window* GameEngine::GetWindow(){return window;}
bool GameEngine::GetRunningState(){return runningState;}

void GameEngine::SetRunningState(bool newState){runningState = newState;}

/*
    Initializes the game window, renderer, and game objects

    w = desired width of the game screen
    h = desired height of the game screen
*/
void GameEngine::Init(const int w, const int h){
    screenW = w;
    screenH = h;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
    }
    
    // Enable gpu_enhanced textures
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("Toilet Paper Panic: 2-ply", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, 
                                screenW,
                                screenH, 
                                0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    /* --- INITIALIZE GAME OBJECTS --- */
    //player = new Player();
    //sanitizer = new GameObject();

    int spriteFrameWidth = 220;
    int spriteFrameHeight = 370;
    double scale = 0.5; //used to scale rendered sprite image if too big/small
    player->Init(renderer, "img/player.png");
    player->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight); //set the area of the surface to be rendered 
    player->GetSprite()->SetScreenRect(screenW/2, 0, spriteFrameWidth * scale, spriteFrameHeight * scale); //set the area of the screen that renders src_rect
    player->GetSprite()->SetY(screenH - player->GetSprite()->GetH() - 25); 
    player->SetBoxCollider(player->GetSprite()->GetScreenRect());

    spriteFrameWidth = 263;
    spriteFrameHeight = 250;
    scale = 0.5;
    cart->Init(renderer,"img/shoppingcart.png");
    cart->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight);
    cart->GetSprite()->SetScreenRect(screenW/2 + 10, 0, spriteFrameWidth * scale, spriteFrameHeight * scale);
    cart->GetSprite()->SetY(screenH - cart->GetSprite()->GetH() - 25);
    cart->SetBoxCollider(cart->GetSprite()->GetScreenRect());

    
    cart2->Init(renderer,"img/shoppingcart.png");
    cart2->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight);
    cart2->GetSprite()->SetScreenRect(screenW/2 - 300, 0, spriteFrameWidth * scale, spriteFrameHeight * scale);
    cart2->GetSprite()->SetY(screenH - cart2->GetSprite()->GetH() - 25);
    cart2->SetBoxCollider(cart2->GetSprite()->GetScreenRect());

    spriteFrameWidth = 239;
    spriteFrameHeight = 500;
    scale = 0.15;
    sanitizer->Init(renderer,"img/sanitizer.png");
    sanitizer->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight);
    sanitizer->GetSprite()->SetScreenRect(screenW/2 + -50, 0, spriteFrameWidth * scale, spriteFrameHeight * scale);
    sanitizer->GetSprite()->SetY(screenH - sanitizer->GetSprite()->GetH() - 25);
    sanitizer->SetBoxCollider(sanitizer->GetSprite()->GetScreenRect());

    objs.push_back(player);
    objs.push_back(cart);
    objs.push_back(cart2);


    scale = 0.1;
    spriteFrameWidth = 247*scale;
    spriteFrameHeight = 247*scale;

    /*
        expression in startX param is the coordinate where the texture will align right on the horizontal center of sanitizer sprite
        expression in startY param is the coordinate where the particle's bottom edge touches the sanitizer sprite's bottom edge
    */
    pe->Init(renderer, "img/healthsparkle.png", sanitizer->GetSprite()->GetX() + sanitizer->GetSprite()->GetW()/2 - spriteFrameWidth/2, 
                                                sanitizer->GetSprite()->GetY() + sanitizer->GetSprite()->GetH() - spriteFrameHeight,
                                                sanitizer->GetSprite()->GetW()/2, sanitizer->GetSprite()->GetH() - spriteFrameHeight,
                                                spriteFrameWidth, spriteFrameHeight, ParticleType::RISE_FADEOUT);

}

bool healthCollected = false;

// Listens for input and sets states accordingly
void GameEngine::HandleEvents(){
    SDL_Event my_input;
    runningState = true;

    //Check for keyboard input
    while (SDL_PollEvent(&my_input) > 0){
        if(my_input.type == SDL_QUIT) runningState = false; //ends the game
        if(my_input.type == SDL_KEYDOWN){
            switch (my_input.key.keysym.sym){
                case SDLK_a: {
                    player->SetPlayerState(PlayerState::MOVE_LEFT);          
                    break;
                }
                case SDLK_d: {
                    player->SetPlayerState(PlayerState::MOVE_RIGHT);  
    
                    break;
                }
                case SDLK_SPACE: {
                    player->SetPlayerState(PlayerState::JUMP); 
                    break;
                }
            }
        }
        else if(my_input.type == SDL_KEYUP){
            player->SetPlayerState(PlayerState::IDLE);
        }
    }
    
    
    //CHECK FOR COLLISIONS
    for (auto obj1 : objs){
        for(auto obj2 : objs){
            if(obj1 != obj2){ //make sure the object isn't being compared with itself
                
                if(obj1->GetType() == ObjType::Pushable){
                    auto p = std::dynamic_pointer_cast<PushableObj>(obj1);
                    std::cout << "obj1: " << p->PrintState() << std::endl;
                }
                if(obj2->GetType() == ObjType::Pushable){
                    auto p = std::dynamic_pointer_cast<PushableObj>(obj2);
                    std::cout << "obj2: " << p->PrintState() << std::endl;
                }
                

                if(IsColliding(obj1->GetBoxCollider(), obj2->GetBoxCollider())){ 
                    //std::cout << "obj1: " << obj1->PrintObjType() << " obj2: " << obj2->PrintObjType() << "   COLLIDING" << std::endl;

                    obj1->DoCollisionResponse(obj2);
                    obj2->DoCollisionResponse(obj1);
                }

                else{
                    //std::cout << "obj1: " << obj1->PrintObjType() << " obj2: " << obj2->PrintObjType() << "   NOT COLLIDING" << std::endl;
                    
                    if(obj1->GetType() == ObjType::Player && obj2->GetType() == ObjType::Pushable){
                        obj2->SetIdle();
                    }
                }
            }
        }
    }

    if(IsColliding(player->GetBoxCollider(), sanitizer->GetBoxCollider())){ 
        healthCollected = true;
    }
}

void GameEngine::Update(){
    for(auto obj : objs){
        switch(obj->GetType()){
            case ObjType::Player:{
                player->SetMovementSpeed(5);
                player->Update();
                break;
            }
            case ObjType::Pushable: {
                auto pushable = std::dynamic_pointer_cast<PushableObj>(obj);
                pushable->SetPushForce(player->GetMovementSpeed());
                pushable->Update();
                break;
            }

            default: {
                break;
            }
        }
    }
    
    if(healthCollected) pe->Update();
}



void GameEngine::Render(){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //set screen's background color to white
    SDL_RenderClear(renderer);

    /* OBJECTS TO RENDER */
    for(auto obj : objs){
        switch(obj->GetType()){
            case ObjType::Player:{
                player->Render(0, NULL, player->GetSprite()->GetFlip());
                #ifdef DEBUG_SHOWCOLLIDERS 
                player->RenderBoxCollider();
                #endif
                break;
            }
            case ObjType::Pushable: {
                obj->Render();
                #ifdef DEBUG_SHOWCOLLIDERS 
                obj->RenderBoxCollider(); 
                #endif
                break;
            }
            default: {
                break;
            }
        }
    }

    if(healthCollected){
        //fade out
        sanitizer->ChangeAlpha(-1 * (255/pe->GetDuration() + 0.5)); //0.5 is added so that the truncation of 255/pe->GetDuration is rounded up
        SDL_SetTextureAlphaMod(sanitizer->GetSprite()->GetTexture(),sanitizer->GetAlpha());
    }
    sanitizer->Render();

    #ifdef DEBUG_SHOWCOLLIDERS 
    sanitizer->RenderBoxCollider();
    #endif
   
    if(healthCollected) pe->Render();
       
    SDL_RenderPresent(renderer);
}

void GameEngine::Quit(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}

bool GameEngine::IsColliding(SDL_Rect a, SDL_Rect b){
    int a_left, b_left;
    int a_right, b_right;
    int a_top, b_top;
    int a_bottom, b_bottom;

    a_left = a.x;
    a_right = a.x + a.w;
    a_top = a.y;
    a_bottom = a.y + a.h;

    b_left = b.x;
    b_right = b.x + b.w;
    b_top = b.y;
    b_bottom = b.y + b.h;

    if(a_right <= b_left ||
       a_left >= b_right ||
       a_bottom <= b_top ||
       a_top >= b_bottom) 
       return false; 

    return true;
}