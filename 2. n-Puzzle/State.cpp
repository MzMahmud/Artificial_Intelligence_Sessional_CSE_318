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

pii getGoalPosition(int a,int n){
    if(a == 0)
        return make_pair(n-1,n-1);

    return make_pair((a-1)/n,(a-1)%n);
}

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


        //builds path
        char ch[100];
        sprintf(ch,"move %d ",ret.a[prev_i][prev_j]);
        ret.path = this->path + "\nState  :\n" + toString()
                              +   "Action : "   + string(ch) + moveName + "\n\n";


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

        for(int i = 0;i < (int)arr.size() - 1;i++){
            for(int j = i + 1;j < arr.size();j++){
                if(arr[i] > arr [j]){
                    inversions++;
                    //printf("(%d,%d)\n",arr[i],arr[j]);
                }
            }
        }

        //printf("blank row %d\ninversions %d\n",blank_row,inversions);

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

    int hamming(){
        int hamm = 0;
        for(int i = 0;i < n;i++){
            for(int j = 0;j < n;j++){
                if(a[i][j]){
                    pii current = make_pair(i,j);
                    pii goal    = getGoalPosition(a[i][j],n);

                    //cout << a[i][j] << " " << (current != goal) << endl;

                    hamm = hamm + (int) (current != goal);
                }
            }
        }
        return hamm;
    }

    int manhattan(){
        int manh = 0;
        for(int i = 0;i < n;i++){
            for(int j = 0;j < n;j++){
                if(a[i][j]){
                    pii current = make_pair(i,j);
                    pii goal    = getGoalPosition(a[i][j],n);

                    manh = manh + abs(current.first  - goal.first)
                                + abs(current.second - goal.second);

                    //cout << a[i][j] << " " << (abs(current.first  - goal.first)
                    //            + abs(current.second - goal.second)) << endl;
                }
            }
        }
        return manh;
    }

    int linearConflict(){

        int conflict = 0;

        //row line
        vector<int> b;
        for(int i = 0;i < n;i++){
            b.clear();
            for(int j = 0;j < n;j++){
                pii pos = getGoalPosition(a[i][j],n);
                if( a[i][j] && (i == pos.first) ){
                    b.push_back(a[i][j]);
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
        for(int j = 0;j < n;j++){
            b.clear();
            for(int i = 0;i < n;i++){
                pii pos = getGoalPosition(a[i][j],n);
                if( a[i][j] && (j == pos.second) ){
                    b.push_back(a[i][j]);
                }
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

        return manhattan() + 2*conflict;
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

    cout << "hamming   = " << start.hamming() << endl;
    cout << "manhattan = " << start.manhattan() << endl;
    cout << "linearConflict = " << start.linearConflict() << endl;

    return 0;
}

