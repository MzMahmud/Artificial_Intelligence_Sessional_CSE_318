#include "state.h"

//returns the best move for player
#define INF INT_MAX
#define MAX_DEPTH 10

typedef pair<int,int> pii;

pii minimax(state position,int depth,int alpha,int beta,int player){
    if(depth == 0 || position.is_game_over() >= 0){
        int heuristic_val;

        if(player == PLAYER_A)
            heuristic_val = position.heuristic_a(position);
        else
            heuristic_val = position.heuristic_b(position);

        return make_pair(-1,heuristic_val);
    }

    vector<int> possible_moves;
    for(int i = 0;i < 6;i++)
        if(position.holes[player + i])
            possible_moves.push_back(player + i);

    if(player == PLAYER_A){//maximizing player
        int maxEval  = -INF;
        int opt_move = -1;
        for(int i = 0;i < possible_moves.size();i++){
            state child = position.move_from(possible_moves[i]);
            int eval = minimax(child,depth-1,alpha,beta,child.turn).second;

            if(eval > maxEval){
                //maxEval  = max(maxEval, eval);
                maxEval  = eval;
                opt_move = possible_moves[i];
            }
            alpha    = max(alpha, eval);
            if(beta <= alpha) break;
        }
        return make_pair(opt_move,maxEval);
    }
    else{
        int minEval = INF;
        int opt_move = -1;
        for(int i = 0;i < possible_moves.size();i++){
            state child = position.move_from(possible_moves[i]);
            int eval = minimax(child, depth - 1, alpha,beta,child.turn).second;

            if(eval < minEval){
                //minEval = min(minEval, eval);
                minEval  = eval;
                opt_move = possible_moves[i];
            }
            beta = min(beta, eval);
            if(beta <= alpha) break;
        }
        return make_pair(opt_move,minEval);
    }
}


//play as palyer(A or B) with heuristic 1 against other with heuristic_2
void play_with_other(int player,int (*heuristic_a)(state&),int (*heuristic_b)(state&)){
    state game_state(heuristic_a,heuristic_b);

    while(game_state.is_game_over() < 0){
        game_state.print();

        if(game_state.turn == PLAYER_A){
            cout << "*****YOUR MOVE*****" << endl;

            int opt_move = minimax(game_state,
                                   MAX_DEPTH ,-INF,INF,
                                   game_state.turn).first;

            cout << "you should move " << opt_move+1 << endl;
            game_state = game_state.move_from(opt_move);
            //game_state.print();
        }else{
            cout << "****OPP's MOVE****" << endl;
            int to_move;
            cout << "Enter opp's move: ";
            cin >> to_move;
            cout << "Opp Gave " << to_move << endl;
            game_state = game_state.move_from(PLAYER_B + to_move - 1);
        }
        //game_state.print();
    }

    game_state.print();
    int winner = game_state.declare_winner(game_state.is_game_over());

    if(winner == -1)
        cout << "*****Draw!*****" << endl;
    else if(winner == player)
        cout << "***YOU WON!!***" << endl;
    else
        cout << "***you lose!***" << endl;

}
void heuristic_vs_heuristic(int (*heuristic_a)(state&),int (*heuristic_b)(state&)){
    state game_state(heuristic_a,heuristic_b);

    while(game_state.is_game_over() < 0){
        int opt_move = minimax(game_state,MAX_DEPTH,-INF,INF,game_state.turn).first;
        string turn = (game_state.turn == PLAYER_A)?string("PLAYER_A->") : string("PLAYER_B->");
        //cout << turn << "to move " << opt_move << endl;
        game_state = game_state.move_from(opt_move);
        //game_state.print();
        //cout << "****************************\n\n";
    }
    int winner = game_state.declare_winner(game_state.is_game_over());
    game_state.print();
    if(winner == -1){
        cout << "Draw!" << endl;
    }else if(winner == PLAYER_A){
        cout << "WINNER! PLAYER_A" << endl;
    }else if(winner == PLAYER_B){
        cout << "WINNER! PLAYER_B" << endl;
    }
}

int main(int argc, char const *argv[]) {
    play_with_other(PLAYER_A,heuristic_3,heuristic_3);
    return 0;
}
