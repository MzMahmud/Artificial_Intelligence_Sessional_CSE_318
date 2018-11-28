#include<bits/stdc++.h>

using namespace std;

#define LEFT  0
#define RIGHT 1
#define UP    2
#define DOWN  3
#define HAMMING   0
#define MANHATTAN 1
#define LINEARCON 2

typedef vector< vector<int> > matrix;
typedef pair<int,int> pii;

///Class defination
class State{
public:
    matrix a;
    int n;
    pii zero;
    string path;
    int f,g,h;

    State(matrix _a,pii _zero);
    State& operator=(const State& other);
    bool canMove(pii offset);
    State move(pii offset,string moveName,int heuristic);
    string toString();
    bool operator<(const State& other)const;
    bool operator==(const State& other);
};

///state comparator. Needed for priority_queue
class StateComparator{
public:
    bool operator()(const State& a,const State& b)const;
};

///utility functions
ostream& operator<<(ostream& sout,const State& a);
pii getGoalPosition(int,int);
int hamming(State);
int manhattan(State);
int linearConflict(State);
State getGoal(int);
bool isSolvable(State);
