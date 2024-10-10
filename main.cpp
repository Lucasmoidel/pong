#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define Win_width 800
#define Win_height 600
#define frame_terget_time (100 / FPS)
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int game_running = false;
int last_frame_time = 0;
int p1score;
int p2score;
int yspeed = 50;
int ballspeed = 3;
float deltatime;
TTF_Font* font = NULL;

struct padle {
    float x, y;
    int width, height;
} p1paddle;
struct p2paddle {
    float x, y;
    int width, height;
} p2paddle;
struct ball {
    float x, y;
    int w, h;
    int xdir, ydir;
} ball;

void display_text();

int initialize_window();

void process_input();

void update();

void render();

void destroy_window();

void setup() {
    font = TTF_OpenFont("comic.ttf", 40);
    p1score = 0;
    p2score = 0;
    p1paddle.x = 10;
    p1paddle.y = Win_height/2;
    p1paddle.width = 10;
    p1paddle.height = 75;
    p2paddle.x = 780;
    p2paddle.y = Win_height/2;
    p2paddle.width = 10;
    p2paddle.height = 75;
    ball.x = 400;
    ball.y = 400;
    ball.w = 20;
    ball.h = 20;
    ball.xdir = 35;
    ball.ydir = 50;
}

int main (int argc, char *argv[]) {
    game_running = initialize_window();
    
    setup();

    while (game_running) {
        process_input();
        update();
        render();
    }

    

    destroy_window();

    return 0;
}

void process_input() {
    deltatime = (SDL_GetTicks() - last_frame_time) / 1000.0f ;
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            game_running = false;
            break;
    
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: 
                    game_running = false;
                    break;
                case SDLK_w:
                    p1paddle.y -= yspeed ;
                    break;
                case SDLK_s:
                    p1paddle.y += yspeed ;
                    break;
                case SDLK_UP:
                    p2paddle.y -= yspeed ;
                    break;
                case SDLK_DOWN:
                    p2paddle.y += yspeed ;
                    break;
                case SDLK_r:
                    p1score = 0;
                    p2score = 0;
                    p1paddle.x = 10;
                    p1paddle.y = Win_height/2;
                    p1paddle.width = 10;
                    p1paddle.height = 75;
                    p2paddle.x = 780;
                    p2paddle.y = Win_height/2;
                    p2paddle.width = 10;
                    p2paddle.height = 75;
                    ball.x = 400;
                    ball.y = 400;
                    ball.w = 20;
                    ball.h = 20;
                    ball.xdir = 35;
                    ball.ydir = 50;
                    break;
                default:
                    break;    

            }   
    }
}

void update() {

    last_frame_time = SDL_GetTicks();

    if (p1paddle.y < 0) {
        p1paddle.y = 0;
    } else if (p1paddle.y > Win_height - p1paddle.height) {
        p1paddle.y = Win_height - p1paddle.height;
    }

    if (p2paddle.y < 0) {
        p2paddle.y = 0;
    } else if (p2paddle.y > Win_height - p2paddle.height) {
        p2paddle.y = Win_height - p2paddle.height;
    }

    if (ball.x <= 0) {
        ball.xdir = ball.xdir * -1;
        p2score += 1;
    }
    if (ball.x >= Win_width - ball.w) {
        ball.xdir = ball.xdir * -1;
        p1score += 1;
    }
    if (ball.y <= 0 || ball.y + ball.h >= Win_height) {
        ball.ydir = ball.ydir * -1;
    }
    
    if ((ball.x <= p1paddle.x + p1paddle.width && ball.y >= p1paddle.y && ball.y <= p1paddle.y + p1paddle.height) || (ball.x  + ball.w >= p2paddle.x && ball.y >= p2paddle.y && ball.y <= p2paddle.y + p2paddle.height)) {
        ball.xdir = ball.xdir * -1;
        //ball.ydir = ball.ydir * -1;
    }
    ball.x += ball.xdir * deltatime * ballspeed;
    ball.y += ball.ydir * deltatime * ballspeed;

}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    display_text();
    SDL_Rect p1paddle_rect = {
    (int)p1paddle.x, 
    (int)p1paddle.y, 
    (int)p1paddle.width, 
    (int)p1paddle.height 
    };
    SDL_Rect p2paddle_rect = {
    (int)p2paddle.x, 
    (int)p2paddle.y, 
    (int)p2paddle.width, 
    (int)p2paddle.height 
    };
    SDL_Rect ballrect = {
    (int)ball.x,
    (int)ball.y,
    (int)ball.w,
    (int)ball.h
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &ballrect);
    SDL_RenderFillRect(renderer, &p1paddle_rect);
    SDL_RenderFillRect(renderer, &p2paddle_rect);
    SDL_RenderPresent(renderer);
}

void display_text() {
    int p1t_width = 0;
    int p1t_height = 0;
    int p2t_width = 0;
    int p2t_height = 0;
    SDL_Color text_color = {255,255,255};
    char p1text[20];
    char p2text[20];
    
    SDL_Texture* p1ftexture = NULL;
    SDL_Texture* p2ftexture = NULL;
    if (font == NULL) {
        printf("font file eror\n");
        game_running = false;
    }
    else {
        sprintf(p1text, "%d", p1score);
        sprintf(p2text, "%d", p2score);
        SDL_Surface* p1text_surface = TTF_RenderText_Solid(font, p1text, text_color);
        SDL_Surface* p2text_surface = TTF_RenderText_Solid(font, p2text, text_color);
        if (p1text_surface == NULL || p2text_surface == NULL) {
            printf("font surface error\n");
            game_running = false;
        } else {
            p1ftexture = SDL_CreateTextureFromSurface(renderer, p1text_surface);
            p2ftexture = SDL_CreateTextureFromSurface(renderer, p2text_surface);
            if (p1ftexture == NULL || p2ftexture == NULL) {
                printf("font texture error\n");
                game_running = false;
            }
            else {
                p1t_width = p1text_surface->w;
                p1t_height = p1text_surface->h;
                p2t_width = p2text_surface->w;
                p2t_height = p2text_surface->h;
                SDL_FreeSurface(p1text_surface);
                SDL_FreeSurface(p2text_surface);
                SDL_Rect p1dst = {150, 40, p1t_width, p1t_height};
                SDL_Rect p2dst = {650, 40, p2t_width, p2t_height};
                SDL_RenderCopy(renderer, p1ftexture, NULL, &p1dst);
                SDL_RenderCopy(renderer, p2ftexture, NULL, &p2dst);
                SDL_DestroyTexture(p1ftexture);
                SDL_DestroyTexture(p2ftexture);
            }
        }
    }
}


void destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int initialize_window(void) {
    TTF_Init();
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0 ) {
        printf("init sdl failed\n");
        return 0;
    }


    window = SDL_CreateWindow("pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Win_width, Win_height, 0);
    

    if (!window) {
        printf ("failed to create window\n");
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        printf ("failed to create renderer\n");
        return 0;
    }

    return 1;
}
