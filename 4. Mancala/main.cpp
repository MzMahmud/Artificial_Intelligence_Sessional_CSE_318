#include "proc.h"

int main(int argc, char const *argv[]) {

    // play_with_other(PLAYER_A,heuristic_3,heuristic_3);

    play_n_games_with_randomPlayer(100,1,PLAYER_A);
    play_n_games_with_randomPlayer(100,2,PLAYER_A);
    play_n_games_with_randomPlayer(100,3,PLAYER_A);
    play_n_games_with_randomPlayer(100,4,PLAYER_A);

    heuristic_func_pointer heuristics[4]
                           = {heuristic_1,heuristic_2,heuristic_3,heuristic_4};

    for(int i = 0;i < 4;i++){
        for(int j = 0;j < 4;j++){
            printf("*****A(heuristic_%d) VS B(heuristic_%d)*****\n--Final Board--\n",i+1,j+1);

            int res = heuristic_vs_heuristic(heuristics[i],heuristics[j]);

            if(res == 0)
                printf("Drawn\n\n\n");
            if(res < 0)
                printf("A(heuristic_%d) WINS with %d points\n\n\n",i+1,-res);
            if(res > 0)
                printf("B(heuristic_%d) WINS with %d points\n\n\n",j+1,res);
        }
    }

    return 0;
}
