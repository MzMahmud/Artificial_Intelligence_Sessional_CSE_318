#include "node.h"

#define FIRST 1
#define BEST 2
#define INF 1000000000
#define EPS 0.0000001

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
        swap(current.nodes[i + j],current.nodes[i + n - j -1]);
    }
    return current;
}

path two_opt(path current,int improvement){
    int n =  ( (int) current.nodes.size() )  - 1;
    double current_best = current.calculateCost();
    for(int i = 1;i < n - 1;i++){
        for (int k = i + 1; k < n; k++) {
            path new_path   = two_opt_swap(current,i,k);
            double new_cost = new_path.calculateCost();
            if(new_cost < current_best){
                current_best = new_cost;
                current      = new_path;

                if(improvement == FIRST){
                    current.calculateCost();
                    return current;
                }
            }
        }
    }
    current.calculateCost();
    return current;
}

path improve(path tour,int improvement){
    double best_cost   = INF;
    double diff = abs(tour.calculateCost() - best_cost);
    while(diff > EPS){//EPS -> epsilon = 0.0000001
        tour = two_opt(tour,improvement);
        diff = abs(tour.cost - best_cost);
        best_cost = tour.cost;
    }
    return tour;
}

int main(int argc, char const *argv[]) {
    srand (time(NULL));//seeds time for different rand() value

    //freopen("input/burma14.tsp","r",stdin);
    //freopen("input/berlin52.tsp","r",stdin);
    //freopen("input/st70.tsp","r",stdin);
    freopen("input/pr76.tsp","r",stdin);

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

    p = improve(p,BEST);
    p.print();

    return 0;
}
