#include<iostream>
using namespace std;
typedef struct SplayNode *Tree;
typedef int ElementType;
struct SplayNode{
    ElementType val;    //define node value
    Tree lchild;        //left child
    Tree rchild;        //right child
    Tree parent;        //parent node
    SplayNode(int val = 0){
        parent = nullptr;
        lchild = nullptr;
        rchild = nullptr;
        this->val = val;
    }
};
Tree left_single_rotation(Tree &root,Tree node);
Tree right_single_rotation(Tree &root,Tree node);
void left_double_rotation(Tree &root,Tree node);
void right_doublerotation(Tree &root,Tree node);
void left_rightrotation(Tree &root,Tree node);
void right_left_rotation(Tree &root,Tree node);
void SplayTree(Tree &root,Tree node);
bool search(Tree &root,ElementType val);
Tree* search_val(Tree &root,ElementType val,Tree &parent);
bool insert(Tree &root,ElementType val);
void up(Tree &root,Tree node);
void remove(Tree &root,ElementType val);
Tree* FindMin(Tree &root);
void PreOrder(Tree root);
void InOrder(Tree root);

Tree left_single_rotation(Tree &root,Tree node){
    if(node == nullptr)
        return nullptr;
    Tree parent = node->parent;         //store node's parent node
    Tree grandparent = parent->parent;  //store node's grandparent node
    parent->rchild = node->lchild;      //node's lchild as parent's rchild
    if(node->lchild)
        node->lchild->parent = parent;  //if node has lchild,update its parent node
    node->lchild = parent;              //update node's lchild
    parent->parent = node;              //update parent's parent node
    node->parent = grandparent;         //update node's parent node
    if(grandparent){                    //if grandparent node exists
        if(grandparent->lchild == parent)   //if node is grandparent's lchild
            grandparent->lchild = node;
        else
            grandparent->rchild = node;     // if node is grandparent's rchild
    }else{
        root = node;                    //if grandparent node doesn't exist
    }
    return node;    //node is new root 
}
Tree right_single_roration(Tree &root,Tree node){
    if(node == nullptr)
        return nullptr;
    Tree parent = node->parent;         //store node's parent node
    Tree grandparent = parent->parent;  //store node's grandparent node
    parent->lchild = node->rchild;      //node's rchild as parent's lchild
    if(node->rchild)
        node->rchild->parent = parent;  //if node has rchild,update its parent node
    node->rchild = parent;              //update node's rchild
    parent->parent = node;              //update parent's parent node
    node->parent = grandparent;         //update node's parent node
    if(grandparent){                    //if grandparent node exists
        if(grandparent->lchild == parent)   //if node is grandparent's lchild
            grandparent->lchild = node;
        else
            grandparent->rchild = node;     // if node is grandparent's rchild
    }else{
        root = node;                    //if grandparent node doesn't exist
    }
    return node;    //node is new root 
}
void left_double_rotation(Tree &root,Tree node){
    left_single_rotation(root,node->parent);   
    left_single_rotation(root,node);
}
void right_double_rotation(Tree &root,Tree node){
    right_single_roration(root,node->parent);
    right_single_roration(root,node);
}
void right_left_rotation(Tree &root,Tree node){
    right_single_roration(root,node);
    left_single_rotation(root,node);
}
void left_right_rotation(Tree &root,Tree node){
    left_single_rotation(root,node);
    right_single_roration(root,node);
}
Tree* search_val(Tree &root,ElementType val,Tree &parent){
    if(root == nullptr)
        return &root;
    if(val < root->val)
        return search_val(root->lchild,val,parent = root);
    else if(val > root->val)
        return search_val(root->rchild,val,parent = root);
    return &root;
}
bool search(Tree &root,ElementType val){
    Tree parent = nullptr;
    Tree *temp = nullptr;
    temp = search_val(root,val,parent);
    if(*temp == root)   //root->val == val
        return true;
    if(*temp && *temp != root){
        SplayTree(root,*temp);  //root->val != val ,need to splay
        return true;
    }
    return false;
}
bool insert(Tree &root,ElementType val){
    Tree *temp = nullptr;
    Tree parent = nullptr;
    temp = search_val(root,val,parent);
    if(*temp == nullptr){
        Tree node = new SplayNode(val);
        *temp = node;
        node->parent = parent;
        return true;
    }
    return false;
}
void SplayTree(Tree &root,Tree node){
    while (root->lchild != node && root->rchild != node && root != node) //node is not root's children and not root
        up(root, node);
    if (root->lchild == node) //node is root's left child
        root = right_single_roration(root,node);
    else if(root->rchild == node) //node is root's right child
        root = left_single_rotation(root,node);
}
void up(Tree &root,Tree node){
    Tree parent,grandparent;
    int i,j;
    parent = node->parent;
    grandparent = parent->parent;
    //zig-zig or zig-zag
    i = grandparent->lchild == parent ? -1 : 1;
    j = parent->lchild == node ? -1 : 1;
    if (i == -1 && j == -1) //LL shape
        right_double_rotation(root, node);
    else if(i==-1 && j==1) //LR shape
        left_right_rotation(root, node);
    else if(i==1 && j==-1) //RL shape
        right_left_rotation(root, node);
    else                    //RR shape
        left_double_rotation(root, node);
}
Tree *Find_Min(Tree &root){
    if (root->lchild)
        return Find_Min(root->lchild);
    return &root;
}
void remove(Tree &root,ElementType val){
    Tree parent = nullptr;
    Tree *temp = nullptr;
    Tree *replace = nullptr;
    Tree replace2 = nullptr;
    temp = search_val(root,val, parent); //find val first
    if(*temp) //val exits
    {
        if (*temp != root)    
            SplayTree(root, *temp);
        
        //delete root node, and find substitute(min value of right child)
        if (root->rchild)
        {
            //substitute exists
            replace = Find_Min(root->rchild); 
            root->val = (*replace)->val;  //replace
            if((*replace)->lchild == nullptr && (*replace)->rchild == nullptr){
                Tree parent1 = (*replace)->parent;
                parent1->lchild = nullptr;
                delete (*replace);
            }
            if((*replace)->rchild != nullptr){  //node's rchild exists
                replace2 = *replace;
                *replace = (*replace)->rchild; 
                (*replace)->parent = replace2->parent;  
                delete replace2;
            }
        }
        else
        {
            //no substitute
            replace2 = root;
            root = root->lchild;
            delete replace2;
        }
    }
}
void PreOrder(Tree root){
    if (root == nullptr)
        return;
    cout << root->val << " ";
    PreOrder(root->lchild);
    PreOrder(root->rchild);
}
//中序
void InOrder(Tree root){
    if (root == nullptr)
        return;
    InOrder(root->lchild);
    cout << root->val << " ";
    InOrder(root->rchild);
}
int main(){
    Tree root = nullptr;
    insert(root, 11);
    insert(root, 7);
    insert(root, 18);
    insert(root, 3);
    insert(root, 9);
    insert(root, 16);
    insert(root, 26);
    insert(root, 14);
    insert(root, 15);
    
    search(root,14);
    printf("find 14:\n");
    printf("preorder:");
    PreOrder(root);
    printf("\n");
    printf("inorder:");
    InOrder(root);
    printf("\n");
    
    remove(root,16);
    printf("remove 16:\n");
    printf("preorder:");
    PreOrder(root);
    printf("\n");
    printf("inorder:");
    InOrder(root);
    printf("\n");
    return 0;
}