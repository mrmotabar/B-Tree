#include <bits/stdc++.h>

#define F first
#define S second

#define IOS ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0);

using namespace std;
typedef long long ll;
typedef long double ld;

const int MAX_N = 1e3 + 10;
const ll  inf = 1e7, INF = 1e18;

class btreenode{
    public:
        pair<int, pair<int, int>> *key;
        int t;
        btreenode **child;
        btreenode *par;
        int full;
        bool leaf;
        btreenode(int _t, bool _leaf){
            t = _t;
            leaf = _leaf;
            key = new pair<int, pair<int, int>>[2 * t + 5];
            child = new btreenode *[2 * t + 1 + 5];
            full = 0;
            par = NULL;
        }
};

class btree{
    public:
        btreenode *root;
        int t;
        btree(int _t){
            root = NULL;
            t = _t;
        }
        btreenode *search(pair<int, pair<int, int>> k, btreenode *v){
            if(v == NULL){
                return NULL;
            }
            int i = 0;
            while(i < (*v).full && k > (*v).key[i]){
                i++;
            }
            if((*v).key[i] == k){
                return v;
            }
            if((*v).leaf == true){
                return NULL;
            }
            return search(k, (*v).child[i]);
        }
        btreenode *miin(btreenode *v){
            btreenode *temp = v;
            while((*temp).leaf != true){
                temp = (*temp).child[0];
            }
            return temp;
        }
        void normalize(btreenode *v){
            if((*v).full <= 2 * t - 1 && (*v).full >= t - 1){
                return;
            }
            else if((*v).full == 2 * t){
                btreenode *right;
                right = new btreenode(t, (*v).leaf);
                for(int i = t + 1; i < 2 * t; i++){
                    (*right).key[i - t - 1] = (*v).key[i];
                }
                if((*v).leaf == false){
                    for(int i = t + 1; i < 2 * t + 1; i++){
                        (*right).child[i - t - 1] = (*v).child[i];
                        if((*v).child[i] != NULL){
                            (*(*v).child[i]).par = right;
                        }
                    }
                }
                btreenode *par = (*v).par;
                (*right).full = t - 1;
                (*v).full = t;
                if(par != NULL){
                    (*right).par = par;
                    int i = 0;
                    while(i < (*par).full && (*v).key[t] > (*par).key[i]){
                        i++;
                    }
                    for(int j = (*par).full - 1; j >= i; j--){
                        (*par).key[j + 1] = (*par).key[j];
                    }
                    for(int j = (*par).full; j >= i + 1; j--){
                        (*par).child[j + 1] = (*par).child[j];
                    }
                    (*par).key[i] = (*v).key[t];
                    (*par).child[i] = v;
                    (*par).child[i + 1] = right;
                    (*par).full++;
                }
                else{
                    root = new btreenode(t, false);
                    (*root).key[0] = (*v).key[t];
                    (*root).child[0] = v;
                    (*root).child[1] = right;
                    (*root).full = 1;
                    (*v).par = root;
                    (*right).par = root;
                }
            }
            else if((*v).full <= t - 2){
                if(v == root){
                    if((*v).full == 0){
                        root = (*root).child[0];
                        (*root).par = NULL;
                        if((*root).full == 0){
                            root = NULL;
                        }
                    }
                    return;
                }
                btreenode *par = (*v).par;
                int i = 0;
                while(v != (*par).child[i]){
                    i++;
                }
                if(i < (*par).full){
                    btreenode *sib = (*par).child[i + 1];
                    if((*sib).full > t - 1){
                        (*v).key[(*v).full] = (*par).key[i];
                        (*v).full++;
                        (*v).child[(*v).full] = (*sib).child[0];
                        if((*sib).child[0] != NULL){
                            (*(*sib).child[0]).par = v;
                        }
                        (*par).key[i] = (*sib).key[0];
                        for(int j = 0; j < (*sib).full; j++){
                            (*sib).key[j] = (*sib).key[j + 1];
                            (*sib).child[j] = (*sib).child[j + 1];
                        }
                        (*sib).full--;
                    }
                    else{
                        (*v).key[(*v).full] = (*par).key[i];
                        (*v).full++;
                        for(int j = 0; j <= (*sib).full; j++){
                            (*v).key[(*v).full] = (*sib).key[j];
                            (*v).child[(*v).full + 1] = (*sib).child[j];
                            if((*sib).child[j] != NULL){
                                (*(*sib).child[j]).par = v;
                            }
                            (*v).full++;
                        }
                        (*v).full--;
                        for(int j = i + 1; j < (*par).full; j++){
                            (*par).key[j] = (*par).key[j + 1];
                            (*par).child[j] = (*par).child[j + 1];
                        }
                        (*par).full--;
                    }
                }
                else{
                    btreenode *sib = (*par).child[i - 1];
                    if((*sib).full > t - 1){
                        for(int j = (*v).full + 1; j > 0; j--){
                            (*v).key[j] = (*v).key[j - 1];
                            (*v).child[j] = (*v).child[j - 1];
                        }
                        (*v).key[0] = (*par).key[i - 1];
                        (*v).child[0] = (*sib).child[(*sib).full];
                        if((*sib).child[(*sib).full] != NULL){
                            (*(*sib).child[(*sib).full]).par = v;
                        }
                        (*v).full++;
                        (*par).key[i - 1] = (*sib).key[(*sib).full - 1];
                        (*sib).full--;
                    }
                    else{
                        (*sib).key[(*sib).full] = (*par).key[i - 1];
                        (*sib).full++;
                        for(int j = 0; j <= (*v).full; j++){
                            (*sib).key[(*sib).full] = (*v).key[j];
                            (*sib).child[(*sib).full] = (*v).child[j];
                            if((*v).child[j] != NULL){
                                (*(*v).child[j]).par = sib;
                            }
                            (*sib).full++;
                        }
                        (*sib).full--;
                        for(int j = i; j < (*par).full; j++){
                            (*par).key[j - 1] = (*par).key[j];
                            (*par).child[j] = (*par).child[j + 1];
                        }
                        (*par).full--;
                    }
                }
            }
        }
        void insertintree(pair<int, pair<int, int>> k, btreenode *v){
            int i = 0;
            while(i < (*v).full && k > (*v).key[i]){
                i++;
            }
            if((*v).leaf == true){
                for(int j = (*v).full - 1; j >= i; j--){
                    (*v).key[j + 1] = (*v).key[j];
                }
                (*v).key[i] = k;
                (*v).full++;
                normalize(v);
                return;
            }
            insertintree(k, (*v).child[i]);
            normalize(v);
        }
        void insert(pair<int, pair<int, int>> k){
            if(root == NULL){
                root = new btreenode(t, true);
                (*root).key[0] = k;
                (*root).full = 1;
                (*root).par = NULL;
            }
            else{
                insertintree(k, root);
            }
        }
        void deleteinleaf(btreenode *v, int ind){
            for(int i = ind; i < (*v).full; i++){
                (*v).key[i] = (*v).key[i + 1];
            }
            (*v).full--;
            while(v != NULL){
                normalize(v);
                v = (*v).par;
            }
        }
        void del(pair<int, pair<int, int>> k){
            btreenode *node = search(k, root);
            if(node == NULL){
                return;
            }
            int i = 0;
            while((*node).key[i] != k){
                i++;
            }
            if((*node).leaf == false){
                btreenode *mi = miin((*node).child[i + 1]);
                (*node).key[i] = (*mi).key[0];
                deleteinleaf(mi, 0);
            }
            else{
                deleteinleaf(node, i);
            }
        }
};

int n, m, table[MAX_N][MAX_N], dp[MAX_N][MAX_N], xx[4] = {1, -1, 0, 0}, yy[4] = {0, 0, 1, -1};
pair<int, int> par[MAX_N][MAX_N];
btree st(2);


int main(){
    IOS;
    cin>>n>>m;
    for(int i = 1; i <= n; i++){
        for(int j = 0; j < m; j++){
            cin>>table[i][j];
            if(table[i][j] ==  0){
                table[i][j] = -1;
            }
        }
    }
    for(int i = 0; i < n + 2; i++){
        for(int j = 0; j < m; j++){
            dp[i][j] = inf;
        }
    }
    dp[0][0] = 0;
    st.insert({dp[0][0], {0, 0}});
    while(st.root != NULL){
        pair<int, pair<int, int>> v = (*st.miin(st.root)).key[0];
        int x = v.S.F, y = v.S.S;
        st.del(v);
        for(int i = 0; i < 4; i++){
            if(x + xx[i] >= 0 && x + xx[i] < n + 2 && y + yy [i] >= 0 && y + yy[i] < m && table[x + xx[i]][y + yy[i]] != -1){
                if(v.F + table[x + xx[i]][y + yy[i]] < dp[x + xx[i]][y + yy[i]]){
                    st.del({dp[x + xx[i]][y + yy[i]], {x + xx[i], y + yy[i]}});
                    dp[x + xx[i]][y + yy[i]] = v.F + table[x + xx[i]][y + yy[i]];
                    st.insert({dp[x + xx[i]][y + yy[i]], {x + xx[i], y + yy[i]}});
                    par[x + xx[i]][y + yy[i]] = {x, y};
                }
            }
        }
    }
    int ind = 0;
    for(int i = 1; i < m; i++){
        if(dp[n + 1][i] < dp[n + 1][ind]){
            ind = i;
        }
    }
    for(int i = 0; i < n + 2; i++){
        for(int j = 0; j < m; j++){
            if(table[i][j] == -1){
                table[i][j] = 0;
            }
        }
    }
    int x = n + 1, y = ind;
    while(x != 0 || y != 0){
        table[x][y] = -1;
        pair<int, int> help = par[x][y];
        x = help.F;
        y = help.S;
    }
    cout<<endl;
    for(int i = 1; i < n + 1; i++){
        for(int j = 0; j < m; j++){
            cout<<table[i][j]<<" ";
        }
        cout<<endl;
    }
}
