#include<iostream>
#include<SDL2/SDL.h>

const int WIDTH = 640;
const int HEIGHT = 480;
const int BALLSIZE = 10;

SDL_Renderer *renderer = NULL;
bool CoinFlip(void) {
    return rand()%2 == 1?  true :false;
}
typedef struct Ball {
    float x;
    float y;
    float xSpeed;
    float ySpeed;
    int size;
} Ball;
Ball ball;
void updateBall(Ball *ball,float elapsed) {
    ball->x += ball->xSpeed *elapsed;
    ball->y += ball->ySpeed *elapsed;

    if(ball->x < BALLSIZE/2) {
        ball->xSpeed = fabs(ball->xSpeed);
    }
    if(ball->x > WIDTH - BALLSIZE/2) {
        ball->xSpeed = -fabs(ball->xSpeed);
    }
    if(ball->y < BALLSIZE/2) {
        ball->ySpeed = fabs(ball->ySpeed);
    }
    if(ball->y > HEIGHT - BALLSIZE/2) {
        ball->ySpeed = -fabs(ball->ySpeed);
    }


}
void RenderBall(const Ball *ball) {
    int size = ball->size;
    int halfSize = size/2;
    SDL_Rect rect = {
    .x = ball->x - halfSize,
    .y = ball->y - halfSize,
    .w = size,
    .h = size, 
    };
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderFillRect(renderer,&rect);
}
Ball MakeBall(int size) {
    const float SPEED = 120;
    Ball ball ={
        .x = WIDTH/2 - size/2,
        .y = HEIGHT/2 - size/2,
        .xSpeed = SPEED *(CoinFlip() ?1 : -1),
        .ySpeed = SPEED * (CoinFlip() ?1 : -1),
        .size = size,

    };
    return ball;
}

void update(float elapsed) {
    SDL_SetRenderDrawColor(renderer,101,50,255,255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    // SDL_Rect *ballRect = new SDL_Rect();
    // ballRect->x = WIDTH/2 - BALLSIZE/2;
    // ballRect->y = HEIGHT/2 - BALLSIZE/2;
    // ballRect->h = BALLSIZE;
    // ballRect->w = BALLSIZE; 
    // SDL_RenderFillRect(renderer,ballRect);
    updateBall(&ball,elapsed);
    RenderBall(&ball);
    SDL_RenderPresent(renderer);


}

int main(int argc,char *args[]){
    
    srand(time(NULL));
    if(SDL_Init(SDL_INIT_VIDEO) <0) {
        std::cerr<<"Error in init \n";
        return 0;
    }
    SDL_Window *window = NULL;
    window = SDL_CreateWindow("pong",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    WIDTH,
    HEIGHT,
    SDL_WINDOW_SHOWN
    );

    if(window == NULL) {
        std::cerr <<"Failed to load window \n";
        return 0;
    }
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);
    
    if(renderer == NULL) {
        std::cerr <<"Failed to create renderer\n";
        return 0;
    }   
    ball = MakeBall(BALLSIZE);

    SDL_Event event;
    
    Uint32 lastTick = SDL_GetTicks();
    
    bool isRunning = true;
    
    while(isRunning) {
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                std::cout <<"Program quited \n";
                isRunning = false;
                break;
            }
        }
        Uint32 currTick = SDL_GetTicks();
        Uint32 diff = currTick - lastTick;
        float elapsed = diff/1000.0f;
        update(elapsed);
        lastTick = currTick;
    
    }
    SDL_DestroyWindow(window);
    SDL_Quit();

}