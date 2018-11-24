#include <bits/stdc++.h>
#define TIMEOUT 30
#define MAXEXPLORED 1000000

using namespace std;

#include "State.cpp"

int expandedBFS = 1,exploredBFS;
double timeBFS;

bool bfs(State start,State goal){
    queue<State> q;
    map<State,bool> visited;

    q.push(start);

    clock_t startTime = clock();

    expandedBFS = 0,exploredBFS = 1;
    while(!q.empty()){
        State u = q.front();q.pop();
		visited[u] = true;

        expandedBFS++;
        //cout << ">> " << u << endl;

        clock_t endTime = clock();
        double timePassed = (double)(endTime - startTime)/CLOCKS_PER_SEC;
        if(timePassed > TIMEOUT){
            cout << "----------TIME OUT----------" << endl;
            return false;
        }

        if(exploredBFS > MAXEXPLORED){
            cout << "----------TOO MANY NODES EXPLORED!----------" << endl;
            return false;
        }

        if(u == goal){
            cout << "-----SOLUTION-----" << endl;
            cout << u.path << endl;
			cout << "GOAL  : \n" + u.toString() <<endl;
            return true;
        }

        for(int i = 0;i < 4;i++){
            if( u.canMove(moves[i].first) ){
                State v = u.move(moves[i].first,moves[i].second);
                if(!visited[v]){
                    visited[v] = true;
                    exploredBFS++;
                    q.push(v);
                }
            }
        }
    }

    cout << "-----------Full Graph Searched! NO SOLUTION-----------" << endl;
    return false;
}

int main(){
    freopen("in.txt","r",stdin);

    int k,x;
    cin >> k;
    int n = sqrt(k+1.0);
    matrix a;
    vector<int> b;
    pii zero;
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++){
            cin >> x;
            if(!x) zero = make_pair(i,j);
            b.push_back(x);
        }
        a.push_back(b);
        b.clear();
    }

    State start(a,zero);
    cout << boolalpha << "solvable = " << start.isSolvable() << endl;
    bfs(start,getGoal(n));

    return 0;
}
