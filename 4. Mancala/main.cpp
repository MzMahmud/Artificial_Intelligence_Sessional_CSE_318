#include "state.h"

int main(int argc, char const *argv[]) {
    state a(heuristic_4,heuristic_4);
    for(int i = 0;i < 6;i++){
        a.holes[PLAYER_B + i] = 0;
    }

    cout << boolalpha << (PLAYER_A == a.declare_winner(a.is_game_over())) << endl;

    a.turn = PLAYER_A;
    a.holes[A_STORE] = 19;
    a.holes[B_STORE] = 7;
    a.a_captured = 2;
    a.b_captured = 1;
    a.a_move_earned = 3;
    a.b_move_earned = 1;

    a.print();
    cout << a.heuristic_a(a) << endl;

    a.turn = PLAYER_B;
    cout << a.heuristic_b(a) << endl;

    state b;
    b = a;
    b.turn = PLAYER_A;
    b.holes[A_STORE] = 19;
    b.holes[B_STORE] = 7;
    b.a_captured = 2;
    b.b_captured = 1;
    b.a_move_earned = 3;
    b.b_move_earned = 1;

    b.print();
    cout << b.heuristic_a(b) << endl;

    b.turn = PLAYER_B;
    cout << b.heuristic_b(b) << endl;
    // a.turn = PLAYER_B;
    // //a.holes[PLAYER_B + 2] = 0;
    // a.holes[PLAYER_B + 4] = 15;
    // //a.holes[A_STORE] = 3;
    // //a.holes[B_STORE] = 5;
    // state b = a.move_from(PLAYER_B + 4);
    // a.print();
    // b.print();
    // cout << boolalpha << (b.turn == PLAYER_B) << endl;
    // cout << b.additional_move_earned << " " << b.stones_captured << endl;


    return 0;
}
