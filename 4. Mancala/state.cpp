#include "state.h"

state::state(){
    for(int i = 0;i < 14;i++)
        this->holes[i] = 4;

    this->holes[A_STORE] = this->holes[B_STORE] = 0;
    this->value = 0;
    this->turn  = PLAYER_A;
    this->a_move_earned = 0;
    this->b_move_earned = 0;
    this->a_captured = 0;
    this->b_captured = 0;
}

state::state(int (*heuristic_1)(state& ),int (*heuristic_2)(state& )){
    for(int i = 0;i < 14;i++)
        this->holes[i] = 4;

    this->holes[A_STORE] = this->holes[B_STORE] = 0;
    this->value = 0;
    this->turn  = PLAYER_A;
    this->a_move_earned = 0;
    this->b_move_earned = 0;
    this->a_captured = 0;
    this->b_captured = 0;
    this->heuristic_a = heuristic_1;
    this->heuristic_b = heuristic_2;
}

state& state::operator=(const state &other){
    for(int i = 0;i < 14;i++)
        this->holes[i] = other.holes[i];

    this->value = other.value;
    this->turn  = other.turn;
    this->a_move_earned = other.a_move_earned;
    this->b_move_earned = other.b_move_earned;
    this->a_captured    = other.a_captured;
    this->b_captured    = other.b_captured;
    this->heuristic_a   = other.heuristic_a;
    this->heuristic_b   = other.heuristic_b;

    return *this;
}

int state::is_game_over(){
    int a_holes = 0;
    for(int i = 0;i < 6;i++){
        a_holes += this->holes[PLAYER_A + i];
    }//counts total #stones in a's holes
    if(!a_holes) return PLAYER_A;// A is empty

    int b_holes = 0;
    for(int i = 0;i < 6;i++){
        b_holes += this->holes[PLAYER_B + i];
    }//counts total #stones in b's holes
    if(!b_holes) return PLAYER_B;//B is empty

    return -1;//game not over yet
}
int state::declare_winner(int empty_player){
    int non_empty_player
        = (empty_player == PLAYER_A)?(PLAYER_B):(PLAYER_A);

    int captured_stones = 0;
    for(int i = 0;i < 6;i++){
        captured_stones += this->holes[non_empty_player + i];
        this->holes[non_empty_player + i] = 0;
    }

    int store = (non_empty_player == PLAYER_A)?(A_STORE):(B_STORE);
    this->holes[store] += captured_stones;

    if(this->holes[A_STORE] == this->holes[B_STORE])
        return -1;//DRAW

    return (this->holes[A_STORE] > this->holes[B_STORE])?(PLAYER_A):(PLAYER_B);
}

void state::print(){
    printf("-------------------------------------------\n");
    printf("%5s"," ");
    for(int i = 5;i >= 0;i--){
        printf("%5d",this->holes[PLAYER_B + i]);
    }
    printf("%5s\n"," ");

    printf("%5d",this->holes[B_STORE]);
    for(int i = 0;i < 6;i++){
        printf("%5s"," ");
    }
    printf("%5d\n",this->holes[A_STORE]);

    printf("%5s"," ");
    for(int i = 0;i < 6;i++){
        printf("%5d",this->holes[PLAYER_A + i]);
    }
    printf("%5s\n"," ");
    printf("-------------------------------------------\n");

}

state state::move_from(int hole){
    state new_state = *this;

    int me = (new_state.turn == PLAYER_A)?(PLAYER_A):(PLAYER_B);
    int op = (new_state.turn == PLAYER_A)?(PLAYER_B):(PLAYER_A);

    int my_store = (me == PLAYER_A)?(A_STORE):(B_STORE);
    int op_store = (me == PLAYER_A)?(B_STORE):(A_STORE);

    int stones_in_hole = new_state.holes[hole];
    int index;
    int i = 1;
    while(stones_in_hole){
        index = (hole + i)%14;
        if(index == op_store) {i++;continue;}

        stones_in_hole--;
        new_state.holes[hole]--;
        new_state.holes[index]++;
        i++;
    }
    if(index == my_store){//final piece lands in my store
        //turn stays
        new_state.turn = me;

        if(me == PLAYER_A) new_state.a_move_earned++;
        if(me == PLAYER_B) new_state.b_move_earned++;
    }
    else{
        //else turn flips
        new_state.turn = op;
    }

    if( me <= index && index <= (me+5) && new_state.holes[index] == 1 ){
        //last piece in my holes
        int opposite_hole = 12 - index;
        if(new_state.holes[opposite_hole]){
            new_state.holes[index]--;//takes the last stone
            new_state.holes[my_store]++;//put it in my store

            //put all the stone of opposite hole in my store
            new_state.holes[my_store] += new_state.holes[opposite_hole];

            if(me == PLAYER_A)
                new_state.a_captured += new_state.holes[opposite_hole];
            else if(me == PLAYER_B)
                new_state.b_captured += new_state.holes[opposite_hole];

            new_state.holes[opposite_hole] = 0;
        }
    }

    return new_state;
}


//heuristic calculation
int del_storage(state& a){
    //stones_in_my_storage – stones_in_opponents_storage
    return a.holes[A_STORE] - a.holes[B_STORE];
}

int del_stones(state& a){
    //(stones_on_my_side – stones_on_opponents_side)
    int a_stones = 0;
    for(int i = 0;i < 6;i++){
        a_stones += a.holes[PLAYER_A + i];
    }//counts total #stones in A's holes

    int b_stones = 0;
    for(int i = 0;i < 6;i++){
        b_stones += a.holes[PLAYER_B + i];
    }//counts total #stones in B's holes

    return (a_stones - b_stones);
}

int move_earned(state& a){
    //(additional_move_earned)
    // int me_move_earned = (a.turn == PLAYER_A)?(a.a_move_earned-a.b_move_earned):(a.a_move_earned-a.b_move_earned);
    // return (me_move_earned);

    return (a.a_move_earned - a.b_move_earned);
}

int captured(state& a){
    //(stones_captured)
    // int me_captured = (a.turn == PLAYER_A)?(a.a_captured-a.b_captured):(a.a_captured-a.b_captured);
    // return (me_captured);

    return (a.a_captured - a.b_captured);
}

//heuristic functions
int heuristic_1(state& a){
    //cout << "heuristic_1()" << endl;
    //stones_in_my_storage – stones_in_opponents_storage
    return del_storage(a);
}

int heuristic_2(state& a){
    /*
        W1 * (stones_in_my_storage – stones_in_opponents_storage)
      + W2 * (stones_on_my_side – stones_on_opponents_side)
    */
    //cout << "heuristic_2()" << endl;
    return W1*del_storage(a)
         + W2*del_stones(a);
}

int heuristic_3(state& a){
    /*
        W1 * (stones_in_my_storage – stones_in_opponents_storage)
      + W2 * (stones_on_my_side – stones_on_opponents_side)
      + W3 * (additional_move_earned)
    */
    //cout << "heuristic_3()" << endl;
    return W1*del_storage(a)
         + W2*del_stones(a)
         + W3*move_earned(a);
}

int heuristic_4(state& a){
    /*
        W1 * (stones_in_my_storage – stones_in_opponents_storage)
      + W2 * (stones_on_my_side – stones_on_opponents_side)
      + W3 * (additional_move_earned)
      + W4 * (stones_captured)
    */
    //cout << "heuristic_4()" << endl;
    return W1*del_storage(a)
         + W2*del_stones(a)
         + W3*move_earned(a)
         + W4*captured(a);
}
