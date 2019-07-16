#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>

using namespace std;

typedef long long ll;

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
    void inorder()const{
        _inorder(root);
    }
    void preorder()const{
        _preorder(root);
    }
    void remove(int val){
        root = _remove(root, val);
    }
    long long rangeSum(int l, int r)const{
        return _rangeSum(root, l, r);
    }
    bool find(int val)const{
        return _find(root, val);
    }
    ll at(size_t index)const{
        return _at(root, index);
    }
    size_t size()const{
        return _size(root);
    }
private:
    Tree(const Tree&);
    Tree& operator= (const Tree&);
    Node* _newNode(int val){
        return new Node({val, NULL, NULL, 1});
    }
    size_t _height(Node *p)const{
        if(p == NULL)
            return 0;
        else{
            p->height = max(_height(p->left), _height(p->right)) + 1;
        }
        return p->height;
    }
    size_t _size(Node* node)const{
        if(node == NULL)
            return 0;
        return _size(node->left) + _size(node->right) + 1;
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
    int _getBalance(Node *p)const{
        if(p == NULL)
            return 0;
        else
            return _height(p->left) - _height(p->right);
    }
    Node* _minElement(Node* node)const{
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
    Node* _remove(Node*& node, int val){
        if(node == NULL)
            return node;
        if(val < node->data)  
            node->left = _remove(node->left, val);  
        else if(val > node->data) 
            node->right = _remove(node->right, val);  
        else{
             if(node->left == NULL || node->right == NULL){
                Node* tmp = node->left != NULL ? node->left : node->right;
                if(tmp == NULL){
                    delete node;
                    node = NULL;
                }else{
                    *node = *tmp;
                    delete tmp;
                }
            }
            else{
                Node* tmp = _minElement(node->right);
                node->data = tmp->data;
                node->right = _remove(node->right, tmp->data);
            }
        }
        if(node == NULL)
            return node;
        return _rebalance(node);
    }
    void _inorder(Node *p)const{
        if(p != NULL){
            _inorder(p->left);
            cout << p->data << " ";
            _inorder(p->right);
        }
    }
    long long _rangeSum(Node* node, int l, int r)const{
        if(node != NULL){
            if(node->data >= l && node->data <= r)
                return node->data + _rangeSum(node->left, l, r) 
                + _rangeSum(node->right, l, r);
            else if(node->data < l)
                return _rangeSum(node->right, l, r);
            else 
                _rangeSum(node->left, l, r); 
        }
        return 0;
    }
    bool _find(Node* node, int val)const{
        if(node == NULL)
            return false;
        if(node->data == val)
            return true;
        else if(val < node->data)  
            return _find(node->left, val);  
        else 
            return _find(node->right, val);  
    }
    void _preorder(Node *p)const{
        if(p != NULL){
            cout << p->data << " ";
            _preorder(p->left);
            _preorder(p->right);
        }
    }
    ll _at(Node* node, size_t index)const{ 
        assert(node != NULL);
        if(node->left != NULL){
            ll size = node->left->height + 1;
            if(size == index)
                return node->data;
            if(size > index)
                return _at(node->left, index);
            else 
                return _at(node->right, index - size);
        }
        else 
            if(index == 0)  return node->data;
        else
            return _at(node->right, index - 1);
    }
};
int main(){
    Tree tree;  
    ifstream fs("test.txt");
    assert(fs.is_open());
   
    int numOfReq;
    ll val, l, r;
    char typeReq;
    fs >> numOfReq;
    for(int i = 0; i < numOfReq; ++i){
        fs >> typeReq;
        if(typeReq == '+'){
            fs >> val;
            tree.insert(val);
        }else if(typeReq == '-'){
            fs >> val;
            tree.remove(val);
        }else if(typeReq == '?'){
            fs >> val;
            cout << (tree.find(val) ? "Found" : "Not found") << endl;
        }else if(typeReq == 's'){
            fs >> l >> r;
            cout << tree.rangeSum(l, r) << endl;
        }
    }
    tree.inorder();
    cout << endl << tree.at(0) << " " << tree.at(1) << endl;
    return 0;
}