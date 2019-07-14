#include <iostream>
#include <algorithm>

using namespace std;

class Tree{
    struct Node{
        int data;
        Node* right, *left;
        size_t height;
    };
    Node* root;

public:
    Tree() : root(NULL){}
    void  insert(int val){
        root = _insert(root, val);
    }
    void inorder(){
        _inorder(root);
    }
    void preorder(){
        _preorder(root);
    }
    void removeNode(int val){
        root = _removeNode(root, val);
    }
    long long rangeSum(int l, int r){
        return _rangeSum(root, l, r);
    }
    bool find(int val){
        return _find(root, val);
    }
private:
    Tree(const Tree&);
    Tree& operator= (const Tree&);
    Node* _newNode(int val){
        return new Node({val, NULL, NULL, 1});
    }
    int _height(Node *p){
        if(p == NULL)
            return 1;
        else{
            p->height = max(_height(p->left), _height(p->right)) + 1;
        }
        return p->height;
    }
    Node* _rightRotate(Node* p){
        Node* child = p->left; 
        p->left = child->right;
        child->right = p;

        p->height = _height(p);
        child->height = _height(child);

        return child;
    }
    Node* _leftRotate(Node* p){
        Node* child = p->right;
        p->right= child->left;
        child->left = p;

        p->height = _height(p);
        child->height = _height(child);

        return child;
    }
    int _getBalance(Node *p){
        if(p == NULL)
            return 0;
        else
            return _height(p->left) - _height(p->right);
    }
    Node* _minElement(Node* node){
        if(node != NULL && node->left != NULL)
            return _minElement(node->left);
        return node;
    } 
    Node* _rebalance(Node* node){
        node->height = _height(node);
        int balance = _getBalance(node);

        // Left Rotation
        if (balance > 1 && _getBalance(root->left) >= 0)  
            return _rightRotate(node);  

        // Right Rotation
        if(balance < -1 && _getBalance(node->right) <= 0)
            return _leftRotate(node);
        
        // Left Right Rotation 
        if(balance > 1 && _getBalance(node->left) < 0){
            node->left = _leftRotate(node->left);
            return _rightRotate(node);
        }
        // Right Left Rotation
        if(balance < -1 && _getBalance(node->right) > 0){
            node->right = _rightRotate(node->right);
            return _leftRotate(node);
        }
        return node;
    } 
    Node* _insert(Node*& node, int val){
        if(node == NULL) 
            return(_newNode(val));
        if(val == node->data)
            return node;
        if(val < node->data){
            node->left = _insert(node->left, val);
        }
        else {
            node->right = _insert(node->right, val);
        }
        return _rebalance(node);
    }
    Node* _removeNode(Node*& node, int val){
        if(node == NULL)
            return node;
        if(val < node->data)  
            node->left = _removeNode(node->left, val);  
        else if(val > node->data) 
            node->right = _removeNode(node->right, val);  
        else{
            if(node->left == NULL && node->right == NULL)
                delete node;
            else if(node->left == NULL || node->right == NULL){
                Node* tmp = node->left != NULL ? node->left : node->right;
                node->data = tmp->data;
                delete tmp;
            }
            else{
                Node* tmp = _minElement(node->right);
                node->data = tmp->data;
                node->right = _removeNode(node->right, tmp->data);
            }
        }
        if(node == NULL)
            return node;
        return _rebalance(node);
    }
    void _inorder(Node *p){
        if(p != NULL){
            _inorder(p->left);
            cout << p->data << " ";
            _inorder(p->right);
        }
    }
    long long _rangeSum(Node* node, int l, int r){
        if(node != NULL){
            long long sum = _rangeSum(node->left, l, r) + _rangeSum(node->right, l, r);
            if(node->data <= l && node->data >= r)
                sum += node->data;
            return sum;
        }
        return 0;
    }
    bool _find(Node* node, int val){
        if(node == NULL)
            return false;
        if(node->data == val)
            return true;
        else if(val < node->data)  
            return _find(node->left, val);  
        else 
            return _find(node->right, val);  
    }
    void _preorder(Node *p){
        if(p != NULL){
            cout << p->data << " ";
            _preorder(p->left);
            _preorder(p->right);
        }
    }
};
int main(){
    Tree tree;  
    tree.insert(10);  
    tree.insert(20);  
    tree.insert(30);  
    tree.insert(40);  
    tree.insert(50);  
    tree.insert(25);

    tree.preorder();
    cout << endl;
    tree.inorder();
    cout << boolalpha << tree.find(40) << endl;
    cout << boolalpha << tree.find(21) << endl;

    cout << tree.rangeSum(20, 30);

    return 0;
}