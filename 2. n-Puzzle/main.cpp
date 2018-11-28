#include "state.h"

#define TIMEOUT 30.0
#define MAXEXPANDED 1000000

pair<pii,string> moves[4] = {
                                make_pair( make_pair( 0, 1), string("LEFT") ),
                                make_pair( make_pair( 0,-1), string("RIGHT")),
                                make_pair( make_pair( 1, 0), string("UP")   ),
                                make_pair( make_pair(-1, 0), string("DOWN") )
                            };

//perfonmance measurement
double runTime[3];
int expanded[3];
int explored[3];
int cost[3];

bool A_star(State start,State goal,int heuristic){

    priority_queue<State,vector<State>,StateComparator>  pq;//Open List
    map<State,bool> visited;//Close List

    pq.push(start);

    explored[heuristic] = 0;
    expanded[heuristic] = 0;
    clock_t startTime = clock();

    while(!pq.empty()){
        State u = pq.top();pq.pop();
		visited[u] = true;

        if(u == goal){
            cout << "----------SOLUTION----------" << endl;
            cout << u.path;
			cout << "REACHED GOAL  : \n" + u.toString() <<endl;
			cout << "Path Length   : " << u.g << endl;
            cost[heuristic] = u.g;
            return true;
        }

        //Timeout
        clock_t endTime = clock();
        double timePassed = (double)(endTime - startTime)/CLOCKS_PER_SEC;
        if(timePassed > TIMEOUT){
            cout << "----------TIME OUT----------" << endl;
            return false;
        }

        //to much node expanded
        if(expanded[heuristic] > MAXEXPANDED){
            cout << "----------TOO MANY NODES EXPLORED!----------" << endl;
            return false;
        }

        //one more node expanded
        expanded[heuristic]++;
        for(int i = 0;i < 4;i++){
            if( u.canMove(moves[i].first) ){

                State v = u.move( moves[i].first,//move description
                                  moves[i].second,//move name
                                  heuristic//heuristic function
                                 );

                if(!visited[v]){
                    //one more node explored
                    explored[heuristic]++;
                    //visited[v] = true;
                    pq.push(v);
                }
            }
        }

    }

    cout << "-----------Full Graph Searched! NO SOLUTION-----------" << endl;
    return false;
}

void run(){
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

    if(!isSolvable(start)){
        cout << start << "--------NOT Solvable--------" << endl;
        return;
    }

    //runs with HAMMING(0),MANHATTAN(1),LINEARCON(2)
    bool ret[3];
    for(int heu = HAMMING;heu <= LINEARCON;heu++){
             if(heu == HAMMING)     printf("----With Hamming Distance------\n");
        else if(heu == MANHATTAN)   printf("----With Manhattan Distance----\n");
        else if(heu == LINEARCON)   printf("----With Linear Conflict-------\n");

        runTime[heu] = (double) clock();
        ret[heu] = A_star(start,getGoal(n),heu);
        runTime[heu] = ((double) clock() - runTime[heu])/CLOCKS_PER_SEC;
    }

    cout<<endl;
    printf("*************************Comparison*************************\n");
    printf("%15s%10s%10s%10s%10s\n"," ","Cost","Time","Expanded","Explored");
    if(ret[HAMMING])
    printf("%15s%10d%10lf%10d%10d\n","Hamming",cost[HAMMING],runTime[HAMMING],expanded[HAMMING],explored[HAMMING]);
    if(ret[MANHATTAN])
    printf("%15s%10d%10lf%10d%10d\n","Manhattan",cost[MANHATTAN],runTime[MANHATTAN],expanded[MANHATTAN],explored[MANHATTAN]);
    if(ret[LINEARCON])
    printf("%15s%10d%10lf%10d%10d\n","Linear Conflict",cost[LINEARCON],runTime[LINEARCON],expanded[LINEARCON],explored[LINEARCON]);
    printf("************************************************************\n");

}

int main(){
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);

    run();

    return 0;
}
