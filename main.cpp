#include <iostream>
#include <ctime>
#define TYPE int
#define MAX_ABS 100

using namespace std;

class AVL_tree
{
public:
    struct Node
    {
        TYPE key;
        Node *left;
        Node *right;
        unsigned char height;
        Node(TYPE t) {key=t; left=right=nullptr; height=1; }
    };
    Node *root; //корень дерева
    unsigned char height(Node *p) //вычисление высоты поддерева p
    {
        if (p!=nullptr) return p->height;
        else return 0;
    }
    int bfactor(Node *p) //вычисление разницы высот правого и левого поддерева узла p
    {
        return height(p->right)-height(p->left);
    }
    void fix_height(Node *p) // восстановление правильного значения высоты поддерева p
    {
        if ((p->left!=nullptr)&&(p->right!=nullptr))
        {
            p->height = max(p->left->height,p->right->height)+1;
        }
        else if ((p->left==nullptr)&&(p->right==nullptr))
        {
            p->height=1;
        }
        else if (p->left==nullptr)
        {
            p->height=p->right->height+1;
        }
        else if (p->right==nullptr)
        {
            p->height=p->left->height+1;
        }
        return;
    }
    void rotate_right(Node *&p) // праввый поворот относительно узла p
    {
        Node *new_p =p->left;
        Node *tmp = p;
        tmp->left = new_p->right;
        new_p->right=tmp;
        fix_height(tmp);
        fix_height(new_p);
        p=new_p;
    }
    void rotate_left(Node *&p) // левый поворот относительно узла p
    {
        Node *new_p =p->right;
        Node *tmp = p;
        tmp->right = new_p->left;
        new_p->left=tmp;
        fix_height(tmp);
        fix_height(new_p);
        p=new_p;
    }
    void balance(Node *&p) // балансировка узла p
    {
        fix_height(p);
        if( bfactor(p)==2 )
        {
            if( bfactor(p->right) < 0 )
                rotate_right(p->right);
            rotate_left(p);
            return;
        }
        if( bfactor(p)==-2 )
        {
            if( bfactor(p->left) > 0  )
                rotate_left(p->left);
            rotate_right(p);
            return;
        }
        return;
    }
    Node* search(TYPE key) // поиск в дереве узла с заданым ключом
    {
        Node *current=root;
        while (current!=nullptr)
        {
            if (current->key==key) return current;
            else if (key < current->key) current = current->left;
            else current = current->right;
        }
        return nullptr;
    }
    Node* insert(Node *&p, TYPE key) //вставка нового узла с ключом key в дерево p
    {
        if(p == nullptr)
        {
            p = new Node(key);
            return p;
        }
        if(key<p->key)
            p->left = insert(p->left,key);
        else
            p->right = insert(p->right,key);
        balance(p);
        return p;
    }
    Node* find_min(Node *p) // поиск узла с минимальным ключом в дереве p
    {
        if (p->left == nullptr) return p;
        else return find_min(p->left);
    }
    Node* removemin(Node *&p) // удаление узла с минимальным ключом из дерева p
    {
        if( p->left==nullptr )
            return p->right;
        p->left = removemin(p->left);
        balance(p);
        return p;
    }
    Node* remove_sub(Node *&p, int key) // удаление ключа key из дерева p
    {
        if( key < p->key )
            p->left = remove_sub(p->left,key);
        else if( key > p->key )
            p->right = remove_sub(p->right,key);
        else //  при k == p->key
        {
            Node* q = p->left;
            Node* r = p->right;
            delete p;
            if( !r ) return q;
            Node* min = find_min(r);
            min->right = removemin(r);
            min->left = q;
            balance(min);
            return min;
        }
        balance(p);
        return p;
    }
    void print_sub(Node *q, long n) //вспомогательная рекурсивная функция для вывода дерева на экран
    {
       long i;
       if (q)
       {
          print_sub(q->right, n+5);
          for (i = 0; i < n; i++)
             printf(" ");
          printf("%d\n", q->key);
          print_sub(q->left, n+5);
       }
    }
public:
    AVL_tree() // конструктор дерева
    {
        root=nullptr;
    }
    bool add(TYPE key) // добавление узла с ключом k в дерево с последующей балансировкой
    {
        if (search(key)==nullptr)
        {
            insert(root,key);
            return true;
        }
        else return false;

    }
    bool remove(TYPE key) // удаление узла с ключом k из дерева с последующей балансировкой
    {
        Node *target = search(key);
        if (target==nullptr) return false;
        else remove_sub(root, key);
        return true;
    }
    void print() //вывод дерева в консоль
    {
        print_sub(root,0);
    }
};


int main()
{
    int n;
    cout << "Number of nodes:";
    cin >> n;
    AVL_tree tree;
    TYPE tmp;
    srand(time(nullptr));
    for (int i=0;i<n;i++)
    {
        tmp=(rand()%(MAX_ABS*2))-MAX_ABS;
        while (tree.search(tmp)!=nullptr)
            tmp=(rand()%(MAX_ABS*2))-MAX_ABS;
        tree.add(tmp);
        cout << tmp << " | ";
    }
    cout <<"\nResult tree:\n\n";
    tree.print();
    return 0;
}
