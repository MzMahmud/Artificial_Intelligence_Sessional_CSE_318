#ifndef STATE_H

#include <bits/stdc++.h>

using namespace std;

#define STATE_H
#define LEFT 0
#define RIGHT 1

class State{
public:
    int man[2],can[2],boat,capacity;
    string path;
    int pathLength;

    State();
    State(int lm,int lc,int rm,int rc,int b);
    void setCapacity(int c);
    bool isValid();
    State operator=(State a);
    State move(int _man,int _can);
    friend ostream& operator<<(ostream& sout,State a);
    bool operator==(const State& a);
    string toString();
};

bool operator<(const State& a,const State& b);
ostream& operator<<(ostream& sout,State a);
char* itoa(int a,char *s);

#endif // STATE_H
