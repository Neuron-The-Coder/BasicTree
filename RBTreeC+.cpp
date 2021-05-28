#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ll long long int
#define ull unsigned long long int
#define pi 3.14159

#define RED 0
#define BLACK 1
#define DOUBLE 2

struct Node {
  int nums;
  int color;
  
  Node *left;
  Node *right;
  Node *parent;
};

Node *recent = NULL; //Node yang baru saja diinsert
Node *upper = NULL; //Node pucuk

Node *newNode(int nums){
  Node* temp = (Node *) malloc (sizeof(Node));
  temp->nums = nums;
  temp->color = RED;

  temp->parent = temp->left = temp->right = NULL;
  recent = temp;
  return temp;
}

Node* Push(Node* root, int nums){
  if (!root) return newNode(nums);
  else if (nums < root->nums){
    root->left = Push(root->left, nums);
    root->left->parent = root;
  }
  else if (nums > root->nums){
    root->right = Push(root->right, nums);
    root->right->parent = root;
  }
  return root;
}

void InOrder(Node *root, int space){
  if (root){
    InOrder(root->right, space + 6);
    
    for (int i=0; i<space; i++) printf(" ");
    int c;
    
    if (root->color == RED) c = 'R';
    else if (root->color == BLACK) c = 'B';
    else c = 'D'; 
    
    printf("| %d-%c |\n", root->nums, c);
    
    InOrder(root->left, space + 6);
  }
}

void PrintTree(Node* upper){
  puts("========================================");
  InOrder(upper, 0);
  puts("========================================");
}

///////////////////////////////////////////////////////////////////
// FIX INSERT

Node *Rotate(Node* oldRoot, char code){
  
  Node* parents = oldRoot->parent;
  
  char parto = 'L';
  if (parents && parents->right == oldRoot) parto = 'R';  
  
  Node* newRoot = NULL;
  Node* temp = NULL;
  
  if (code == 'R'){
     newRoot = oldRoot->left;
     temp = newRoot->right;
     
     newRoot->right = oldRoot;
     oldRoot->left = temp;
  }
  else {
    newRoot = oldRoot->right;
    temp = newRoot->left;
    
    newRoot->left = oldRoot;
    oldRoot->right = temp;
  }
  
  oldRoot->parent = newRoot;
  newRoot->parent = parents;
  
  if (parents) (parto == 'L') ? parents->left = newRoot : parents->right = newRoot; 
  
  return newRoot;
}


void FixInsert(Node *root){
  // Misal dia dah di pucuk
  
  if (!root->parent){
    upper = root;
    upper->color = BLACK;
    return;
  }
  else if (!root->parent->parent) {
    FixInsert(root->parent);
    return;
  }
  
  Node *rootP = root->parent;
  Node *rootGP = root->parent->parent;
  Node *rootU = NULL;
  
  if (root->color == BLACK || root->parent->color == BLACK) return;
  
  // Kasus berbeda tergantung arah uncle
     // Dipecah berdasarkan warna si uncle
  if (rootGP->left == root->parent){
    rootU = rootGP->right;
    
    if (rootU && rootU->color == RED){
      rootGP->color = RED;
      if (rootGP->right) rootGP->right->color = BLACK;
      rootGP->left->color = BLACK;
    }
    
    else {
      
      if (rootGP->left->right == root) rootP = Rotate(rootP, 'L');
      rootGP = Rotate(rootGP, 'R');
      
      //JANGAN LUPA RECOLOR
      rootGP->color = BLACK;
      if (rootGP->right) rootGP->right->color = RED;
      rootGP->left->color = RED;
    }
    
  }
  
  else if (rootGP->right == root->parent) {
    rootU = rootGP->left;
    
    if (rootU && rootU->color == RED){
      rootGP->color = RED;
      if (rootGP->left) rootGP->left->color = BLACK;
      rootGP->right->color = BLACK;
    }
    
    else {
      
      if (rootGP->right->left == root) rootP = Rotate(rootP, 'R');
      rootGP = Rotate(rootGP, 'L');
      
      //JANGAN LUPA RECOLOR
      rootGP->color = BLACK;
      if (rootGP->left) rootGP->left->color = RED;
      rootGP->right->color = RED; 
    }
  }
  
  FixInsert(rootGP);
  
}

///////////////////////////////////////////////////////////////////

int main(){
    
  int s = 9;
  int num[] = {16, 27, 9, 11, 36, 54, 81, 63, 72};
    
  for (int i=0; i<9; i++){
    upper = Push(upper, num[i]);
    FixInsert(recent);
    PrintTree(upper);
  }
    

    
return 0;
}

