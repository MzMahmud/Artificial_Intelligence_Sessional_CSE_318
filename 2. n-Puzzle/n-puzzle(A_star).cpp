#include <bits/stdc++.h>

using namespace std;

#include "State.cpp"

bool A_star(State start,State goal,int heuristic){

    priority_queue<State,vector<State>,StateComparator>  pq;//Open List
    map<State,bool> visited;//Close List

    pq.push(start);

    while(!pq.empty()){
        State u = pq.top();pq.pop();
		visited[u] = true;

        if(u == goal){
            cout << "-----SOLUTION-----" << endl;
            cout << u.path;
			cout << "REACHED GOAL  : \n" + u.toString() <<endl;
			cout << "Path Length   : " << u.g << endl;
            return true;
        }

        for(int i = 0;i < 4;i++){
            if( u.canMove(moves[i].first) ){

                State v = u.move( moves[i].first,//move description
                                  moves[i].second,//move name
                                  heuristic//heuristic function
                                 );

                if(!visited[v]){
                    //visited[v] = true;
                    pq.push(v);
                }
            }
        }
    }

    cout << "-----------Full Graph Searched! NO SOLUTION-----------" << endl;
    return false;
}

int main(){
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);

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

    if(isSolvable(start))
        A_star(start,getGoal(n),MANHATTAN);
    else
        cout << "Not solvabable" << endl;

    return 0;
}

