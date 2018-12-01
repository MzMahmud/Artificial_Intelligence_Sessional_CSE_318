#include "state.h"

#define LEFT 0
#define RIGHT 1

///class implementation
State::State(){
    man[LEFT] = 3;can[LEFT] = 3;
    man[RIGHT] = 0;can[RIGHT] = 0;
    boat = LEFT;
    path = "";
    pathLength = 0;
}
State::State(int lm,int lc,int rm,int rc,int b){
    man[LEFT] = lm;can[LEFT] = lc;
    man[RIGHT] = rm;can[RIGHT] = rc;
    boat = b;
    path = "";
    capacity = 2;
    pathLength = 0;
}
void State::setCapacity(int c){
    capacity = c;
}
bool State::isValid(){
    if(man[LEFT] < 0 || can[LEFT] < 0 || man[RIGHT] < 0 || can[RIGHT] < 0)
        return false;

    if(man[LEFT] > 0 && can[LEFT] > man[LEFT])
        return false;

    if(man[RIGHT] > 0 && can[RIGHT] > man[RIGHT])
        return false;

    return true;
}
State State::operator=(State a){

    this->man[LEFT] = a.man[LEFT];this->man[RIGHT] = a.man[RIGHT];
    this->can[LEFT] = a.can[LEFT];this->can[RIGHT] = a.can[RIGHT];
    this->boat = a.boat;
    this->path = a.path;
    this->capacity = a.capacity;
    this->pathLength = a.pathLength;

    return *this;
}
State State::move(int _man,int _can){
    State a = *this;

    string passenger = "";
    for(int i = 1;i <= _man;i++){
        passenger.push_back('M');
    }
    for(int i = 1;i <= _can;i++){
        passenger.push_back('C');
    }
    if(a.boat == LEFT){
        passenger = "(" + passenger + ")....\n\n";
    }else{
        passenger = "....(" + passenger + ")\n\n";
    }

    a.path = a.path + "State : " + a.toString() + "\nAction: " + passenger;
    a.pathLength++;

    a.man[a.boat] -= _man;
    a.can[a.boat] -= _can;

    a.boat = !a.boat;

    a.man[a.boat] += _man;
    a.can[a.boat] += _can;

    return a;
}
bool State::operator==(const State& a){
    return man[0] == a.man[0] && man[1] == a.man[1]
        && can[0] == a.can[0] && can[1] == a.can[1]
        && boat == a.boat;
}
string State::toString(){
    string s;
    char c[100];
    s = string("(M: ") + string(itoa(man[LEFT],c))
      + string(",C: ") + string(itoa(can[LEFT],c)) + string(") ");
    if(boat == LEFT) s += string("B.... ");
    else               s += "....B ";
    s += string("(M: ") + string(itoa(man[RIGHT],c))
       + string(",C: ") + string(itoa(can[RIGHT],c)) + string(")");

    return s;
}

///utility functions
char* itoa(int a,char *s){
	sprintf(s,"%d",a);
	return s;
}
bool operator<(const State& a,const State& b){
	if(a.man[0] == b.man[0]){
		if(a.can[0] == b.can[0]){
			return a.boat < b.boat;
		}
		return a.can[0] < b.can[0];
	}
	return a.man[0] < b.man[0];
}
ostream& operator<<(ostream& sout,State a){
    sout << "(M: " << a.man[LEFT] << ",C: " << a.can[LEFT] << ") ";
    if(a.boat == LEFT) sout << "B.... ";
    else               sout << "....B ";
    sout << "(M: " << a.man[RIGHT] << ",C: " << a.can[RIGHT] << ")";
    return sout;
}
