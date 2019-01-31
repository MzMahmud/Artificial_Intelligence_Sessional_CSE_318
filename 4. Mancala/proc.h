#ifndef PROC_H
#define PROC_H

#include "state.h"

#define INF INT_MAX
#define MAX_DEPTH 6

typedef pair<int,int> pii;
typedef int (*heuristic_func_pointer)(state&);

pii minimax(state position,int depth,int alpha,int beta,int player);
int heuristic_randomPlayer(int (*heuristic)(state&),int randomPlayer);
void play_with_other(int player,int (*heuristic_a)(state&),int (*heuristic_b)(state&));
int heuristic_vs_heuristic(int (*heuristic_a)(state&),int (*heuristic_b)(state&));
void play_n_games_with_randomPlayer(int n,int heuristic_number,int player);

#endif
