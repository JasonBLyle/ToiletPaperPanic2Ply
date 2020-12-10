/*
    Nikita Tran
    CPSC 4160 2D Game Engine Construction
    Fall 2020
*/

#include "GameEngine.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>

//#define DEBUG_SHOWCOLLIDERS
#define DEBUG_BYPASSTITLESCREEN

/* ---------- GAME OBJECTS  ---------- */
auto cart = std::make_shared<PushableObj>();
auto cart2 = std::make_shared<PushableObj>();
auto player = std::make_shared<Player>();
auto enemy = std::make_shared<Enemy>();
auto enemy2 = std::make_shared<Enemy>();
auto enemy3 = std::make_shared<Enemy>();
auto sanitizer = std::make_shared<HealthObj>();
auto tp = std::make_shared<ToiletPaper>();
auto checkout = std::make_shared<GameObject>();

std::vector<std::shared_ptr<GameObject>> objs;
auto pauseMenuOptions = std::make_shared<MenuOptions>();
auto titleMenuOptions = std::make_shared<MenuOptions>();
auto gameOverMenuOptions = std::make_shared<MenuOptions>();

/* ---------- TEXT  ---------- */
auto healthLabel = std::make_shared<Text>();
auto healthValue = std::make_shared<Text>();

auto unpause_text = std::make_shared<Text>();
auto exitToTitle_text = std::make_shared<Text>();


auto startGame_text = std::make_shared<Text>();
auto howToPlayLabel_text = std::make_shared<Text>();
auto quitGame_text = std::make_shared<Text>();

auto tryagain_text = std::make_shared<Text>();
auto exitToTitle2_text = std::make_shared<Text>();

auto selection_controls = std::make_shared<Text>();

/* ---------- FOR THE MENUS  ---------- */
SDL_Rect fullScreenRect;
auto pause_title_sprite = std::make_shared<GameObject>();
auto main_title_sprite = std::make_shared<GameObject>();
auto gameover_sprite = std::make_shared<GameObject>();
/* ------------------------------------ */

GameEngine::GameEngine(){
    screenW = 0;
    screenH = 0;
    floorY = 0;
    window = NULL;
    renderer = NULL;
    srand(time(NULL));

    /* ---------------- INITIALIZE GAME STATE ------------------- */
    runningState = true;
    paused = false;
    gameOver = false;
    win = false;

    #ifdef DEBUG_BYPASSTITLESCREEN
        showTitleScreen = false;
    #endif
    #ifndef DEBUG_BYPASSTITLESCREEN

        showTitleScreen = true;
    #endif
};

GameEngine *GameEngine::instance = NULL;

GameEngine* GameEngine::GetInstance(){
    if(instance == NULL) instance = new GameEngine();
    return instance;
}

int GameEngine::GetScreenWidth(){return screenW;}
int GameEngine::GetScreenHeight(){return screenH;}
int GameEngine::GetFloorY(){return floorY;}
SDL_Renderer* GameEngine::GetRenderer(){return renderer;}
SDL_Window* GameEngine::GetWindow(){return window;}
bool GameEngine::GetRunningState(){return runningState;}

void GameEngine::SetRunningState(bool newState){runningState = newState;}
int GameEngine::GetCameraX(){return camera.x;}//Background change
int GameEngine::GetCameraY(){return camera.y;}//Background change
int GameEngine::getBgWidth(){return GameBG.getMapWidth();}//Background change
int GameEngine::getBgHeight(){return GameBG.getMapHeight();}//Background change
int GameEngine::GetCameraWidth(){ return camera.w + camera.x;}//Background change
int GameEngine::GetCameraHeight(){ return camera.h + camera.y;}//Background change
void GameEngine::setCamera(int x,int y,int w,int h){//Background change
    camera.x=x;
    camera.y=y;
    camera.w=w;
    camera.h=h;
}
void GameEngine::setCameraX(int x){camera.x=x;}//Background change
void GameEngine::setCameraY(int y){camera.y=y;}//Background change


/*
    Initialize GameObjects here.
    Also used to reset GameObjects to their initial positions.
*/
void GameEngine::InitObjects(){
    setCameraX(0);
    setCameraY(0);

    floorY = 25;

    /* ----------------------------------- */
    int spriteFrameWidth = 220;
    int spriteFrameHeight = 370;
    double scale = 0.5;
    player->Init(renderer, "img/player.png",&camera);
    player->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight); //set the area of the texture to be rendered
    player->GetSprite()->SetScreenRect(screenW/2, screenH - spriteFrameHeight * scale - floorY, spriteFrameWidth * scale, spriteFrameHeight * scale); //set the area of the screen that renders src_rect
    player->SetBoxCollider(player->GetSprite()->GetScreenRect());
    player->SetHealth(96.0);
    player->SetIdle();
    player->GotTP(false);

    spriteFrameWidth = 212;
    spriteFrameHeight = 351;
    enemy->Init(renderer, "img/enemy.png", &camera);
    enemy->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight); //set the area of the texture to be rendered
    enemy->GetSprite()->SetScreenRect(screenW/2 + 300, screenH - spriteFrameHeight * scale - floorY, spriteFrameWidth * scale, spriteFrameHeight * scale); //set the area of the screen that renders src_rect
    enemy->SetBoxCollider(enemy->GetSprite()->GetScreenRect());
    enemy->SetIdle();


    enemy2->Init(renderer, "img/enemy.png", &camera);
    enemy2->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight); //set the area of the texture to be rendered
    enemy2->GetSprite()->SetScreenRect(screenW/2 + 400, screenH - spriteFrameHeight * scale - floorY, spriteFrameWidth * scale, spriteFrameHeight * scale); //set the area of the screen that renders src_rect
    enemy2->SetBoxCollider(enemy2->GetSprite()->GetScreenRect());
    enemy2->SetIdle();

    enemy3->Init(renderer, "img/enemy.png", &camera);
    enemy3->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight); //set the area of the texture to be rendered
    enemy3->GetSprite()->SetScreenRect(screenW/2 + 500, screenH - spriteFrameHeight * scale - floorY, spriteFrameWidth * scale, spriteFrameHeight * scale); //set the area of the screen that renders src_rect
    enemy3->SetBoxCollider(enemy3->GetSprite()->GetScreenRect());
    enemy3->SetIdle();

    /* ----------------------------------- */

    spriteFrameWidth = 263;
    spriteFrameHeight = 250;
    scale = 0.5;
    cart->Init(renderer,"img/shoppingcart.png",&camera);//background change
    cart->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight);
    cart->GetSprite()->SetScreenRect(screenW/2 + 600, screenH - spriteFrameHeight * scale - floorY, spriteFrameWidth * scale, spriteFrameHeight * scale);
    cart->SetBoxCollider(cart->GetSprite()->GetScreenRect());
    cart->SetObjState(PushableObjState::IDLE);
    cart->SetVelocity(0);

    cart2->Init(renderer,"img/shoppingcart.png",&camera);//background change
    cart2->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight);
    cart2->GetSprite()->SetScreenRect(screenW/2 + 200, screenH - (spriteFrameHeight * scale) - floorY, spriteFrameWidth * scale, spriteFrameHeight * scale);
    cart2->SetBoxCollider(cart2->GetSprite()->GetScreenRect());
    cart2->SetObjState(PushableObjState::IDLE);
    cart2->SetVelocity(0);

    spriteFrameWidth = 239;
    spriteFrameHeight = 500;
    scale = 0.15;
    sanitizer->Init(renderer,"img/sanitizer.png",&camera);//background change
    sanitizer->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight);
    sanitizer->GetSprite()->SetScreenRect(screenW/2 + -50, screenH - spriteFrameHeight * scale - floorY, spriteFrameWidth * scale, spriteFrameHeight * scale);
    sanitizer->SetBoxCollider(sanitizer->GetSprite()->GetScreenRect());
    sanitizer->SetHealthType(HealthType::SANITIZER);
    sanitizer->SetObjState(HealthObjState::NOT_COLLECTED);
    sanitizer->ResetSprite();

    spriteFrameWidth = 240;
    spriteFrameHeight = 249;
    scale = 0.2;
    tp->Init(renderer,"img/tp.png",&camera);//background change
    tp->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight);
    tp->GetSprite()->SetScreenRect(screenW/2 + 300, screenH - spriteFrameHeight * scale - floorY, spriteFrameWidth * scale, spriteFrameHeight * scale);
    tp->SetBoxCollider(tp->GetSprite()->GetScreenRect());
    tp->SetObjState(TPObjState::NOT_COLLECTED);
    tp->ResetSprite();

    spriteFrameWidth = 271;
    spriteFrameHeight = 372;
    scale = 0.85;
    checkout->Init(renderer,"img/checkout.png",&camera);//background change
    checkout->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight);
    checkout->GetSprite()->SetScreenRect(-50, screenH - spriteFrameHeight * scale - floorY, spriteFrameWidth * scale, spriteFrameHeight * scale);
    checkout->SetBoxCollider(checkout->GetSprite()->GetScreenRect());
    checkout->SetType(ObjType::Checkout);
}


/*
    Initializes the game window, renderer, and game objects

    w = desired width of the game screen
    h = desired height of the game screen
*/
void GameEngine::Init(const int w, const int h){
    /* ---------------- INITIALIZE WINDOW ------------------- */
    screenW = w;
    screenH = h;
    setCamera(0,0,screenW,screenH);//Background change

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
    if (TTF_Init() != 0) std::cout << "Error initializing TTF: " << TTF_GetError() << std::endl;
    IMG_Init(IMG_INIT_PNG); // Enable gpu_enhanced textures
    window = SDL_CreateWindow("Toilet Paper Panic: 2-ply", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenW, screenH, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    /* ---------------- BACKGROUND ------------------- */
    int spriteFrameWidth = 2000;//Background change
    int spriteFrameHeight = 960;//Background change
    Background temp(renderer, "img/woodenbackground.png",0, 0, spriteFrameWidth, spriteFrameHeight,&camera);//Background change
    GameBG = temp;//Background change

    spriteFrameWidth = 2000;//Background change
    spriteFrameHeight = 960;//Background change
    Background temp2(renderer, "img/woodenbackground.png",0, 0, spriteFrameWidth, spriteFrameHeight,&camera);//Background change
    TitleBG = temp2;//Background change

    //Initialize Game Objects
    InitObjects();
    objs = {player, enemy, enemy2, enemy3, cart, cart2, sanitizer, tp, checkout};
    //objs = {player, tp, checkout};


    /* ---------------- MUSIC ------------------- */
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 32 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }

    menuMusic = Mix_LoadMUS( "sounds/Toilet_Paper_Waltz_Final.wav" );
    gameMusic = Mix_LoadMUS( "sounds/TPP_3rd_Draft_Final.wav" );
    gameOverMusic = Mix_LoadMUS( "sounds/Game_Over.wav" );

    Mix_VolumeMusic(60); //max volume is 128

    /* ---------------- TEXT ------------------- */
    InitMenus(renderer, screenW, screenH);
    InitText(renderer, screenW, screenH);
}







int playingGameOver = 0;
// Listens for input and sets states accordingly
void GameEngine::HandleEvents(){
    SDL_Event my_input;

    /* ---------- KEYBOARD INPUT  ---------- */

    while (SDL_PollEvent(&my_input) > 0){
	//int jumping = 0;
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
                    if(paused && !showTitleScreen){
                        switch(pauseMenuOptions->GetCurrentOption()){//need to update to track
                            case 0: { //Unpause
                                paused = false;
                                break;
                            }
                            case 1: { //go to title screen
                                Mix_PlayMusic( menuMusic, -1 );
                                showTitleScreen = true;
                                break;
                            }
                        }
                    }
                    else if(showTitleScreen){
                        switch(titleMenuOptions->GetCurrentOption()){
                            case 0: { //Start new game
                                showTitleScreen = false;
                                paused = false;
                                InitObjects();
                                Mix_PlayMusic( gameMusic, -1 );
                                //reset all objects to original states/positions
                                break;
                            }
                            case 1: {
                                //show instructions page
                                break;
                            }
                            case 2: {
                                //quit game
                                runningState = false;
                                break;
                            }
                        }
                    }
                    else if(gameOver){
                        switch(gameOverMenuOptions->GetCurrentOption()){
                            case 0: { //Retry?
                                showTitleScreen = false;
                                paused = false;
                                InitObjects();
                                Mix_PlayMusic( gameMusic, -1 );
                                break;
                            }
                            case 1: { //Exit to title
                                //probably need to add a background change at somepoint
                                Mix_PlayMusic( menuMusic, -1 );
                                showTitleScreen = true;
                                break;
                            }
                        }
                    }
                    else{
			if(player->GetPlayerState() == PlayerState::IDLE && player->GetSprite()->GetY() > 0){
			    if(player->GetJumping() < 1) {
				//std::cout << "Set state to jump\n";
				player->SetPlayerState(PlayerState::JUMP);
				//jumping++;
			    }
			} else if(player->GetPlayerState() == PlayerState::JUMP){
			    player->SetPlayerState(PlayerState::FALL);
			}

                    }

                    break;
                }
                case SDLK_w: {
                    if(paused && !showTitleScreen){ pauseMenuOptions->SelectPrevOption();}
                    if (showTitleScreen){titleMenuOptions->SelectPrevOption();}
                    if (gameOver){gameOverMenuOptions->SelectPrevOption();}
                    break;
                }
                case SDLK_s: {
                    if (showTitleScreen){titleMenuOptions->SelectNextOption();}
                    if(paused && !showTitleScreen){pauseMenuOptions->SelectNextOption();}
                    if (gameOver){gameOverMenuOptions->SelectNextOption();}
                    else if(player->GetSprite()->GetY() + player->GetSprite()->GetH() < GetScreenHeight() - floorY){
                        player->SetPlayerState(PlayerState::FALL);
                    }
                    break;
                }
                case SDLK_ESCAPE:{
                    if(!gameOver && !showTitleScreen && !win) paused = true;
                    break;
                }
            }
        }
        else if(my_input.type == SDL_KEYUP){
            if (player->GetPlayerState() == PlayerState::JUMP) {
                if(player->GetJumping() > 0) {
                    player->SetPlayerState(PlayerState::FALL);
                }
            }

            if(player->GetPlayerState() != PlayerState::FALL){
                player->SetPlayerState(PlayerState::IDLE);
		player->SetJumping(0);
            }

        }
    }

    if(!paused && !showTitleScreen){
        /* ---------- COLLISION CHECKING  ---------- */
        bool playerTest = false;
        for (auto obj1 : objs){
            for(auto obj2 : objs){
                if(obj1 != obj2){ //make sure the object isn't being compared with itself
                    if(IsColliding(obj1->GetBoxCollider(), obj2->GetBoxCollider())){
                        //std::cout << "obj1: " << obj1->PrintObjType() << " obj2: " << obj2->PrintObjType() << "   COLLIDING" << std::endl;
                        if(obj1->GetType() == ObjType::Player || obj2->GetType() == ObjType::Player){ playerTest = true; }
                        
                        //if player has the TP and is at the checkout, you win!
                        if(obj1->GetType() == ObjType::Player && obj2->GetType() == ObjType::Checkout && player->GotTP()){ win = true; }
                        
                        obj1->DoCollisionResponse(obj2);
                    }

                    else{
                        //std::cout << "obj1: " << obj1->PrintObjType() << " obj2: " << obj2->PrintObjType() << "   NOT COLLIDING" << std::endl;

                        if(obj1->GetType() == ObjType::Player && obj2->GetType() == ObjType::Pushable){
			    //std::cout << "set to idle1\n";
                            obj2->SetIdle();
                        }
                    }
                }
            }
        }
        //Checking to see if player is colliding. If not, and idle, suggest that it go to falling state (overrided if on the floor)
        if(playerTest == false && player->GetPlayerState() == PlayerState::IDLE && player->GetSprite()->GetY() + player->GetSprite()->GetH() < GetScreenHeight() - floorY){
            player->SetPlayerState(PlayerState::FALL);
        }
    }

    //check if game over
    if(player->GetHealth() <= 0){
        gameOver = true;
        playingGameOver++;
        if(playingGameOver == 1) Mix_PlayMusic(gameOverMusic, -1);
    }
    else {
        gameOver = false;
        playingGameOver = 0;
    }
}





void GameEngine::Update(){
    if( Mix_PlayingMusic() == 0 ){
        Mix_PlayMusic( gameMusic, -1 );
    }

    if(!paused && !showTitleScreen && !gameOver && !win){
        const char *health = std::to_string((int)player->GetHealth()).c_str();
        healthValue->SetText(health);

        for(auto obj : objs){
            switch(obj->GetType()){
                case ObjType::Pushable: {
                    auto pushable = std::dynamic_pointer_cast<PushableObj>(obj);
                    pushable->SetPushForce(player->GetMovementSpeed());
                    pushable->Update();
                    break;
                }
                case ObjType::Enemy: {
                    auto enemy = std::dynamic_pointer_cast<Enemy>(obj);
                    if(enemy->GetAggro()){
                        if(abs(player->GetSprite()->GetX() - enemy->GetSprite()->GetX()) <= 5){
                            enemy->SetIdle();
                        }
                        else if(player->GetSprite()->GetX() < enemy->GetSprite()->GetX() && enemy->GetSprite()->GetX() < GameBG.getScreenx() + GetScreenWidth()){
                            enemy->SetEnemyState(EnemyState::MOVE_LEFT);
                        }
                        else if(player->GetSprite()->GetX() > enemy->GetSprite()->GetX() && enemy->GetSprite()->GetX() + enemy->GetSprite()->GetW() > GameBG.getScreenx()){
                            enemy->SetEnemyState(EnemyState::MOVE_RIGHT);
                        }
                        else {
                            enemy->SetIdle();
                        }
                    }
                    else{
                        if(abs(enemy->GetSprite()->GetX() - enemy->GetDestination()) > 5){
                            if(enemy->GetSprite()->GetX() > enemy->GetDestination()){
                                enemy->SetEnemyState(EnemyState::MOVE_LEFT);
                            }
                            else if(enemy->GetSprite()->GetX() < enemy->GetDestination()){
                                enemy->SetEnemyState(EnemyState::MOVE_RIGHT);
                            }
                        }
                        else{
                            if(rand() % 300 == 0){
                                enemy->SetDestination(rand() % (getBgWidth() - enemy->GetSprite()->GetW()));
                            }
                            enemy->SetIdle();
                        }
                    }
                }
                default: {
                    obj->Update();
                    break;
                }
            }
        }
    }

    if(gameOver){
        healthValue->SetText("0");
    }
}




void GameEngine::Render(){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //set screen's background color to white
    SDL_RenderClear(renderer);

    /* OBJECTS TO RENDER */
    if(showTitleScreen){
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &fullScreenRect);

        setCameraX(0);//background change
        setCameraY(0);//background change
        TitleBG.render();

        main_title_sprite->Render();
        selection_controls->Render();
        titleMenuOptions->Render();
    }
    else{

      GameBG.moveScreenX(player->GetBoxCollider());//Background change
      GameBG.render();//Background change
        for(auto obj : objs){
            switch(obj->GetType()){
                case ObjType::Player:{
                    player->Render(0, NULL, player->GetSprite()->GetFlip());

                    #ifdef DEBUG_SHOWCOLLIDERS
                    player->RenderBoxCollider();
                    #endif
                    break;
                }
                case ObjType::Enemy:{
                    obj->Render(0, NULL, obj->GetSprite()->GetFlip());

                    #ifdef DEBUG_SHOWCOLLIDERS
                    obj->RenderBoxCollider();
                    #endif
                    break;
                }
                default: {
                    obj->Render();

                    #ifdef DEBUG_SHOWCOLLIDERS
                    obj->RenderBoxCollider();
                    #endif
                    break;
                }
            }
        }

        /* TEXT TO RENDER */
        healthLabel->Render();
        healthValue->Render();

        if(paused){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_RenderFillRect(renderer, &fullScreenRect);

            pause_title_sprite->GetSprite()->AddX(GetCameraX());//background change
            pause_title_sprite->Render();
            pause_title_sprite->GetSprite()->AddX(-GetCameraX());//background change

            selection_controls->Render();
            pauseMenuOptions->Render();
        }
        else if(gameOver){
            SDL_SetRenderDrawColor(renderer, 150, 0, 0, 200);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_RenderFillRect(renderer, &fullScreenRect);

            gameover_sprite->GetSprite()->AddX(GetCameraX());//background change
            gameover_sprite->Render();
            gameover_sprite->GetSprite()->AddX(-GetCameraX());//background change

            selection_controls->Render();
            gameOverMenuOptions->Render();
        }
        else if(win){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_RenderFillRect(renderer, &fullScreenRect);
        }
    }

    SDL_RenderPresent(renderer);
}





void GameEngine::Quit(){
    Mix_FreeMusic(menuMusic);
    Mix_FreeMusic(gameMusic);
    Mix_FreeMusic(gameOverMusic);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
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





void GameEngine::InitText(SDL_Renderer *renderer, int screenW, int screenH){
    //initialize each text object
    SDL_Color black = {0,0,0};
    healthLabel->Init(renderer, "fonts/theboldfont.ttf", 24, 10, 10, black);
    healthLabel->SetText("Health: ");
    healthValue->Init(renderer, "fonts/theboldfont.ttf", 24, healthLabel->GetW() + 10, 10, black);
    const char *health = std::to_string((int)player->GetHealth()).c_str();
    healthValue->SetText(health);

    SDL_Color white = {255,255,255};
    //for pause menu
    unpause_text->Init(renderer, "fonts/theboldfont.ttf", 48, 0, 0, white);
    unpause_text->SetText("Resume");
    unpause_text->SetX(screenW/2 - unpause_text->GetW()/2);
    unpause_text->SetY(pause_title_sprite->GetSprite()->GetY() +
                        pause_title_sprite->GetSprite()->GetH() + 80);

    exitToTitle_text->Init(renderer, "fonts/theboldfont.ttf", 48, 0, 0, white);
    exitToTitle_text->SetText("Exit to title");
    exitToTitle_text->SetX(screenW/2 - exitToTitle_text->GetW()/2);
    exitToTitle_text->SetY(unpause_text->GetY() + unpause_text->GetH() + 50);

    //for title screen
    startGame_text->Init(renderer, "fonts/theboldfont.ttf", 36, 0, 0, black);
    startGame_text->SetText("Start New Game");
    startGame_text->SetX(screenW/2 - startGame_text->GetW()/2);
    startGame_text->SetY(main_title_sprite->GetSprite()->GetY() +
                        main_title_sprite->GetSprite()->GetH() + 25);

    howToPlayLabel_text->Init(renderer, "fonts/theboldfont.ttf", 36, 0, 0, black);
    howToPlayLabel_text->SetText("How To Play");
    howToPlayLabel_text->SetX(screenW/2 - howToPlayLabel_text->GetW()/2);
    howToPlayLabel_text->SetY(startGame_text->GetY() + startGame_text->GetH() + 5);

    quitGame_text->Init(renderer, "fonts/theboldfont.ttf", 36, 0, 0, black);
    quitGame_text->SetText("Quit Game");
    quitGame_text->SetX(screenW/2 - quitGame_text->GetW()/2);
    quitGame_text->SetY(howToPlayLabel_text->GetY() + howToPlayLabel_text->GetH() + 5);

    selection_controls->Init(renderer, "fonts/Comfortaa-Regular.ttf", 18, 0, 0, black);
    selection_controls->SetText("Use W and S to select, [SPACE] to confirm");
    selection_controls->SetX(screenW/2 - selection_controls->GetW()/2);
    selection_controls->SetY(screenH - selection_controls->GetH() - 10);


    tryagain_text->Init(renderer, "fonts/theboldfont.ttf", 48, 0, 0, white);
    tryagain_text->SetText("Retry?");
    tryagain_text->SetX(screenW/2 - unpause_text->GetW()/2);
    tryagain_text->SetY(gameover_sprite->GetSprite()->GetY() +
                        gameover_sprite->GetSprite()->GetH() + 60);

    exitToTitle2_text->Init(renderer, "fonts/theboldfont.ttf", 48, 0, 0, white);
    exitToTitle2_text->SetText("Exit to title");
    exitToTitle2_text->SetX(screenW/2 - exitToTitle_text->GetW()/2);
    exitToTitle2_text->SetY(unpause_text->GetY() + unpause_text->GetH() + 50);

    //create pause menu buttons
    std::vector<std::shared_ptr<Text>> temp;
    temp = {unpause_text, exitToTitle_text};
    pauseMenuOptions->Init(renderer, "img/selector.png", 100, 100, 0.4, temp);

    //create start menu buttons
    temp = {startGame_text, howToPlayLabel_text, quitGame_text};
    titleMenuOptions->Init(renderer, "img/selector.png", 100, 100, 0.3, temp);

    //create game over menu buttons
    temp = {tryagain_text, exitToTitle2_text};
    gameOverMenuOptions->Init(renderer, "img/selector.png", 100, 100, 0.4, temp);
}

void GameEngine::InitMenus(SDL_Renderer *renderer, int screenW, int screenH){
    fullScreenRect.x = 0;
    fullScreenRect.y = 0;
    fullScreenRect.w = screenW;
    fullScreenRect.h = screenH;

    int spriteFrameWidth = 737;
    int spriteFrameHeight = 235;
    double scale = 0.5;

    pause_title_sprite->Init(renderer,"img/paused.png",&camera);//background change
    pause_title_sprite->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight);
    pause_title_sprite->GetSprite()->SetScreenRect(screenW/2 - (spriteFrameWidth/2 * scale), 10, spriteFrameWidth * scale, spriteFrameHeight * scale);

    spriteFrameWidth = 726;
    spriteFrameHeight = 695;
    scale = 0.40;
    main_title_sprite->Init(renderer,"img/title.png",&camera);//background change
    main_title_sprite->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight);
    main_title_sprite->GetSprite()->SetScreenRect(screenW/2 - (spriteFrameWidth/2 * scale), 10, spriteFrameWidth * scale, spriteFrameHeight * scale);

    spriteFrameWidth = 736;
    spriteFrameHeight = 397;
    scale = 0.40;

    gameover_sprite->Init(renderer,"img/gameover.png",&camera);//background change

    gameover_sprite->GetSprite()->SetSrcRect(0, 0, spriteFrameWidth, spriteFrameHeight);
    gameover_sprite->GetSprite()->SetScreenRect(screenW/2 - (spriteFrameWidth/2 * scale), 10, spriteFrameWidth * scale, spriteFrameHeight * scale);
}
