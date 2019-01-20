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

path& path::operator=(const path &other){
    this->cost  = other.cost;
    this->nodes = other.nodes;
    return *this;
}
void path::print(){
    int n = this->nodes.size();
    cout << "Cost => " << this->cost << endl;
    for(int i = 0;i < n;i++){
        cout << this->nodes[i].index << (i==(n-1)?"\n":" -> ");
    }
}

bool path::operator<(const path &other) const{
    return this->cost < other.cost;
}

double dist(node a,node b){
    return sqrt( (a.x-b.x)*(a.x-b.x)
               + (a.y-b.y)*(a.y-b.y) );
}
