#define LEFT  0
#define RIGHT 1
#define UP    2
#define DOWN  3
#define HAMMING   0
#define MANHATTAN 1
#define LINEARCON 2

typedef vector< vector<int> > matrix;
typedef pair<int,int> pii;

///all possible moves
pair<pii,string> moves[4] = {
                                make_pair( make_pair( 0, 1), string("LEFT") ),
                                make_pair( make_pair( 0,-1), string("RIGHT")),
                                make_pair( make_pair( 1, 0), string("UP")   ),
                                make_pair( make_pair(-1, 0), string("DOWN") )
                            };
struct State;
pii getGoalPosition(int,int);
int hamming(State);
int manhattan(State);
int linearConflict(State);


///Class implementation
class State{
public:
    matrix a;
    int n;
    pii zero;
    string path;
    int f,g,h;

    State(matrix _a,pii _zero){
        this->a    = _a;this->zero = _zero;
        this->n    = a.size();this->path = "";
        this->f    = 0;this->g    = 0;this->h    = 0;
    }

    State& operator=(const State& other){
        this->a    = other.a;this->n    = other.n;
        this->zero = other.zero;this->path = other.path;
        this->f    = other.f;this->g    = other.g;
        this->h    = other.h;
        return *this;
    }

    bool canMove(pii offset){
        int i = zero.first  + offset.first;
        int j = zero.second + offset.second;
        //if i crosses the grid its invalid move
        return !(i < 0 || j < 0 || i >= n || j >= n);
    }

    State move(pii offset,string moveName,int heuristic){
        State ret = *this;
        //performs the move
        int prev_i,prev_j,new_i,new_j;
        prev_i = ret.zero.first;
        prev_j = ret.zero.second;
         new_i = prev_i + offset.first;
         new_j = prev_j + offset.second;
        swap(ret.a[prev_i][prev_j],ret.a[new_i][new_j]);
        //updates the new position of zero
        ret.zero = make_pair(new_i,new_j);

        //builds path
        char ch[100];
        sprintf(ch,"move %d ",ret.a[prev_i][prev_j]);
        ret.path = this->path + "State  :\n" + toString()
                              + "Action : "  + string(ch) + moveName + "\n\n";

        //updates estimated cost f(n) = g(n) + h(n)
        ret.g = this->g + 1;
        int heu;
             if(heuristic == HAMMING)   heu = hamming(ret);
        else if(heuristic == MANHATTAN) heu = manhattan(ret);
        else if(heuristic == LINEARCON) heu = linearConflict(ret);

        ret.h = heu;
        ret.f = ret.g + ret.h;

        return ret;
    }

    string toString(){
        char s[10000] = "",t[1000];
        for(int i = 0;i < n;i++){
            for(int j = 0;j < n;j++){
                sprintf(t,"%d ",a[i][j]);
                strcat(s,t);
            }
            sprintf(t,"\n");
            strcat(s,t);
        }
        return string(s);
    }

    bool operator<(const State& other) const {
        for(int i = 0;i < n;i++)
            for(int j = 0;j < n;j++)
                if(this->a[i][j] != other.a[i][j])
                    return this->a[i][j] < other.a[i][j];

        return false;
    }

    bool operator==(const State& other){
        for(int i = 0;i < n;i++)
            for(int j = 0;j < n;j++)
                if(this->a[i][j] != other.a[i][j])
                    return false;

        return true;
    }
};


///state comparator. Needed for priority_queue

class StateComparator{
public:
    bool operator()(const State& a,const State& b)const{
        // '>' for min heap
        // '<' for max heap
        return a.f > b.f;
    }
};


///utility functions
ostream& operator<<(ostream& sout,const State& a){
    sout << "----Board----" << endl;
    for(int i = 0;i < a.n;i++){
        for(int j = 0;j < a.n;j++){
            sout << a.a[i][j] << " ";
        }
        sout << endl;
    }
    sout << "n = " << a.n << endl;
    sout << "zero = (" << a.zero.first << "," << a.zero.second << ")" << endl;

    return sout;
}

State getGoal(int n){
    matrix a;
    vector<int> b;
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n;j++){
            int val = i*n + j + 1;
            b.push_back(val);
        }
        a.push_back(b);
        b.clear();
    }
    a[n-1][n-1] = 0;
    pii zero = make_pair(n-1,n-1);
    State goal(a,zero);
    return goal;
}

bool isSolvable(State state){
    vector<int> arr;
    int inversions = 0;
    int blank_row = state.zero.first;

    for(int i = 0;i < state.n;i++)
        for(int j = 0;j < state.n;j++)
            if(state.a[i][j]) arr.push_back(state.a[i][j]);

    for(int i = 0;i < (int)arr.size() - 1;i++){
        for(int j = i + 1;j < arr.size();j++){
            if(arr[i] > arr [j]){
                inversions++;
                //printf("(%d,%d)\n",arr[i],arr[j]);
            }
        }
    }
    //printf("blank row %d\ninversions %d\n",blank_row,inversions);

    if(state.n % 2 == 1){
        //number of inversions must be EVEN to be solvable
        return (bool) !(inversions % 2);
    }else{
        //number of inversions + blank row must be ODD to be solvable
        return (bool) ( (inversions + blank_row) % 2 );
    }
}

pii getGoalPosition(int a,int n){
    if(a == 0)
        return make_pair(n-1,n-1);

    return make_pair((a-1)/n,(a-1)%n);
}


///heuristic functions

int hamming(State state){
    int hamm = 0;
    for(int i = 0;i < state.n;i++){
        for(int j = 0;j < state.n;j++){
            if(state.a[i][j]){
                pii current = make_pair(i,j);
                pii goal    = getGoalPosition(state.a[i][j],state.n);
                hamm = hamm + (int) (current != goal);
            }
        }
    }
    return hamm;
}

int manhattan(State state){
    int manh = 0;
    for(int i = 0;i < state.n;i++){
        for(int j = 0;j < state.n;j++){
            if(state.a[i][j]){
                pii current = make_pair(i,j);
                pii goal    = getGoalPosition(state.a[i][j],state.n);

                manh = manh + abs(current.first  - goal.first)
                            + abs(current.second - goal.second);
            }
        }
    }
    return manh;
}

int linearConflict(State state){
    int conflict = 0;
    //row line
    vector<int> b;
    for(int i = 0;i < state.n;i++){
        b.clear();
        for(int j = 0;j < state.n;j++){
            pii pos = getGoalPosition(state.a[i][j],state.n);
            if( state.a[i][j] && (i == pos.first) ){
                b.push_back(state.a[i][j]);
            }
        }
        //a row line is in b
        for(int k = 0;k < (int)b.size() - 1;k++){
            for(int l = k + 1;l < b.size();l++){
                if(b[k] > b [l]){//linear conflict
                    //printf("(%d,%d)\n",b[k],b[l]);
                    conflict++;
                }
            }
        }
    }
    //column line
    for(int j = 0;j < state.n;j++){
        b.clear();
        for(int i = 0;i < state.n;i++){
            pii pos = getGoalPosition(state.a[i][j],state.n);
            if( state.a[i][j] && (j == pos.second) )
                b.push_back(state.a[i][j]);
        }
        //a line is in b
        for(int k = 0;k < (int)b.size() - 1;k++){
            for(int l = k + 1;l < b.size();l++){
                if(b[k] > b [l]){//linear conflict
                    //printf("(%d,%d)\n",b[k],b[l]);
                    conflict++;
                }
            }
        }
    }
    return manhattan(state) + 2*conflict;
}
