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
    k = (int) topNeighbours.size();
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


path savings_heuristic(vector<node> nodes,int depo,int k){
    srand (time(NULL));

    int n = nodes.size();
    double savings[n][n] = {};
    pair<int,int> max_index;
    double max_saving = -1;
    //calculate savings
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++){
            if(i == j || i == depo || j == depo){
                continue;
            }
            savings[i][j] = dist(nodes[depo],nodes[i])
                          + dist(nodes[depo],nodes[j])
                          - dist(nodes[i]   ,nodes[j]);
            if(savings[i][j] > max_saving){
                max_saving = savings[i][j];
                max_index = make_pair(i,j);
            }
        }
    }

    deque<node> cycle;//adds nodes to cycle
    bool added[n] = {};//nodes already added

    //adds max savings to the cycle
    cycle.push_front(nodes[max_index.first]);
    cycle.push_back (nodes[max_index.second]);
    added[max_index.first] = added[max_index.second] = true;
    int times = 3;
    while( (int) cycle.size() < (n-1) ){//n-1 nodes have to be added to cycle
        //choose next nodes to be added to cycle
        int u = cycle.front().index - 1;//-1 because + 1 was added in index
        int u_next = -420;
        //selects next of u
        vector<int> u_neighbours;
        bool u_pushed[n] = {};
        for(int j = 0;j < k;j++){//takes to k neigbhbours

            double u_best_neighbour = -1;
            int    u_best_neighbour_index = -1;
            for(int i = 0;i < n;i++){
                if(i == u || i == depo || added[i] || u_pushed[i] ){
                    continue;
                }
                if(savings[u][i] > u_best_neighbour){
                    u_best_neighbour = savings[u][i];
                    u_best_neighbour_index  = i;
                }
            }
            if(u_best_neighbour_index >= 0){
                u_neighbours.push_back(u_best_neighbour_index);
                u_pushed[u_best_neighbour_index] = true;
            }
        }
        int u_total = (int) u_neighbours.size();
        if(u_total > 0){
            int u_random = abs(rand()) % u_total;
            u_next =  u_neighbours[u_random];
        }else{
            u_next = -1;
        }

        //***************************FOR v*******************************
        int v = cycle.back().index - 1;//-1 because + 1 was added in index
        int v_next = -420;
        vector<int> v_neighbours;
        bool v_pushed[n] = {};

        for(int j = 0;j < k;j++){//takes to k neigbhbours

            double v_best_neighbour = -1;
            int    v_best_neighbour_index = -1;
            for(int i = 0;i < n;i++){
                if(i == v || i == depo || added[i] || v_pushed[i] ){
                    continue;
                }
                if(savings[v][i] > v_best_neighbour){
                    v_best_neighbour = savings[v][i];
                    v_best_neighbour_index  = i;
                }
            }

            if(v_best_neighbour_index >= 0){
                v_neighbours.push_back(v_best_neighbour_index);
                v_pushed[v_best_neighbour_index] = true;
            }
        }

        int v_total = (int) v_neighbours.size();
        if(v_total > 0){
            int v_random = abs(rand()) % v_total;
            v_next =  u_neighbours[v_random];
        }else{
            v_next = -1;
        }
        //cout << "v_next  " << v_next << endl;

        //**********************************************************************

        //adds node to cycle
        if(u_next >= 0 && v_next >= 0){
            if(u_next == v_next){
                if(savings[u][u_next] > savings[v][v_next]){
                    cycle.push_front(nodes[u_next]);
                    added[u_next] = true;
                }else{
                    cycle.push_back(nodes[v_next]);
                    added[v_next] = true;
                }
            }else{
                cycle.push_front(nodes[u_next]);
                added[u_next] = true;

                cycle.push_back(nodes[v_next]);
                added[v_next] = true;
            }
        }else if(u_next < 0 && v_next > 0){

            cycle.push_back(nodes[v_next]);
            added[v_next] = true;

        }else if(u_next > 0 && v_next < 0){

            cycle.push_front(nodes[u_next]);
            added[u_next] = true;

        }if(u_next < 0 && v_next < 0){
            break;
        }

    }

    cycle.push_front(nodes[depo]);
    cycle.push_back (nodes[depo]);

    path tour;
    deque<node>::iterator it;
    for(it = cycle.begin();it != cycle.end();it++){
        tour.nodes.push_back(*it);
    }

    tour.calculateCost();
    return tour;
}

vector< vector<double> > task_1;
vector< vector<double> > task_2;
vector< vector<double> > task_best;
vector< vector<double> > task_first;
vector< vector<double> > task_final;

void run(const char *file_name){
    ifstream fin(file_name);
    int n;
    vector<node> nodes;

    fin >> n;
    for(int i = 0;i < n;i++){
        double x,y;
        fin >> x >> y;
        nodes.push_back(node(x,y,i + 1));
    }

    ///TODO: pick k = 5 statring nodes
    int k = 5;
    vector<int> starts;
    map<int,bool> taken;
    while( (int)starts.size() < k){
        int start = abs(rand())%n;
        if(!taken[start]){
            taken[start] = true;
            starts.push_back(start);
        }
    }
    vector<path> nnh_path,sh_path;
    for(int i = 0;i < starts.size();i++){
        //cout << starts[i] << " ";
        nnh_path.push_back(nearestNeighbour(nodes,starts[i],1));
         sh_path.push_back(savings_heuristic(nodes,starts[i],1));
    }

    sort(nnh_path.begin(),nnh_path.end());
    // for(int i = 0;i < nnh_path.size();i++){
    //     nnh_path[i].print();
    // }
    // cout << endl;

    sort(sh_path.begin(),sh_path.end());
    // for(int i = 0;i < sh_path.size();i++){
    //     sh_path[i].print();
    // }
    // cout << endl;
    double nnh_avg = 0;
    double  sh_avg = 0;

    //AVERAGE CASE
    vector<double> stat;
    for(int i = 0;i < k;i++){
        nnh_avg += nnh_path[i].cost;
         sh_avg +=  sh_path[i].cost;
    }
    stat.push_back(nnh_avg/k);
    stat.push_back( sh_avg/k);

    //BEST CASE
    stat.push_back(nnh_path[0].cost);
    stat.push_back( sh_path[0].cost);

    //WORST CASE
    stat.push_back(nnh_path[k-1].cost);
    stat.push_back( sh_path[k-1].cost);

    task_1.push_back(stat);

    //TODO: Task 2
    int best_start = nnh_path[0].nodes[0].index - 1;
    int run_count = 10;
    int top_count = 5;
    vector<path> nnh_rand,sh_rand;
    for(int i = 0; i < run_count;i++){
        nnh_rand.push_back(nearestNeighbour(nodes,best_start,top_count));
         sh_rand.push_back(savings_heuristic(nodes,best_start,top_count));
    }
    sort(nnh_rand.begin(),nnh_rand.end());
    sort( sh_rand.begin(), sh_rand.end());
    //AVERAGE CASE
    vector<double> stat_rand;
    nnh_avg = sh_avg = 0;
    for(int i = 0;i < k;i++){
        nnh_avg += nnh_rand[i].cost;
         sh_avg +=  sh_rand[i].cost;
    }
    stat_rand.push_back(nnh_avg/run_count);
    stat_rand.push_back( sh_avg/run_count);

    //BEST CASE
    stat_rand.push_back(nnh_rand[0].cost);
    stat_rand.push_back( sh_rand[0].cost);

    //WORST CASE
    stat_rand.push_back(nnh_rand[run_count-1].cost);
    stat_rand.push_back( sh_rand[run_count-1].cost);

    task_2.push_back(stat_rand);

    //TODO: Task 3
    vector<path> nnh_improve,sh_improve;
    nnh_improve.push_back(nnh_path[0]);
    nnh_improve.push_back(nnh_rand[0]);
    nnh_improve.push_back(nnh_rand[1]);
    nnh_improve.push_back(nnh_rand[2]);

    sh_improve.push_back(sh_path[0]);
    sh_improve.push_back(sh_rand[0]);
    sh_improve.push_back(sh_rand[1]);
    sh_improve.push_back(sh_rand[2]);

    vector<path> nnh_best,sh_best;
    vector<path> nnh_first,sh_first;
    int to_improve_count = 4;
    for(int i = 0; i < to_improve_count;i++){
        nnh_best.push_back(improve(nnh_improve[i],BEST));
         sh_best.push_back(improve( sh_improve[i],BEST));

        nnh_first.push_back(improve(nnh_improve[i],FIRST));
         sh_first.push_back(improve( sh_improve[i],FIRST));
    }
    sort(nnh_best.begin(),nnh_best.end());
    sort( sh_best.begin(), sh_best.end());

    sort(nnh_first.begin(),nnh_first.end());
    sort( sh_first.begin(), sh_first.end());

    //AVERAGE CASE
    vector<double> stat_best;
    vector<double> stat_first;

    double nnh_best_avg = 0,sh_best_avg = 0;
    double nnh_first_avg = 0,sh_first_avg = 0;

    for(int i = 0;i < to_improve_count;i++){
        nnh_best_avg += nnh_best[i].cost;
         sh_best_avg +=  sh_best[i].cost;

        nnh_first_avg += nnh_first[i].cost;
         sh_first_avg +=  sh_first[i].cost;
    }

    stat_best.push_back(nnh_best_avg/to_improve_count);
    stat_best.push_back( sh_best_avg/to_improve_count);

    stat_first.push_back(nnh_first_avg/to_improve_count);
    stat_first.push_back( sh_first_avg/to_improve_count);

    //BEST CASE
    stat_best.push_back(nnh_best[0].cost);
    stat_best.push_back( sh_best[0].cost);

    stat_first.push_back(nnh_first[0].cost);
    stat_first.push_back( sh_first[0].cost);

    //WORST CASE
    stat_best.push_back(nnh_best[to_improve_count-1].cost);
    stat_best.push_back( sh_best[to_improve_count-1].cost);

    stat_first.push_back(nnh_first[to_improve_count-1].cost);
    stat_first.push_back( sh_first[to_improve_count-1].cost);

    task_best.push_back(stat_best);
    task_first.push_back(stat_first);
}

int main(int argc, char const *argv[]) {
    srand (time(NULL));//seeds time for different rand() value

    //freopen("input/berlin52.tsp","r",stdin);
    //freopen("input/st70.tsp","r",stdin);
    //freopen("input/pr76.tsp","r",stdin);

    //TODO: Taks 1
    string file_name[3] = { "pr76","berlin52","st70"};
    for(int i = 0; i < 3;i++){
        string name = string("input/") + file_name[i] + string(".tsp");
        run(name.c_str());
    }

    printf("\t\tSIMPLE GREEDY RESULT\n");
    printf("%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\n",
           "INPUT","NNH AVG","SH AVG","NNH BEST","SH BEST","NNH WORST","SH WORST");
    for(int i = 0; i < task_1.size();i++){
        printf("%10s\t",file_name[i].c_str());
        for(int j = 0;j < task_1[i].size();j++){
            printf("%10.6lf\t",task_1[i][j]);
        }
        cout << endl;
    }

    printf("\n\n\t\tGREEDY RANDOM RESULT\n");
    printf("%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\n",
           "INPUT","NNH AVG","SH AVG","NNH BEST","SH BEST","NNH WORST","SH WORST");
    for(int i = 0; i < task_2.size();i++){
        printf("%10s\t",file_name[i].c_str());
        for(int j = 0;j < task_2[i].size();j++){
            printf("%10.6lf\t",task_2[i][j]);
        }
        cout << endl;
    }

    printf("\n\n\t\tThe 2-opt results for best improvement\n");
    printf("%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\n",
           "INPUT","NNH AVG","SH AVG","NNH BEST","SH BEST","NNH WORST","SH WORST");
    for(int i = 0; i < task_best.size();i++){
        printf("%10s\t",file_name[i].c_str());
        for(int j = 0;j < task_best[i].size();j++){
            printf("%10.6lf\t",task_best[i][j]);
        }
        cout << endl;
    }

    printf("\n\n\t\tThe 2-opt results for first improvement\n");
    printf("%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\n",
           "INPUT","NNH AVG","SH AVG","NNH BEST","SH BEST","NNH WORST","SH WORST");
    for(int i = 0; i < task_first.size();i++){
        printf("%10s\t",file_name[i].c_str());
        for(int j = 0;j < task_first[i].size();j++){
            printf("%10.6lf\t",task_first[i][j]);
        }
        cout << endl;
    }

    //TODO: optimal with best and first
    vector<double> stat_final;
    //first input file
    stat_final.push_back(108159);stat_final.push_back(100);
    stat_final.push_back(task_best[0][2]/108159.0*100);
    stat_final.push_back(task_best[0][3]/108159.0*100);
    stat_final.push_back(task_first[0][2]/108159.0*100);
    stat_final.push_back(task_first[0][3]/108159.0*100);

    task_final.push_back(stat_final);
    stat_final.clear();

    //second input file
    stat_final.push_back(7542);stat_final.push_back(100);
    stat_final.push_back(task_best[1][2]/7542.0*100);
    stat_final.push_back(task_best[1][3]/7542.0*100);
    stat_final.push_back(task_first[1][2]/7542.0*100);
    stat_final.push_back(task_first[1][3]/7542.0*100);

    task_final.push_back(stat_final);
    stat_final.clear();

    //third input file
    stat_final.push_back(675);stat_final.push_back(100);
    stat_final.push_back(task_best[2][2]/675.0*100);
    stat_final.push_back(task_best[2][3]/675.0*100);
    stat_final.push_back(task_first[2][2]/675.0*100);
    stat_final.push_back(task_first[2][3]/675.0*100);

    task_final.push_back(stat_final);
    stat_final.clear();

    printf("\n\n\t\tPerformance Comparison\n");
    printf("%10s\t%10s\t%10s\t%10s\t%10s\t%10s\t%10s\n",
           "INPUT","ACTUAL","%%","NNH BEST %%","SH BEST %%","NNH FISRT %%","SH FISRT %%");
    for(int i = 0; i < task_final.size();i++){
        printf("%10s\t",file_name[i].c_str());
        for(int j = 0;j < task_final[i].size();j++){
            printf("%10.3lf\t",task_final[i][j]);
        }
        cout << endl;
    }



    return 0;
}
