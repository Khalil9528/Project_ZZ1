#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int j=1;

void drawTornado(SDL_Renderer* renderer, float angle) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int centreX = SCREEN_WIDTH / 2 ;
    int centreY = SCREEN_HEIGHT / 2;
    
    for (int i = 0; i < 10000; i++) {
        float rayon = i * 3;
        float x = centreX + rayon * cos(angle + i * 0.2);
        float y = centreY + rayon * sin(angle + i * 0.2);
        if(i%10 == 0){
            SDL_RenderDrawPoint(renderer, (int)x, (int)y);
        }
        //SDL_Rect r = {x, y, j, j};
        //SDL_RenderFillRect(renderer, &r);
        
    }
}


void draw_sun(SDL_Renderer* renderer, int i) {                            // Dessiner un soleil

    for (float angle = i*M_PI/240; angle < 2 * M_PI+i*M_PI/240; angle += M_PI / 4000) {      
     SDL_SetRenderDrawColor(renderer,
                (cos(angle * 2) + 1) * 255 / 2,          // quantité de Rouge      
                (cos(angle * 5) + 1) * 255 / 2,          //          de vert 
                (cos(angle) + 1) * 255 / 2,              //          de bleu
                255);                                    // opacité = opaque
     SDL_RenderDrawPoint(renderer,                  
                 400 + 50 * cos(angle),                     // coordonnée en x
                 300 + 50 * sin(angle));                    //            en y  
       }

    int cx = 400;  // Centre du soleil en x
    int cy = 300;  // Centre du soleil en y
    int radius = 50;  // Rayon du cercle central
    int ray_length = 10;  // Longueur des rayons

    for (int j = 0; j < 8; ++j) {
        double angle = i*M_PI/240 + j * M_PI / 4;  // Angle en radians
        SDL_SetRenderDrawColor(renderer, 255,255,0
                //(cos(angle * 2) + 1) * 255 / 2,         // quantité de Rouge      
                //(cos(angle * 5) + 1) * 255 / 2,          //          de vert 
                //(cos(angle) + 1) * 255 / 2,              //          de bleu
                ,255); 
        int x_beg = cx + (radius) * cos(angle);  // Calculer la coordonnée x de l'extrémité du rayon
        int y_beg = cy + (radius) * sin(angle);
        int x_end = cx + (radius + ray_length) * cos(angle);  // Calculer la coordonnée x de l'extrémité du rayon
        int y_end = cy + (radius + ray_length) * sin(angle);  // Calculer la coordonnée y de l'extrémité du rayon
        
        SDL_RenderDrawLine(renderer,x_beg ,y_beg, x_end, y_end);  // Tracer le rayon
}
}



int main(void) {
     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("ErrorS: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Tornado Animation", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        SCREEN_WIDTH, SCREEN_HEIGHT, 
        SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("ErrorW: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }



    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }




    bool q = false;
    SDL_Event e;
    float angle = 0.0f;

    while (!q) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                q = true;
            }
        }

        angle += 0.1f;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        for( int i = 0 ; i < 50; ++i)
   {
        draw_sun(renderer,i);
        drawTornado(renderer, angle);
        

   }
        

        SDL_RenderPresent(renderer);
        SDL_Delay(16);  
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
