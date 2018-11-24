#include <bits/stdc++.h>

using namespace std;

typedef vector< vector<int> > matrix;
typedef pair<int,int> pii;

#define LEFT  0
#define RIGHT 1
#define UP    2
#define DOWN  3

pair<pii,string> moves[4] = {
                                make_pair( make_pair( 0, 1), string("LEFT") ),
                                make_pair( make_pair( 0,-1), string("RIGHT")),
                                make_pair( make_pair( 1, 0), string("UP")   ),
                                make_pair( make_pair(-1, 0), string("DOWN") )
                            };

struct State{
    matrix a;
    int n;
    pii zero;
    string path;

    State(matrix _a,pii _zero){
        this->a    = _a;
        this->zero = _zero;
        this->n    = a.size();
        this->path = "";
    }

    State& operator=(const State& other){
        this->a    = other.a;
        this->n    = other.n;
        this->zero = other.zero;
        this->path = other.path;
        return *this;
    }

    bool canMove(pii offset){
        int i = zero.first  + offset.first;
        int j = zero.second + offset.second;
        //if i crosses the grid its invalid move
        return !(i < 0 || j < 0 || i >= n || j >= n);
    }

    State move(pii offset,string moveName){
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

        char ch[100];
        sprintf(ch,"move %d ",ret.a[prev_i][prev_j]);

        ret.path = this->path + "\nState  :\n" + toString()
                              +   "Action : "   + string(ch) + moveName + "\n\n";

        //cout << ret.a[prev_i][prev_j] << " " << moveName << endl;

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
        //printf("in toString()\n%s\n",s);
        return string(s);
    }

    bool isSolvable(){
        vector<int> arr;
        int inversions = 0;
        int blank_row = zero.first;

        for(int i = 0;i < n;i++)
            for(int j = 0;j < n;j++)
                if(a[i][j]) arr.push_back(a[i][j]);

        for(int i = 0;i < arr.size() - 1;i++){
            for(int j = i + 1;j < arr.size();j++){
                if(arr[i] > arr [j]){
                    inversions++;
                    //printf("(%d,%d)\n",arr[i],arr[j]);
                }
            }
        }

        printf("blank row %d\ninversions %d\n",blank_row,inversions);

        if(n % 2 == 1){
            //number of inversions must be EVEN to be solvable
            return (bool) !(inversions % 2);
        }else{
            //number of inversions + blank row must be ODD to be solvable
            return (bool) ( (inversions + blank_row) % 2 );
        }
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

/*
int main(){
    freopen("in.txt","r",stdin);

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
    /*cout << start << endl;

    start = start.move(moves[LEFT].first,moves[LEFT].second);
    cout << start << endl;

    start = start.move(moves[UP].first,moves[UP].second);
    cout << start << endl;

    start = start.move(moves[LEFT].first,moves[LEFT].second);
    cout << start << endl;

    start = start.move(moves[UP].first,moves[UP].second);
    cout << start << endl;


    cout << "toString() -> " << endl << start.toString() << endl;
    /*cout << "left " << boolalpha << start.canMove(moves[LEFT].first) << endl;
    cout << "right " << boolalpha << start.canMove(moves[RIGHT].first) << endl;
    cout << "up " << boolalpha << start.canMove(moves[UP].first) << endl;
    cout << "down " << boolalpha << start.canMove(moves[DOWN].first) << endl;*/
    /*State next = start.move(moves[UP].first,moves[UP].second);
    cout << start << endl << next;
    cout << boolalpha << (next < start);

    //State goal = getGoal(4);
    //cout << goal << endl;
    return 0;
}
*/
