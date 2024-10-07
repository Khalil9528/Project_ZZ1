#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define DELAY 100

SDL_Texture *load_texture_from_image(char *file_image_name, SDL_Window *window, SDL_Renderer *renderer ){
 (void)window;
 SDL_Surface *my_image = NULL;
 SDL_Texture* my_texture = NULL; 

 my_image = IMG_Load(file_image_name);
 if (my_image == NULL){ 
 printf("Echec image : %s\n", IMG_GetError());
 // exit(EXIT_FAILURE);
 }

 my_texture = SDL_CreateTextureFromSurface(renderer, my_image);
 SDL_FreeSurface(my_image); 
 if (my_texture == NULL){
 printf("Echec transfo texture: %s\n", IMG_GetError());
 // exit(EXIT_FAILURE);
 }
 return my_texture;
}

void play_with_texture_1_1(SDL_Texture *my_texture, SDL_Window *window, SDL_Renderer *renderer) {
 SDL_Rect 
 source = {0}, // Rectangle définissant la zone de la texture à récupérer
 window_dimensions = {0},// Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
 destination = {0}; // Rectangle définissant où la zone_source doit être déposée dans le renderer

 SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h); // Récupération des dimensions de la fenêtre
 SDL_QueryTexture(my_texture, NULL, NULL, &source.w, &source.h); // Récupération des dimensions de l'image

 destination = window_dimensions; // On fixe les dimensions de l'affichage à celles de la fenêtre

 /* On veut afficher la texture de façon à ce que l'image occupe la totalité de la fenêtre */
 SDL_RenderCopy(renderer, my_texture, &source, &destination); 
}

void play_with_texture_2(SDL_Texture* my_texture, SDL_Window* window, SDL_Renderer* renderer) {
 SDL_Rect 
 source = {0}, // Rectangle définissant la zone de la texture à récupérer
 window_dimensions = {0}, // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
 destination = {0}; // Rectangle définissant où la zone_source doit être déposée dans le renderer

 SDL_GetWindowSize(
 window, &window_dimensions.w,
 &window_dimensions.h); // Récupération des dimensions de la fenêtre
 SDL_QueryTexture(my_texture, NULL, NULL,
 &source.w, &source.h); // Récupération des dimensions de l'image

 float zoom = 1.5; // Facteur de zoom à appliquer 
 destination.w = source.w * zoom; // La destination est un zoom de la source
 destination.h = source.h * zoom; // La destination est un zoom de la source
 destination.x =
 (window_dimensions.w - destination.w) /2; // La destination est au milieu de la largeur de la fenêtre
 destination.y =
 (window_dimensions.h - destination.h) / 2; // La destination est au milieu de la hauteur de la fenêtre

 SDL_RenderCopy(renderer, my_texture, // Préparation de l'affichage 
 &source,
 &destination); 
}

void play_with_texture_3(SDL_Texture* my_texture, SDL_Window* window, SDL_Renderer* renderer) {
 SDL_Rect 
 source = {0}, // Rectangle définissant la zone de la texture à récupérer
 window_dimensions = {0}, // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
 destination = {0}; // Rectangle définissant où la zone_source doit être déposée dans le renderer
 
 SDL_GetWindowSize( 
 window, &window_dimensions.w, 
 &window_dimensions.h); // Récupération des dimensions de la fenêtre
 SDL_QueryTexture(my_texture, NULL, NULL, 
 &source.w, 
 &source.h); // Récupération des dimensions de l'image
 
 /* On décide de déplacer dans la fenêtre cette image */
 float zoom = 0.25; // Facteur de zoom entre l'image source et l'image affichée
 
 int nb_it = 200; // Nombre d'images de l'animation
 destination.w = source.w * zoom; // On applique le zoom sur la largeur
 destination.h = source.h * zoom; // On applique le zoom sur la hauteur
 destination.x = 
 (window_dimensions.w - destination.w) / 2; // On centre en largeur
 float h = window_dimensions.h - destination.h; // hauteur du déplacement à effectuer

 for (int i = 0; i < nb_it; ++i) {
 destination.y =
 h * (1 - exp(-5.0 * i / nb_it) / 2 *
 (1 + cos(10.0 * i / nb_it * 2 *
 M_PI))); // hauteur en fonction du numéro d'image

 SDL_RenderClear(renderer); // Effacer l'image précédente

 SDL_SetTextureAlphaMod(my_texture,(1.0-1.0*i/nb_it)*255); // L'opacité va passer de 255 à 0 au fil de l'animation
 SDL_RenderCopy(renderer, my_texture, &source, &destination); // Préparation de l'affichage
 SDL_RenderPresent(renderer); // Affichage de la nouvelle image
 SDL_Delay(30); // Pause en ms
 } 
}

void play_with_texture_4(SDL_Texture* my_texture, SDL_Window* window, SDL_Renderer* renderer) {
 SDL_Rect 
 source = {0}, // Rectangle définissant la zone totale de la planche
 window_dimensions = {0}, // Rectangle définissant la fenêtre, on n'utilisera que largeur et hauteur
 destination = {0}, // Rectangle définissant où la zone_source doit être déposée dans le renderer
 state = {0}; // Rectangle de la vignette en cours dans la planche 

 SDL_GetWindowSize(window, // Récupération des dimensions de la fenêtre
 &window_dimensions.w,
 &window_dimensions.h);
 SDL_QueryTexture(my_texture, // Récupération des dimensions de l'image
 NULL, NULL,
 &source.w, &source.h);

 /* Mais pourquoi prendre la totalité de l'image, on peut n'en afficher qu'un morceau, et changer de morceau :-) */

 int nb_images = 8; // Il y a 8 vignette dans la ligne de l'image qui nous intéresse
 float zoom = 2; // zoom, car ces images sont un peu petites
 int offset_x = source.w / nb_images, // La largeur d'une vignette de l'image, marche car la planche est bien réglée
 offset_y = source.h / 4; // La hauteur d'une vignette de l'image, marche car la planche est bien réglée


 state.x = 0 ; // La première vignette est en début de ligne
 state.y = 3 * offset_y; // On s'intéresse à la 4ème ligne, le bonhomme qui court
 state.w = offset_x; // Largeur de la vignette
 state.h = offset_y; // Hauteur de la vignette

 destination.w = offset_x * zoom; // Largeur du sprite à l'écran
 destination.h = offset_y * zoom; // Hauteur du sprite à l'écran

 destination.y = // La course se fait en milieu d'écran (en vertical)
 (window_dimensions.h - destination.h) /2;

 int speed = 9;
 for (int x = 0; x < window_dimensions.w - destination.w; x += speed) {
 destination.x = x; // Position en x pour l'affichage du sprite
 state.x += offset_x; // On passe à la vignette suivante dans l'image
 state.x %= source.w; // La vignette qui suit celle de fin de ligne est
 // celle de début de ligne

 SDL_RenderClear(renderer); // Effacer l'image précédente avant de dessiner la nouvelle
 SDL_RenderCopy(renderer, my_texture, // Préparation de l'affichage
 &state,
 &destination); 
 SDL_RenderPresent(renderer); // Affichage
 SDL_Delay(80); // Pause en ms
 }
}

void play_with_texture_5(SDL_Texture *bg_texture, SDL_Texture *my_texture, SDL_Window *window, SDL_Renderer *renderer) {
 SDL_Rect 
 source = {0},
 window_dimensions = {0},
 destination = {0} ; 

 SDL_GetWindowSize(window, &window_dimensions.w, &window_dimensions.h); 
 SDL_QueryTexture(my_texture, NULL, NULL, &source.w, &source.h); 


 int nb_images = 8;
 int nb_images_animation = 1 * nb_images;
 float zoom = 4; // zoom, car ces images sont un peu petites 
 int offset_x = source.w / 8, offset_y = source.h ; // La hauteur d'une vignette de l'image
 SDL_Rect state[nb_images]; 

 /* construction des différents rectangles autour de chacune des vignettes de la planche */
 int i = 0; 
 for (int y = 0; y < source.h ; y += offset_y) {
 for (int x = 0; x < source.w; x += offset_x) {
 state[i].x = x;
 state[i].y = y;
 state[i].w = offset_x;
 state[i].h = offset_y;
 ++i;
 }
 }
 destination.w = offset_x * zoom; 
 destination.h = offset_y * zoom; 
 destination.x = window_dimensions.w / 2 - destination.w/2 + 200; 
 destination.y = window_dimensions.h / 2 - destination.h/2; 


 i = 0;
 for (int cpt = 0; cpt < nb_images_animation ; ++cpt) {
 play_with_texture_1_1(bg_texture, window, renderer); 
 SDL_RenderCopy(renderer, my_texture, &state[i], &destination);
 i = (i + 1) % nb_images; 
 SDL_RenderPresent(renderer); 
 SDL_Delay(100); 
 SDL_RenderClear(renderer); 
 }
}

int main(void) {
 SDL_Window *window = NULL;
 SDL_DisplayMode DM;

 int flags = IMG_INIT_JPG|IMG_INIT_PNG;
 int initted = IMG_Init(flags);

 if((initted&flags) != flags){
 printf("IMG_Init: Impossible d'initialiser le support des formats JPG et PNG requis!\n");
 printf("IMG_Init: %s\n", IMG_GetError());
 exit(EXIT_FAILURE);
 }

 if (SDL_Init(SDL_INIT_VIDEO) != 0) {
 SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
 exit(EXIT_FAILURE);
 }

 /* Taille de l'écran */
 if (SDL_GetCurrentDisplayMode(0, &DM) != 0){
 SDL_Log("Error : Get display size - %s\n", SDL_GetError());
 exit(EXIT_FAILURE);
 }
 const int height = DM.h;
 const int width = DM.w;

 window = SDL_CreateWindow("Sprite", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width/1.5f, height/1.5f, SDL_WINDOW_RESIZABLE);

 if (window == NULL) {
 SDL_Log("Error : SDL window creation - %s\n", SDL_GetError()); 
 SDL_Quit(); 
 exit(EXIT_FAILURE);
 }

 SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
 if (renderer == NULL) {
 SDL_DestroyWindow(window);
 fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
 exit(EXIT_FAILURE);
 }

 SDL_Texture *guard = load_texture_from_image("guard.png", window, renderer);

 int quit = 0;
 SDL_Event e;

 SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
 SDL_RenderClear(renderer); 

 while (!quit) {
 while (SDL_PollEvent(&e)) {
 switch (e.type) {
 case SDL_QUIT :
 printf("on quitte\n"); 
 quit = 1;
 break;

 case SDL_KEYDOWN :
 if(e.key.keysym.sym == SDLK_q){
 quit = 1;
 printf("on quitte\n");
 break;
 }
 default:
 }
 }
 SDL_Delay(DELAY);

 play_with_texture_5(NULL, guard, window, renderer); 

 SDL_RenderPresent(renderer);
 SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
 SDL_RenderClear(renderer); 
 }

 SDL_DestroyWindow(window); 
 SDL_DestroyRenderer(renderer);
 SDL_DestroyTexture(guard);
 IMG_Quit(); 
 SDL_Quit(); 

 return 0;
}
