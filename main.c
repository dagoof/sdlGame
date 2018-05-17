#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <time.h>

typedef int bool;
enum{false, true};

typedef struct{
    int x, y;
    short life;
    char* dude;
}Man; 

typedef struct{
    int x, y;
}Star;

typedef struct{
    Man man;
    Star stars[100];
    
    SDL_Texture* star;
    SDL_Renderer* renderer;
    
}GameStateObj;

void loadGame(GameStateObj* game){
    SDL_Surface* starSurface = NULL;
    
    starSurface = IMG_Load("star.png");
    if(starSurface == NULL){
        printf("Cannot find star.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    
    game->star = SDL_CreateTextureFromSurface(game->renderer, starSurface);
    SDL_FreeSurface(starSurface);

    game->man.x = 320-40;
    game->man.y = 240-40;
    
    for(int i = 0; i < 100; i++){
        game->stars[i].x = random()%640;
        game->stars[i].y = random()%480;
    }
}

void drawingRender(SDL_Renderer* renderer, GameStateObj* game){
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    SDL_Rect rect = { game->man.x, game->man.y, 80, 80 };
    SDL_RenderFillRect(renderer, &rect);
    
    
    for(int i = 0; i < 100; i++){
        SDL_Rect starRect = { game->stars[i].x, game->stars[i].y, 64, 64 };
        SDL_RenderCopy(renderer, game->star, NULL, &starRect);
    }
    
    SDL_RenderPresent(renderer);
}

void stateOfKey(GameStateObj* gameState){
    const Uint8* state = SDL_GetKeyboardState(NULL);
    
    if(state[SDL_SCANCODE_LEFT]){
        gameState->man.x -= 10;
    }if(state[SDL_SCANCODE_RIGHT]){
        gameState->man.x += 10;
    }if(state[SDL_SCANCODE_UP]){
        gameState->man.y -= 10;
    }if(state[SDL_SCANCODE_DOWN]){
        gameState->man.y += 10;
    }
}

bool keydownEvent(SDL_Event event, bool done){
    switch(event.key.keysym.sym){
        case SDLK_ESCAPE:
            done = true;
        break;
    }
    return done;
}

bool testEvent(SDL_Event event, SDL_Window* window, GameStateObj* gameState, bool done){
    switch(event.type){
        case SDL_WINDOWEVENT_CLOSE:
            if(window){
                SDL_DestroyWindow(window);
                window = NULL;
                done = true;
            }
        break;
        case SDL_KEYDOWN:
            done = keydownEvent(event, done);
        break;
        case SDL_QUIT:
            done = true;
        break;
    }

    stateOfKey(gameState);

    return done;
}

bool pollEvent(SDL_Window* window, GameStateObj* gameState, bool done){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
       done = testEvent(event, window, gameState, done);
    }
    return done;
}

SDL_Window* makeWindow(SDL_Window* window){
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Game Window",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,640,480,0);
    return window;
}

SDL_Renderer* makeRenderer(SDL_Window* window, SDL_Renderer* renderer){
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    return renderer;
}

void destroyGame(SDL_Window* window, SDL_Renderer* renderer){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void gameLoop(){
    srandom((int)time(NULL));
    bool done = false;
    GameStateObj gameState;
    SDL_Window* window = makeWindow(window);
    SDL_Renderer* renderer = makeRenderer(window, renderer);

    gameState.renderer = renderer;
    
    loadGame(&gameState);
    
    while(done == false){
        done = pollEvent(window, &gameState, done);
        drawingRender(renderer, &gameState);
        SDL_Delay(10);
    }
    
    destroyGame(window, renderer);
}

int main(int argc, char** argv){
    gameLoop();
return 0;
}    


