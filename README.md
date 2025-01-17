# President_Card_Game (Week 2)

The goal was to implement a game of incomplete information to apply the mcts algorithm.  
We chose President(Asshole) Card game.

## Features

- [x] Play through SDL interface
- [ ] MCTS algorithm

## Our Implementation

### Demo (4x time speed)

<img src="images/president.mp4" width="800" height="400">

### How to play ?

- `make` and then `./main`
- Switch cards in your hand with right click.
- Select cards to play with left click.
- Play (Cards you selected, if not you pass) by pressing space bar.  
[Rules Source](https://www.wikihow.com/Play-President-(Card-Game))

### Image
![image](images/img_pres.png)


# Malawi_Game (Week 1) 

School project ZZ1 ISIMA by group of 3 made in 2.5 days.  
The goal was to implement an atypic board game as well as the minmax algorithm so you can play when you have no friends.

## Features

- [x] Play through SDL interface
- [x] Minmax algorithm

## Malawi

![image](images/malawi.jpg)

## Our Implementation
### Demo (3x time speed)

![video](images/malawi.gif)

### How to play ?

- `make` and then `./main`
- Each player starts with 6 pawns having 2 marbles each.
- Move a pawn : Horizontally or vertically the number of marbles of the pawn when no obstacles in sight.
- Split marbles : A pawn loses all of his marbles and give each one of them to his favorite brothers, if he has 6 marbles we lose 1 marble.
- Attack a pawn : Same as move except you do not move and the opponent pawn loses all his marbles.
- To win : A pawn is on the opponent back row unnaked and cannot be attacked or Opponent has no more marbles.  
[Rules Source](https://boardgamegeek.com/boardgame/1045/malawi)

### Images
![image](images/img3.png)
![image](images/img1.png)


