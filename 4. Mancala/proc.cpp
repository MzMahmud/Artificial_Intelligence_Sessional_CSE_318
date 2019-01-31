#include "proc.h"

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

//heuristic plays against a player who chooses a move at random
int heuristic_randomPlayer(int (*heuristic)(state&),int randomPlayer){
    srand (time(NULL));//seeds time for different rand() value

    state game_state;

    if(randomPlayer == PLAYER_A)
        game_state = state(heuristic_1,heuristic);//B plays with heuristic
    else
        game_state = state(heuristic,heuristic_1);//A plays with heuristic

    while(game_state.is_game_over() < 0){
        //game_state.print();

        if(game_state.turn == randomPlayer){
            // cout << "*****Random Player's MOVE*****" << endl;

            // gets the vailble moves
            vector<int> available_moves;
            for(int i = 0;i < 6;i++)
                if(game_state.holes[randomPlayer + i])
                    available_moves.push_back(randomPlayer + i);

            int random_number = abs(rand()) % ((int) available_moves.size());
            int rand_move = available_moves[random_number];

            // cout << "Random Player Moved: " << rand_move+1 << endl;
            game_state = game_state.move_from(rand_move);
            // game_state.print();
        }else{
            // cout << "*****Heuristic MOVE*****" << endl;

            int opt_move = minimax(game_state,
                                   MAX_DEPTH ,-INF,INF,
                                   game_state.turn).first;

            // cout << "Heuristic Moved: " << opt_move+1 << endl;
            game_state = game_state.move_from(opt_move);
        }
        // game_state.print();
    }

    // game_state.print();
    int winner = game_state.declare_winner(game_state.is_game_over());

    if(winner == -1){
        // cout << "*****Draw!*****" << endl;
        return 0;
    }
    else if(winner == randomPlayer){
        // cout << "***Random Player WON!!***" << endl;
        return -1;
    }
    else{
        // cout << "***Heuristic WON!!***" << endl;
        return 1;
    }
}

//play as palyer(A or B) with heuristic 1 against other with heuristic_2
void play_with_other(int player,int (*heuristic_a)(state&),int (*heuristic_b)(state&)){
    state game_state(heuristic_a,heuristic_b);

    while(game_state.is_game_over() < 0){
        game_state.print();

        if(game_state.turn == player){
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

//0->draw,-ve left heuristic won,+ve right heuristic won
int heuristic_vs_heuristic(int (*heuristic_a)(state&),int (*heuristic_b)(state&)){
    state game_state(heuristic_a,heuristic_b);

    while(game_state.is_game_over() < 0){
        int opt_move = minimax(game_state,MAX_DEPTH,-INF,INF,game_state.turn).first;
        //string turn = (game_state.turn == PLAYER_A)?string("PLAYER_A->") : string("PLAYER_B->");
        //cout << turn << "to move " << opt_move << endl;
        game_state = game_state.move_from(opt_move);
        //game_state.print();
        //cout << "****************************\n\n";
    }
    int winner = game_state.declare_winner(game_state.is_game_over());
    game_state.print();
    if(winner == -1){
        //cout << "Draw!" << endl;
        return  0;
    }else if(winner == PLAYER_A){
        //cout << "WINNER! PLAYER_A" << endl;
        return (game_state.holes[B_STORE] - game_state.holes[A_STORE]);
    }else if(winner == PLAYER_B){
        //cout << "WINNER! PLAYER_B" << endl;
        return (game_state.holes[B_STORE] - game_state.holes[A_STORE]);
    }
}

void play_n_games_with_randomPlayer(int n,int heuristic_number,int player){
    int win,lose,draw;
    win = lose = draw = 0;
    for(int i = 0;i < n;i++){
        int res;
        switch (heuristic_number) {
            case 1:
                res = heuristic_randomPlayer(heuristic_1,player);break;
            case 2:
                res = heuristic_randomPlayer(heuristic_2,player);break;
            case 3:
                res = heuristic_randomPlayer(heuristic_3,player);break;
            case 4:
                res = heuristic_randomPlayer(heuristic_4,player);break;
            default:
                cout << "No Heuristic With Number!! " << heuristic_number << endl;
                return;
        }
             if(res ==  0) draw++;
        else if(res ==  1) win++;
        else if(res == -1) lose++;
    }
    printf("*************Heuristic_%d Stat*************\n",heuristic_number);
    printf("Game Played %d\n",n);
    printf("Game Won    %d\n",win);
    printf("Game Lose   %d\n",lose);
    printf("Game Drawn  %d\n",draw);
    printf("Win Ration  %.2lf\n\n",(win*100.0)/n);
}
