#include <bits/stdc++.h>

#define F first
#define S second

using namespace std;
typedef long long ll;
typedef long double ld;

const int MAX_N = 1e3 + 10;
const ll inf = 1e7, INF = 1e18;

template <class Type>
class Btree_Node
{
public:
    Type *key;
    int t; // Order of the B-tree
    Btree_Node **child;
    Btree_Node *parent;
    int full;
    bool leaf;
    Btree_Node(int _t, bool _leaf)
    {
        t = _t;
        leaf = _leaf;
        key = new Type[2 * t + 5];
        child = new Btree_Node *[2 * t + 1 + 5];
        full = 0;
        parent = NULL;
    }
};

template <class Type>
class Btree
{
public:
    Btree_Node<Type> *root;
    int t; // Order of the B-tree
    Btree(int _t)
    {
        root = NULL;
        t = _t;
    }

    Btree_Node<Type> *search(Type key, Btree_Node<Type> *node)
    {
        if (node == NULL)
        {
            return NULL;
        }
        int i = 0;
        while (i < (*node).full && key > (*node).key[i])
        {
            i++;
        }
        if ((*node).key[i] == key)
        {
            return node;
        }
        if ((*node).leaf == true)
        {
            return NULL;
        }
        return search(key, (*node).child[i]);
    }

    Btree_Node<Type> *min(Btree_Node<Type> *node)
    {
        Btree_Node<Type> *temp = node;
        while ((*temp).leaf != true)
        {
            temp = (*temp).child[0];
        }
        return temp;
    }

    void normalize(Btree_Node<Type> *node)
    {
        if ((*node).full <= 2 * t - 1 && (*node).full >= t - 1)
        {
            return;
        }
        else if ((*node).full == 2 * t)
        {
            Btree_Node<Type> *right;
            right = new Btree_Node<Type> (t, (*node).leaf);
            for (int i = t + 1; i < 2 * t; i++)
            {
                (*right).key[i - t - 1] = (*node).key[i];
            }
            if ((*node).leaf == false)
            {
                for (int i = t + 1; i < 2 * t + 1; i++)
                {
                    (*right).child[i - t - 1] = (*node).child[i];
                    if ((*node).child[i] != NULL)
                    {
                        (*(*node).child[i]).parent = right;
                    }
                }
            }
            Btree_Node<Type> *parent = (*node).parent;
            (*right).full = t - 1;
            (*node).full = t;
            if (parent != NULL)
            {
                (*right).parent = parent;
                int i = 0;
                while (i < (*parent).full && (*node).key[t] > (*parent).key[i])
                {
                    i++;
                }
                for (int j = (*parent).full - 1; j >= i; j--)
                {
                    (*parent).key[j + 1] = (*parent).key[j];
                }
                for (int j = (*parent).full; j >= i + 1; j--)
                {
                    (*parent).child[j + 1] = (*parent).child[j];
                }
                (*parent).key[i] = (*node).key[t];
                (*parent).child[i] = node;
                (*parent).child[i + 1] = right;
                (*parent).full++;
            }
            else
            {
                root = new Btree_Node<Type> (t, false);
                (*root).key[0] = (*node).key[t];
                (*root).child[0] = node;
                (*root).child[1] = right;
                (*root).full = 1;
                (*node).parent = root;
                (*right).parent = root;
            }
        }
        else if ((*node).full <= t - 2)
        {
            if (node == root)
            {
                if ((*node).full == 0)
                {
                    root = (*root).child[0];
                    (*root).parent = NULL;
                    if ((*root).full == 0)
                    {
                        root = NULL;
                    }
                }
                return;
            }
            Btree_Node<Type> *parent = (*node).parent;
            int i = 0;
            while (node != (*parent).child[i])
            {
                i++;
            }
            if (i < (*parent).full)
            {
                Btree_Node<Type> *sibling = (*parent).child[i + 1];
                if ((*sibling).full > t - 1)
                {
                    (*node).key[(*node).full] = (*parent).key[i];
                    (*node).full++;
                    (*node).child[(*node).full] = (*sibling).child[0];
                    if ((*sibling).child[0] != NULL)
                    {
                        (*(*sibling).child[0]).parent = node;
                    }
                    (*parent).key[i] = (*sibling).key[0];
                    for (int j = 0; j < (*sibling).full; j++)
                    {
                        (*sibling).key[j] = (*sibling).key[j + 1];
                        (*sibling).child[j] = (*sibling).child[j + 1];
                    }
                    (*sibling).full--;
                }
                else
                {
                    (*node).key[(*node).full] = (*parent).key[i];
                    (*node).full++;
                    for (int j = 0; j <= (*sibling).full; j++)
                    {
                        (*node).key[(*node).full] = (*sibling).key[j];
                        (*node).child[(*node).full + 1] = (*sibling).child[j];
                        if ((*sibling).child[j] != NULL)
                        {
                            (*(*sibling).child[j]).parent = node;
                        }
                        (*node).full++;
                    }
                    (*node).full--;
                    for (int j = i + 1; j < (*parent).full; j++)
                    {
                        (*parent).key[j] = (*parent).key[j + 1];
                        (*parent).child[j] = (*parent).child[j + 1];
                    }
                    (*parent).full--;
                }
            }
            else
            {
                Btree_Node<Type> *sibling = (*parent).child[i - 1];
                if ((*sibling).full > t - 1)
                {
                    for (int j = (*node).full + 1; j > 0; j--)
                    {
                        (*node).key[j] = (*node).key[j - 1];
                        (*node).child[j] = (*node).child[j - 1];
                    }
                    (*node).key[0] = (*parent).key[i - 1];
                    (*node).child[0] = (*sibling).child[(*sibling).full];
                    if ((*sibling).child[(*sibling).full] != NULL)
                    {
                        (*(*sibling).child[(*sibling).full]).parent = node;
                    }
                    (*node).full++;
                    (*parent).key[i - 1] = (*sibling).key[(*sibling).full - 1];
                    (*sibling).full--;
                }
                else
                {
                    (*sibling).key[(*sibling).full] = (*parent).key[i - 1];
                    (*sibling).full++;
                    for (int j = 0; j <= (*node).full; j++)
                    {
                        (*sibling).key[(*sibling).full] = (*node).key[j];
                        (*sibling).child[(*sibling).full] = (*node).child[j];
                        if ((*node).child[j] != NULL)
                        {
                            (*(*node).child[j]).parent = sibling;
                        }
                        (*sibling).full++;
                    }
                    (*sibling).full--;
                    for (int j = i; j < (*parent).full; j++)
                    {
                        (*parent).key[j - 1] = (*parent).key[j];
                        (*parent).child[j] = (*parent).child[j + 1];
                    }
                    (*parent).full--;
                }
            }
        }
    }

    void insertintree(Type key, Btree_Node<Type> *node)
    {
        int i = 0;
        while (i < (*node).full && key > (*node).key[i])
        {
            i++;
        }
        if ((*node).leaf == true)
        {
            for (int j = (*node).full - 1; j >= i; j--)
            {
                (*node).key[j + 1] = (*node).key[j];
            }
            (*node).key[i] = key;
            (*node).full++;
            normalize(node);
            return;
        }
        insertintree(key, (*node).child[i]);
        normalize(node);
    }

    void insert(Type key)
    {
        if (root == NULL)
        {
            root = new Btree_Node<Type> (t, true);
            (*root).key[0] = key;
            (*root).full = 1;
            (*root).parent = NULL;
        }
        else
        {
            insertintree(key, root);
        }
    }

    void deleteinleaf(Btree_Node<Type> *node, int index)
    {
        for (int i = index; i < (*node).full - 1; i++)
        {
            cerr << i << endl;
            (*node).key[i] = (*node).key[i + 1];
        }
        (*node).full--;
        while (node != NULL)
        {
            normalize(node);
            node = (*node).parent;
        }
    }

    void del(Type key) // Delete function
    {
        Btree_Node<Type> *node = search(key, root);
        if (node == NULL)
        {
            return;
        }
        int i = 0;
        while ((*node).key[i] != key)
        {
            i++;
        }
        if ((*node).leaf == false)
        {
            Btree_Node<Type> *min_node = min((*node).child[i + 1]);
            (*node).key[i] = (*min_node).key[0];
            deleteinleaf(min_node, 0);
        }
        else
        {
            deleteinleaf(node, i);
        }
    }
};


int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int t;
    cin >> t;
    Btree<int> tree(t);
    while (true)
    {
        string s;
        cin >> s;
        if (s == "insert")
        {
            int x;
            cin >> x;
            tree.insert(x);
        }
        else if (s == "delete")
        {
            int x;
            cin >> x;
            tree.del(x);
        }
        else if (s == "search")
        {
            int x;
            cin >> x;
            if (tree.search(x, tree.root) != NULL)
            {
                cout << "Found\n";
            }
            else
            {
                cout << "Not Found\n";
            }
        }
        else if (s == "exit")
        {
            break;
        }
    }
    return 0;
}
