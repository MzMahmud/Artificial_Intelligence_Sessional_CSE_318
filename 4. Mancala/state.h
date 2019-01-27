#ifndef STATE_H
#define STATE_H

#include <bits/stdc++.h>

using namespace std;

#define PLAYER_A 0
#define PLAYER_B 7
#define A_STORE 6
#define B_STORE 13

struct state{
    int holes[14];//holes in Mancala Board
    int value;//value of this state
    int turn;//which players turn to move
    int a_move_earned,b_move_earned;
    int a_captured,b_captured;
    int (*heuristic_a)(state& );//heuristic function for A
    int (*heuristic_b)(state& );//heuristic function for B

    state();//constructor
    state(int (*heuristic_1)(state& ),
          int (*heuristic_2)(state& ));//constructor with heuristics

    state move_from(int hole);//move from hole->index of hole
    int is_game_over();//is the game over
                      // returns PLAYER_A if A is empty,PLAYER_B if B is empty,-1 otherwise
    int declare_winner(int empty_player);//if game is over declares winner
    state& operator=(const state &other);
    void print();
};

//heuristics
int heuristic_1(state& a);
int heuristic_2(state& a);
int heuristic_3(state& a);
int heuristic_4(state& a);
//weights defined in heuristic calculation
#define W1 1
#define W2 1
#define W3 1
#define W4 1

#endif
