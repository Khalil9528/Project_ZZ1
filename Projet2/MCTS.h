#ifndef _MCTS_H_
#define _MCTS_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "presidents.h"

// Constants
#define C sqrt(2)

// Enums
typedef enum {
    RED,
    BLACK
} NodeColor;

// Player structures
typedef struct {
    Hand hand;
    int hand_size;
    int player_rank;
    int is_turn;
    int is_done;
    Cardlist *played_cards;
} Player;

typedef struct {
    Hand hand;
    int hand_size;
    int player_rank;
    int is_turn;
    int is_done;
    int passes;
    Cardlist *played_cards;
} PlayerView;

// View structure
typedef struct {
    int num_players;
    PlayerView *players;
    int current_turn;
    int round;
    Card top_card;
    int top_rank;
} View;

// MCTS structures
typedef struct {
    Play play;
    int gain;
} Play_gain;

typedef struct {
    Play_gain *play_gains;
    Game game;
    int number_play;
    int total_number;
} MCTSNode;

typedef struct Tree {
    struct Tree *link[2];
    int color;
    MCTSNode node;
    unsigned long hash;
} Tree;

// Function declarations
int mcts(Game *game, Tree *root);
Play player_intention_func(Game *game);
int isFinished(Game *game, Play player_intention);
int existe1(int rank, Hand hand, int hand_size);
int existe2(int rank, Hand hand, int hand_size);
int get_card_index(Hand hand, int hand_size, int rank);
Game copy_game(Game game);
Game createRandomGame(Game *game);
int simulation(Game* game, Play play);
Game simulated_final_game_state(Game game);
int sizeofList(Playlist *playlist);
int* evaluation(Game game);
Play getPlay(Playlist *playlist, int n);
Play tirage_aleatoire(Game *game);
int eval(Game game);
Play_gain ucb(MCTSNode *node);
unsigned long hash_card(Card *card);
unsigned long hash_cardlist(Cardlist *cl);
unsigned long hash_hand(Hand h, int n);
unsigned long hash_player(Player *player);
unsigned long hash_stack(Stack *stack);
unsigned long hash_view(View *view);
void getPlayerView(Hand hand, int hand_size, int player_rank, int is_turn, int is_done, PlayerView *playerview);
void getView(Game *game, View *view);
Tree *createTree(MCTSNode *node);
void rbRotateRight(Tree **y, Tree **papa, int son);
void rbRotateLeft(Tree **y, Tree **papa, int son);
void rbRotate(Tree **x, Tree **papa, int right, int son);
Tree *insertTree(Tree *root, MCTSNode *node);
MCTSNode *getNode(Game *game, Tree *root, int *indice);
Play play_IA(Game *game, Tree *root, int iterations);
MCTSNode *createMCTSNode(Game *game) ;
unsigned long hash_state(Game *game);
void incrMove(MCTSNode *node, int i);


#endif 









