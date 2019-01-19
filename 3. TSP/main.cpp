#include "node.h"

#define First_Improvement 1
#define Best_Improvement 2
#define INF 1000000000

using namespace std;

/*
 * input : nodes = set of nodes,
           visited = viited array,
           current = current node,
           k = number of top near nodes
 * return: next neighbour choosen from top k near neighbours
 */
int getNextNode(vector<node> nodes,bool visited[],int current,int k){
    int n = nodes.size();//# nodes

    vector<int> topNeighbours;
    bool added[n] = {};//already added in topNeighbours

    for(int i = 0;i < k;i++){
        int next;
        double min_cost = INF;

        for(int i = 0; i < n;i++){
            if(i == current || visited[i] || added[i])
                continue;

            double cost = dist(nodes[current],nodes[i]);
            if(cost < min_cost){
                min_cost = cost;
                next = i;
            }
        }

        added[next] = true;
        topNeighbours.push_back(next);
    }

    int random = abs(rand()) % k;//selects a random integer in [0,k]
    int next   = topNeighbours[random];//randomly selects a top neighbour as next
    return next;
}

/* Nearest Neighbour Heuristic
 * input : nodes = set of nodes,
           start = starting nodes,
           k = number of top near nodes
 * return: a path covering all nodes
 */
path nearestNeighbour(vector<node> nodes,int start,int k){
    int n = nodes.size();
    bool visited[n] = {};
    int count = 0;
    int current = start;
    path tour;

    while(count < n){
        visited[current] = true;
        tour.nodes.push_back(nodes[current]);
        count++;

        current = getNextNode(nodes,visited,current,k);
    }
    tour.nodes.push_back(nodes[start]);
    tour.calculateCost();
    return tour;
}

path two_opt_swap(path current,int i,int k){
    int n = k - i + 1;
    int limit = n/2;
    for(int j = 0;j < limit;j++){
        //cout << (i+j) << " " << (i+n-j-1) << endl;
        swap(current.nodes[i + j],current.nodes[i + n - j -1]);
    }
    return current;
}

path two_opt(path current){
    int n =  ( (int) current.nodes.size() )  - 1;
    double current_best = current.calculateCost();
    for(int i = 1;i < n - 1;i++){
        for (int k = i + 1; k < n; k++) {
            path new_path   = two_opt_swap(current,i,k);
            double new_cost = new_path.calculateCost();
            //cout << new_cost << endl;
            if(new_cost < current_best){
                current_best = new_cost;
                current      = new_path;
            }
        }
    }
    current.calculateCost();
    return current;
}

path improve(path tour,int improvement){
    if(improvement == First_Improvement)
        return two_opt(tour);

    int no_imprv_count = 0;
    double best_cost = tour.calculateCost();

    while(no_imprv_count < 10){
        tour = two_opt(tour);

        double new_cost = tour.calculateCost();
        double improvement_factor
               = abs(best_cost-new_cost)/best_cost*100;

        if(improvement_factor < 10.0)
            no_imprv_count++;
        else
            no_imprv_count = 0;

        best_cost = new_cost;
    }
    return tour;
}

int main(int argc, char const *argv[]) {
    srand (time(NULL));//seeds time for different rand() value 
    
    //freopen("burma14.tsp","r",stdin);
    //freopen("berlin52.tsp","r",stdin);
    //freopen("st70.tsp","r",stdin);
    freopen("pr76.tsp","r",stdin);

    int n;
    vector<node> nodes;

    cin >> n;
    for(int i = 0;i < n;i++){
        double x,y;
        cin >> x >> y;
        nodes.push_back(node(x,y,i + 1));
    }
    
    path p = nearestNeighbour(nodes,11,1);
    p.print();

    p = improve(p,Best_Improvement);
    p.print();
    
    return 0;
}
