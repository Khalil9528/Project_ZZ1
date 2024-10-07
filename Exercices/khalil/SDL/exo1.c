#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_WINDOWS 185


void init_sdl() {
 if (SDL_Init(SDL_INIT_VIDEO) != 0) {
 SDL_Log("Error: SDL initialization - %s\n", SDL_GetError());
 exit(EXIT_FAILURE);
 }
}



int main(int argc, char **argv) {
 (void)argc;
 (void)argv;
 SDL_Window *window[MAX_WINDOWS];
 int window_count = 0;
 init_sdl();

 int i;

 // Barre verticale du "K"
 for (i = 0; i < 60; ++i) {
 window[window_count] = SDL_CreateWindow("K", 
 100, 100 + (i * 5), 
 100+i, 100, 
 SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
 if (window[window_count] == NULL) {
 SDL_Log("Error: SDL window creation - %s\n", SDL_GetError());
 SDL_Quit();
 exit(EXIT_FAILURE);
 }
 window_count ++ ;
 SDL_Delay(5); 
 }
 

 // Diagonale supérieure du "K"
 for (i = 0; i < 100; ++i) {
 window[window_count] = SDL_CreateWindow(
 "K", 
 250 + (i * 3), 400 - (i * 3), 
 50, 50,
 SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
 if (window == NULL) {
 SDL_Log("Error: SDL window creation - %s\n", SDL_GetError());
 SDL_Quit();
 exit(EXIT_FAILURE);
 }
 window_count ++ ;
 SDL_Delay(5); 
 }

 // Diagonale inférieure du "K"
 for (i = 0; i < 25; ++i) {
 window[window_count] = SDL_CreateWindow("K", 
 250 + (i * 3), 400 + (i * 3), 
 50, 50, 
 SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
 if (window == NULL) {
 SDL_Log("Error: SDL window creation - %s\n", SDL_GetError());
 SDL_Quit();
 exit(EXIT_FAILURE);
 }
 window_count ++ ;
 SDL_Delay(5); 
 }

 int x,y;

 for(int j=0; j<window_count; ++j){
 SDL_GetWindowPosition(window[j], &x, &y);
 SDL_SetWindowPosition(window[j], x+500, y );
 }

 SDL_Delay(10000); 
 for (i = 0; i < window_count; ++i) {
 SDL_DestroyWindow(window[i]);
 }
 SDL_Quit();
 return 0;
}