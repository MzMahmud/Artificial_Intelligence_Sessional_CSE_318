#include "node.h"

node::node(){
    this->x = .0;
    this->y = .0;
}

node::node(double _x,double _y,int _index){
    this->x = _x;
    this->y = _y;
    this->index = _index;
}


double path::calculateCost(){
    this->cost = 0;
    for(int i = 1;i < nodes.size();i++){
        this->cost += dist(nodes[i-1],nodes[i]);
    }
    return this->cost;
}

double dist(node a,node b){
    return sqrt( (a.x-b.x)*(a.x-b.x)
               + (a.y-b.y)*(a.y-b.y) );
}
