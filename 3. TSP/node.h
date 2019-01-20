#ifndef NODE_H
#define NODE_H

#include <bits/stdc++.h>

using namespace std;

struct node{
    double x,y;
    int index;
    node();
    node(double _x,double _y,int index);
};

struct path{
    double cost;
    vector<node> nodes;
    double calculateCost();
    path& operator=(const path &);
    void print();
    bool operator<(const path &) const;
};

double dist(node ,node );

#endif
