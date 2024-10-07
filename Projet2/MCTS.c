#include "MCTS.h" 

Play player_intention_func(Game *game) {
    Play play = NULL;
    int hand_size = NB_CARDS / game->nb_players;

    printf("player_intention_func: stack.nb = %d, turn = %d\n", game->stack.nb, game->turn);

    switch (game->stack.nb) {
        case 1:
            if ((existe1(2, game->hands[game->turn], hand_size) == 1) && 
                (Nb_card_hand(game->hands[game->turn], hand_size) > 1) && 
                (Nb_card_hand(game->hands[(game->turn + 1) % 4], hand_size) < 2) && 
                (Nb_card_hand(game->hands[game->turn], hand_size) < 7)) {
                
                int index = get_card_index(game->hands[game->turn], hand_size, 2);
                play = listAdd(play, index);
            } else {
                Playlist *playlist = All_play(*game);
                play = playlist->play;
            }
            break;
        case 2:
            if ((existe2(2, game->hands[game->turn], hand_size) == 1) && 
                (Nb_card_hand(game->hands[game->turn], hand_size) > 2) && 
                (Nb_card_hand(game->hands[(game->turn + 1) % 4], hand_size) < 3) && 
                (Nb_card_hand(game->hands[game->turn], hand_size) < 7)) {
                
                int index1 = get_card_index(game->hands[game->turn], hand_size, 2);
                int index2 = get_card_index(game->hands[game->turn], hand_size, 2);
                if (index1 != -1 && index2 != -1 && index1 != index2) {
                    play = listAdd(play, index1);
                    play = listAdd(play, index2);
                }
            } else {
                Playlist *playlist = All_play(*game);
                play = playlist->play;
            }
            break;
        case 3:
            {
                Playlist *playlist = All_play(*game);
                play = playlist->play;
            }
            break;
        case 4:
            play = NULL;
            break;
    }
    return play;
}

int isFinished(Game *game, Play player_intention) {
    int finished_players = 0;
    for (int i = 0; i < game->nb_players; i++) {
        if (Is_hand_empty(game->hands[i], NB_CARDS / game->nb_players)) {
            finished_players++;
        }
    }
    if (finished_players >= game->nb_players - 1) {
        return 1; 
    }

    if (Is_hand_empty(game->hands[game->turn], NB_CARDS / game->nb_players)) {
        return 1; 
    }

    if (player_intention == NULL) {
        return 1; 
    }

    return 0; 
}

int existe1(int rank, Hand hand, int hand_size) {
    for (int i = 0; i < hand_size; i++) {
        if ((int)hand[i].type == rank && hand[i].no_card == 0) { // assuming no_card is 0 if the card is present
            return 1;
        }
    }
    return 0;
}

int existe2(int rank, Hand hand, int hand_size) {
    int count = 0;
    for (int i = 0; i < hand_size; i++) {
        if ((int)hand[i].type == rank && hand[i].no_card == 0) { // assuming no_card is 0 if the card is present
            count++;
            if (count == 2) {
                return 1;
            }
        }
    }
    return 0;
}

int get_card_index(Hand hand, int hand_size, int rank) {
    for (int i = 0; i < hand_size; i++) {
        if ((int)hand[i].type == rank && hand[i].no_card == 0) { // assuming no_card is 0 if the card is present
            return i;
        }
    }
    return -1;
}

Game copy_game(Game game) {
    Game copy;
    copy.nb_players = game.nb_players;
    copy.turn = game.turn;
    copy.win = game.win;

    copy.hands = (Hand*)malloc(4 * sizeof(Hand));
    copy.leaderboard = (int*)malloc(4 * sizeof(int));
    copy.stack.stack = CardlistCopy(game.stack.stack);
    copy.stack.nb = game.stack.nb;
    copy.stack.belongs_to = game.stack.belongs_to;
    copy.stack.top = game.stack.top;

    for(int i=0; i<4; ++i){
        copy.hands[i] = (Card*)malloc(13*sizeof(Card));
        for(int j=0; j<13; ++j){
            copy.hands[i][j] = game.hands[i][j];
        }
    }

    for(int i=0; i<4; ++i)
        copy.leaderboard[i] = game.leaderboard[i];

    return copy;
}

Game createRandomGame(Game *game) {
    Game copy = copy_game(*game);
    Play play;
    for (int i = 0; i < 3; i++) {
        play = tirage_aleatoire(&copy);
        Apply_play(&copy, play);
        //listFree(play); // Assuming that tirage_aleatoire allocates memory for the play
    }
    return copy;
}

int simulation(Game* game, Play play){
    Game copy = copy_game(*game);
    Apply_play(&copy, play);
    return eval(copy);
}

Game simulated_final_game_state(Game game) {
    // while (!Is_game_over(game)) {
    //     Play coup = tirage_aleatoire(&game);
    //     // listDisplay(coup);
    //     Apply_play(&game, coup);
    // }
    return game;
}

int sizeofList(Playlist *playlist){
    int compteur = 0; 
    while(playlist != NULL){
        compteur ++ ;
        playlist = playlist->next ;
    }
    return compteur ;
}

int* evaluation(Game game) {
    int* score = malloc(NB_PLAYERS * sizeof(int));
    if (score == NULL) {
        fprintf(stderr, "Erreur : Impossible d'allouer la mémoire pour le tableau de scores.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < NB_PLAYERS; i++) {
        score[i] = 0;
    }
    if (game.leaderboard != NULL) {
        int n = 4;
        for (int i = 0; i < n; i++) {
            score[game.leaderboard[i]] += 10000 / (i + 1);
        }
    }
    for (int i = 0; i < NB_PLAYERS; i++) {
        if (Is_hand_empty(game.hands[i], NB_CARDS / 4)) {
            score[i] += 10000;
        }
    }
    if (game.leaderboard[NB_PLAYERS - 1] == game.turn) {
        score[game.leaderboard[NB_PLAYERS - 1]] += -1000;
    }
    return score;
}

Play getPlay(Playlist *playlist, int n){
    for(int i = 0; i<n-2; i++){
        playlist = playlist->next ;
    }
    return playlist->play ;
}

Play tirage_aleatoire(Game *game) {
    Playlist *playlist = All_play(*game);
    int n = sizeofList(playlist);
    if (n == 0) {
        fprintf(stderr, "Erreur : Aucune combinaison de jeu disponible.\n");
        exit(EXIT_FAILURE);
    }
    int random_number = rand() % n;
    Play selected_play = getPlay(playlist, random_number);
    //PlaylistFree(playlist);
    return selected_play;
}

int eval(Game game) {
    Game final_state = simulated_final_game_state(game);
    int *scores = evaluation(final_state);
    int result = scores[game.turn];
    printf("az\n");
    //free(scores);
    return result;
}

Play_gain ucb(MCTSNode *node) {
    int indicemax = 0;
    double max_ucb = -INFINITY;

    for (int i = 0; i < node->number_play; ++i) {
        if (node->play_gains[i].gain == 0) {
            return node->play_gains[i];
        }

        double g = (double) node->play_gains[i].gain / node->play_gains[i].gain;
        double e = C * sqrt(log(node->total_number) / node->play_gains[i].gain);
        double ucb_value = g + e;

        if (ucb_value > max_ucb) {
            max_ucb = ucb_value;
            indicemax = i;
        }


    }

    return node->play_gains[indicemax];
}

unsigned long hash_card(Card *card) {
    unsigned long hash = 5381;
    hash = ((hash << 5) + hash) + card->type;
    hash = ((hash << 5) + hash) + card->c;
    hash = ((hash << 5) + hash) + card->s;
    return hash;
}

unsigned long hash_cardlist(Cardlist *cl) {
    unsigned long hash = 5381;
    Cardlist *current = cl;
    while (current != NULL) {
        hash = ((hash << 5) + hash) + hash_card(&current->c);
        current = current->next;
    }
    return hash;
}

unsigned long hash_hand(Hand h, int n) {
    unsigned long hash = 5381;
    for (int i = 0; i < n; i++) {
        hash = ((hash << 5) + hash) + hash_card(&h[i]);
    }
    return hash;
}

unsigned long hash_player(Player *player) {
    unsigned long hash = 5381;
    hash = ((hash << 5) + hash) + hash_hand(player->hand, player->hand_size);
    hash = ((hash << 5) + hash) + player->player_rank;
    hash = ((hash << 5) + hash) + player->is_turn;
    hash = ((hash << 5) + hash) + player->is_done;
    hash = ((hash << 5) + hash) + hash_cardlist(player->played_cards);
    return hash;
}

unsigned long hash_player_view(PlayerView *player) {
    unsigned long hash = 5381;
    hash = ((hash << 5) + hash) + hash_hand(player->hand, player->hand_size);
    hash = ((hash << 5) + hash) + player->player_rank;
    hash = ((hash << 5) + hash) + player->is_turn;
    hash = ((hash << 5) + hash) + player->is_done;
    hash = ((hash << 5) + hash) + hash_cardlist(player->played_cards);
    return hash;
}

unsigned long hash_stack(Stack *stack) {
    unsigned long hash = 5381;
    hash = ((hash << 5) + hash) + stack->nb;
    hash = ((hash << 5) + hash) + stack->belongs_to;
    hash = ((hash << 5) + hash) + hash_card(&stack->top);
    hash = ((hash << 5) + hash) + hash_cardlist(stack->stack);
    return hash;
}

unsigned long hash_view(View *view) {
    unsigned long hash = 5381;
    for (int i = 0; i < view->num_players; i++) {
        hash = ((hash << 5) + hash) + hash_player_view(&view->players[i]);
    }
    hash = ((hash << 5) + hash) + view->current_turn;
    hash = ((hash << 5) + hash) + view->round;
    hash = ((hash << 5) + hash) + hash_card(&view->top_card);
    hash = ((hash << 5) + hash) + view->top_rank;
    return hash;
}

void getPlayerView(Hand hand, int hand_size, int player_rank, int is_turn, int is_done, PlayerView *playerview) {
    playerview->hand = hand;
    playerview->hand_size = hand_size;
    playerview->player_rank = player_rank;
    playerview->is_turn = is_turn;
    playerview->is_done = is_done;
    playerview->played_cards = NULL; // Assuming you have a way to set played cards if needed
}

void getView(Game *game, View *view) {
    view->num_players = game->nb_players;
    view->players = malloc(sizeof(PlayerView) * game->nb_players);
    
    for (int i = 0; i < game->nb_players; ++i) {
        int is_turn = (game->turn == i);
        int is_done = Is_hand_empty(game->hands[i], NB_CARDS / game->nb_players);
        int player_rank = -1;

        for (int j = 0; j < game->nb_players; ++j) {
            if (game->leaderboard[j] == i) {
                player_rank = j;
                break;
            }
        }

        getPlayerView(game->hands[i], NB_CARDS / game->nb_players, player_rank, is_turn, is_done, &view->players[i]);
    }

    view->current_turn = game->turn;
    view->round = game->turn;
    if (game->stack.stack != NULL) {
        view->top_card = game->stack.top;
    } else {
        view->top_card.no_card = 1; // Assuming a card with no_card = 1 means there is no top card
    }
    view->top_rank = game->leaderboard[game->turn];
}

Tree *createTree(MCTSNode *node) {
    Tree *tree = malloc(sizeof(Tree));
    if (tree == NULL) {
        fprintf(stderr, "Erreur : Impossible d'allouer la mémoire pour Tree.\n");
        exit(EXIT_FAILURE);
    }
    tree->color = RED;
    tree->link[0] = NULL;
    tree->link[1] = NULL;
    tree->node = *node;

    View *view = malloc(sizeof(View));
    if (view == NULL) {
        fprintf(stderr, "Erreur : Impossible d'allouer la mémoire pour View.\n");
        free(tree);
        exit(EXIT_FAILURE);
    }
    getView(&node->game, view);
    tree->hash = hash_view(view);
    free(view);

    return tree;
}

MCTSNode *createMCTSNode(Game *game) {
    // Allouer de la mémoire pour le nouveau nœud
    MCTSNode *node = malloc(sizeof(MCTSNode));
    if (node == NULL) {
        fprintf(stderr, "Erreur : Impossible d'allouer la mémoire pour MCTSNode.\n");
        exit(EXIT_FAILURE);
    }

    // Copier l'état du jeu dans le nœud
    node->game = copy_game(*game);

    // Initialiser le tableau de play_gains
    node->number_play = 0;  // Initialement, il n'y a pas de coups enregistrés
    node->total_number = 0; // Initialiser le nombre total de simulations
    node->play_gains = NULL; // Initialiser à NULL (sera alloué dynamiquement plus tard si nécessaire)

    // Retourner le pointeur vers le nouveau nœud
    return node;
}

void rbRotateRight(Tree **y, Tree **papa, int son) {
    if (y != NULL && *y != NULL) {
        Tree *temp = (*y)->link[0];
        if (temp != NULL) {
            (*y)->link[0] = temp->link[1];
            temp->link[1] = *y;
            if (*y != *papa) {
                (*papa)->link[son] = temp;
            } else {
                *papa = temp;
            }
        }
    }
}

void rbRotateLeft(Tree **y, Tree **papa, int son) {
    if (y != NULL && *y != NULL) {
        Tree *temp = (*y)->link[1];
        if (temp != NULL) {
            (*y)->link[1] = temp->link[0];
            temp->link[0] = *y;
            if (*y != *papa) {
                (*papa)->link[son] = temp;
            } else {
                *papa = temp;
            }
        }
    }
}

void rbRotate(Tree **x, Tree **papa, int right, int son) {
    if (right) {
        rbRotateRight(x, papa, son);
    } else {
        rbRotateLeft(x, papa, son);
    }
}

Tree *insertTree(Tree *root, MCTSNode *node) {
    Tree *stack[100], *ptr, *newTree;
    int dir[100], ht = 0, index;
    ptr = root;

    if (!root) {
        root = createTree(node);
        root->color = BLACK;
        return root;
    }

    unsigned long hash = hash_state(&node->game);
    stack[ht] = root;
    dir[ht++] = 0;

    while (ptr != NULL) {
        index = (hash - ptr->hash) > 0 ? 1 : 0;
        stack[ht] = ptr;
        ptr = ptr->link[index];
        dir[ht++] = index;
    }

    newTree = createTree(node);
    stack[ht - 1]->link[index] = newTree;

    while (ht >= 3 && stack[ht - 1]->color == RED) {
        if (stack[ht - 2]->link[1 - dir[ht - 2]] != NULL && stack[ht - 2]->link[1 - dir[ht - 2]]->color == RED) {
            stack[ht - 2]->color = RED;
            stack[ht - 2]->link[1 - dir[ht - 2]]->color = BLACK;
            stack[ht - 1]->color = BLACK;
            ht -= 1;
        } else {
            if (dir[ht - 1] != dir[ht - 2]) {
               

 rbRotate(&stack[ht - 1], &stack[ht - 2], dir[ht - 2], dir[ht - 2]);
                Tree *temp = stack[ht - 1];
                stack[ht - 1] = newTree;
                newTree = temp;
                dir[ht - 1] = dir[ht - 2];
            }
            rbRotate(&stack[ht - 2], &stack[ht - 3], 1 - dir[ht - 2], dir[ht - 3]);
            stack[ht - 1]->color = BLACK;
            stack[ht - 2]->color = RED;
            dir[ht - 2] = dir[ht - 1];
            if (ht == 3) {
                root = stack[ht - 3];
            }
            ht -= 1;
        }
        ht -= 1;
    }

    root->color = BLACK;
    return root;
}

MCTSNode *getNode(Game *game, Tree *root, int *indice) {
    View *view = malloc(sizeof(View));
    if (view == NULL) {
        fprintf(stderr, "Erreur : Impossible d'allouer la mémoire pour View.\n");
        exit(EXIT_FAILURE);
    }
    getView(game, view);
    unsigned long hash = hash_view(view);
    free(view);

    printf("z\n");

    if (root != NULL) {
        View *viewnode = malloc(sizeof(View));
        if (viewnode == NULL) {
            fprintf(stderr, "Erreur : Impossible d'allouer la mémoire pour View.\n");
            exit(EXIT_FAILURE);
        }
        getView(&(root->node.game), viewnode);
        unsigned long hashnode = hash_view(viewnode);
        free(viewnode);

        if (hashnode == hash) {
            return &(root->node);
        } else if (hashnode < hash) {
            return getNode(game, root->link[0], indice);
        } else {
            return getNode(game, root->link[1], indice);
        }
    } else {
        *indice = 1;
        MCTSNode *new_node = createMCTSNode(game);
        Tree *newTree = insertTree(root, new_node);
        return &newTree->node;
    }
}

// Définition de hash_state
unsigned long hash_state(Game *game) {
    unsigned long hash = 5381;
    hash = ((hash << 5) + hash) + game->nb_players;
    hash = ((hash << 5) + hash) + game->turn;
    hash = ((hash << 5) + hash) + game->win;
    for (int i = 0; i < game->nb_players; i++) {
        hash = ((hash << 5) + hash) + hash_hand(game->hands[i], NB_CARDS / game->nb_players);
    }
    hash = ((hash << 5) + hash) + hash_stack(&game->stack);
    for (int i = 0; i < game->nb_players; i++) {
        hash = ((hash << 5) + hash) + game->leaderboard[i];
    }
    return hash;
}

// Définition de incrMove
void incrMove(MCTSNode *node, int i) {
    node->total_number += 1;
    node->play_gains[i].gain += 1;
}

int mcts(Game *game, Tree *root) {
    int mon_indice = 0;
    MCTSNode *node = getNode(game, root, &mon_indice);
    printf("mcts: game.turn = %d, mon_indice = %d\n", game->turn, mon_indice);

    if (mon_indice == 0) {
        printf("yousk\n");
        Play_gain *play_gain = NULL;
        Play player_intention;
        int terminal_node = 0;

        if (node->total_number < node->number_play) {
            play_gain = &node->play_gains[node->total_number];
            incrMove(node, node->total_number);
        } else {
            *play_gain = ucb(node);
        }

        player_intention = player_intention_func(game);
        Game random_game = createRandomGame(game);
        listDisplay(play_gain->play);
        Apply_play(&random_game, play_gain->play);

        terminal_node = isFinished(&random_game, player_intention);

        int gain;
        if (terminal_node) {
            gain = eval(random_game);
            play_gain->gain += gain;
            free(player_intention);
            return gain;
        } else {
            gain = mcts(&random_game, root);
            play_gain->gain += gain;
            free(player_intention);
            return gain;
        }
    } else {
        Play_gain *play_gain = &node->play_gains[0];
        Play player_intention;

        player_intention = player_intention_func(game);

        int gain = simulation(game, player_intention);
        //play_gain->gain += gain;
        printf("a\n");
        //free(player_intention);
        return gain;
    }
}

Play play_IA(Game *game, Tree *root, int iterations) {
    while (iterations != 0) {
        Game random_game = createRandomGame(game);
        printf("play_IA: iteration = %d\n", iterations);
        mcts(&random_game, root);
        printf("zzz\n");
        iterations--;
    }

    int nb_coup = root->node.number_play;
    printf("aaaaa\n");
    int max = -1; // Initialiser à -1 pour s'assurer que toute valeur positive sera plus grande
    int indicemax = 0;

    for (int i = 0; i < nb_coup; ++i) {
        int gain = root->node.play_gains[i].gain;
        int total_number = root->node.total_number;
        if (max < gain / total_number) {
            max = gain / total_number;
            indicemax = i;
        }
    }
    return root->node.play_gains[indicemax].play;
}