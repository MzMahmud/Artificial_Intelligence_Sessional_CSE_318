#include "state.h"

#define TIMEOUT 30
#define MAXEXPLORED 1000000

int expandedBFS = 1,exploredBFS,pathLengthBFS;
double timeBFS;
bool bfs(State start,State goal){
    queue<State> q;
    map<State,bool> visited;

    q.push(start);

    clock_t startTime = clock();

    expandedBFS = 0,exploredBFS = 1;
    while(!q.empty()){
        State u;

        u = q.front();q.pop();
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
			pathLengthBFS = u.pathLength;
            cout << "GOAL  : " + u.toString() <<endl;
            return true;
        }

        for(int man = 0;man <= u.capacity;man++){
            for(int can = 0;can <= u.capacity;can++){
                if(    ( (man+can) <= u.capacity )//not more than capacity
                    && (man||can) //both not zero
                    && ( man == 0 || man > 0 && man >= can  )//man not outnumbered in boat
                  ){

                    State v = u.move(man,can);

                    if(v.isValid() && !visited[v]){
                        //cout << v << endl;
						visited[v] = true;
						exploredBFS++;
                        q.push(v);
                    }
                }
            }
        }
    }

    cout << "-----------Full Graph Searched! NO SOLUTION-----------" << endl;
    return false;
}

int expandedDFS = 1,exploredDFS,pathLengthDFS;
double timeDFS;
bool dfs(State start,State goal){
    stack<State> s;

	map<State,bool> visited;

    s.push(start);

    clock_t startTime = clock();

    expandedDFS = 0,exploredDFS = 1;
    while(!s.empty()){
        State u;
        u = s.top();s.pop();

		visited[u] = true;

		expandedDFS++;
        //cout << ">> " << u << endl;

        clock_t endTime = clock();
        double timePassed = (double)(endTime - startTime)/CLOCKS_PER_SEC;
        if(timePassed > TIMEOUT){
            cout << "----------TIME OUT----------" << endl;
            return false;
        }

        if(exploredDFS > MAXEXPLORED){
            cout << "----------TOO MANY NODES EXPLORED!----------" << endl;
            return false;
        }

        if(u == goal){
            cout << "-----SOLUTION-----" << endl;
            cout << u.path << endl;
            pathLengthDFS = u.pathLength;
			cout << "GOAL  : " + u.toString() <<endl;
            return true;
        }



        for(int man = 0;man <= u.capacity;man++){
            for(int can = 0;can <= u.capacity;can++){

                if(    ( (man+can) <= u.capacity )//not more than capacity
                    && (man||can) //both not zero
                    && ( man == 0 || man > 0 && man >= can  )//man not outnumbered in boat
                  ){

                    State v = u.move(man,can);

                    if(v.isValid() && !visited[v]){
                        //cout << v << endl;
                        visited[v] = true;
						exploredDFS++;
                        s.push(v);
                    }
                }
            }
        }
    }

    cout << "-----------Full Graph Searched! NO SOLUTION-----------" << endl;
    return false;
}


void run(){
    printf("\n\nThe Missionaries and Cannibals Problem\n\n");

    int man,can,cap;
    printf("Enter number of Missionaries : ");
    scanf("%d",&man);

    printf("Enter number of Cannibals : ");
    scanf("%d",&can);

    printf("Enter Capacity of boat: ");
    scanf("%d",&cap);

    State start(man,can,0,0,LEFT);
    start.setCapacity(cap);
    State goal(0,0,man,can,RIGHT);
    goal.setCapacity(cap);

    cout << endl << "Start: " << start ;
    cout << endl << "Goal : " << goal  << endl;

    printf("\n****Running BFS****\n");
    timeBFS = (double) clock();
    bfs(start,goal);
    timeBFS = ((double) clock() - timeBFS)/CLOCKS_PER_SEC;


    printf("\n\n****Running DFS****\n\n");
    timeDFS = (double) clock();
    dfs(start,goal);
    timeDFS = ((double) clock() - timeDFS)/CLOCKS_PER_SEC;

    cout << endl << endl << "Start: " << start ;
    cout << endl << "Goal : " << goal ;
    printf("\n\n************Comparison************\n");
    printf("%10s%10s%10s\n"," ","BFS","DFS");
    printf("Path Len : %10d %10d\n",pathLengthBFS,pathLengthDFS);
	printf("Time     : %9lfs %9lfs\n",timeBFS,timeDFS);
    printf("Expanded : %10d %10d\n",expandedBFS,expandedDFS);
    printf("Explored : %10d %10d\n",exploredBFS,exploredDFS);
}

int main(){
    //freopen("out.txt","w",stdout);
    char ch;
    do{

        run();
        printf("\nEnter A/a to run again!\n");
        cin >> ch;

    }while(ch == 'A' || ch == 'a');

    return 0;
}
