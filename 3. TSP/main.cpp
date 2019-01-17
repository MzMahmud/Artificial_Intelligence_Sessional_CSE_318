#include "node.h"

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


int main(int argc, char const *argv[]) {
    
    freopen("burma14.tsp","r",stdin);

    int n;
    vector<node> nodes;

    cin >> n;
    for(int i = 0;i < n;i++){
        double x,y;
        cin >> x >> y;
        nodes.push_back(node(x,y,i));
    }
    for(int start = 0;start < n;start++){
        path p = nearestNeighbour(nodes,start,1);
        cout << p.nodes[0].index << " " <<  p.cost << endl;
        for(int i = 0;i < p.nodes.size();i++){
            cout << p.nodes[i].index+1 << " ";
        }
        cout << endl;
    }

    return 0;
}
