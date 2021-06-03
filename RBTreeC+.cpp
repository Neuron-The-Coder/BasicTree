/*
Bugs? Please contact me via Discord
DoctorNeuron#6979

Special Thanks for GeeksForGeeks for the pseudocode and the example :)
*/

#include <stdio.h>
#include <stdlib.h>
#define ll long long int
#define ull unsigned long long int
#define pi 3.14159
#define RED 0
#define BLACK 1

void Print();

struct Node{
  int num;
  int color;
  
  Node *parent;
  Node *left;
  Node *right;
};

Node* upper = NULL;
Node* recent = NULL;

Node* newNode(int num){
  Node* temp = (Node*) malloc (sizeof(Node));
  temp->num = num;
  temp->left = temp->right = temp->parent = NULL;
  temp->color = RED;
  
  recent = temp;
  
  return temp; 
}

////////////////////////////////////////////////////////////////////
// FIX INSERTION

//Rotate sudah langsusng memperbaiki hubungan parent dengan child
Node* Rotate(Node* oldRoot, char code){
  
  Node* parents = oldRoot->parent;
  char parto = (parents && parents->left == oldRoot) ? 'L' : 'R'; // ARAH PARENT KE OLDROOT 
  Node* newRoot = NULL;
  Node* temp = NULL;
  
  if (code == 'L'){
    newRoot = oldRoot->right;
    temp = newRoot->left;
    
    newRoot->left = oldRoot;
    oldRoot->right = temp;  
  }
  else if (code == 'R'){
    newRoot = oldRoot->left;
    temp = newRoot->right;
    
    newRoot->right = oldRoot;
    oldRoot->left = temp;
  }
  
  oldRoot->parent = newRoot;
  newRoot->parent = parents;
  if (temp) temp->parent = oldRoot;
  if (parents) (parto == 'L') ? parents->left = newRoot : parents->right = newRoot;
  
  if (oldRoot == upper) upper = newRoot;
  
  return newRoot;
}

void Fix(Node *root){
  
  if (!root->parent){
    upper = root;
    upper->color = BLACK;
    return;
  }
  else if (!root->parent->parent){
    Fix(root->parent);
    return;
  }
  
  Node* rootGP = root->parent->parent;
  Node* rootP = root->parent;
  Node* rootU = NULL;
//  printf("Fixing %d with par and parpar %d %d\n", root->num, root->parent->num, root->parent->parent->num);
  
  if (root->color != RED || root->parent->color != RED) return;
  
  // Uncle kiri
  if (rootGP->left && rootGP->left == rootP){
    rootU = rootGP->right;
    
    //RED dan ADA
    if (rootU && rootU->color == RED){
      rootGP->color = RED;
      if (rootGP->left) rootGP->left->color = BLACK;
      if (rootGP->right) rootGP->right->color = BLACK;
    }
    
    //BLACK
    else {
      
      if (root == rootP->right){
        rootP = Rotate(rootP, 'L');
      }
      rootGP = Rotate(rootGP, 'R');
      
      rootGP->color = BLACK;
      if (rootGP->left) rootGP->left->color = RED;
      if (rootGP->right) rootGP->right->color = RED;
      
    }
  }   
  
  // Uncle kanan
  else if (rootGP->right && rootGP->right == rootP){
    rootU = rootGP->left;
    
    //RED
    if (rootU && rootU->color == RED){
      rootGP->color = RED;
      if (rootGP->left) rootGP->left->color = BLACK;
      if (rootGP->right) rootGP->right->color = BLACK;
    }
    
    //BLACK
    else {
      
      if (root == rootP->left){
        rootP = Rotate(rootP, 'R');
      }
      rootGP = Rotate(rootGP, 'L');
      
      rootGP->color = BLACK;
      if (rootGP->left) rootGP->left->color = RED;
      if (rootGP->right) rootGP->right->color = RED; 
    } 
  }
  Fix(rootGP);
  
}

////////////////////////////////////////////////////////////////////
// FIX DELETION

void FixPop(Node* x){
  // Di root
  if (x->parent == NULL){
    upper = x;
    return;
  }
  
  Node *s = (x->parent->left == x) ? x->parent->right : x->parent->left;
  if (!s) FixPop(x->parent);
  
  
  else if (s->color == RED){
    s->parent->color = RED;
    s->color = BLACK;
    
    if (x->parent->left == x) Rotate(x->parent, 'L');
    else Rotate(x->parent, 'R');
    FixPop(x);
  }
  
  else if (s->color == BLACK){
    
    // Kasus kl sekarang arahnya beda
    if (s->right && s->right->color == RED && x->parent->left == x){
      s->color = s->parent->color;
      s->parent->color = BLACK;
      s->right->color = BLACK;
      Rotate(s->parent, 'L');
      return;
    }
    else if (s->left && s->left->color == RED && x->parent->right == x){
      s->color = s->parent->color;
      s->parent->color = BLACK;
      s->left->color = BLACK;
      Rotate(s->parent, 'R');
      return;
    }
    
    // Kasus ada red dan sibling->red sejajar parent->x
    else if (s->left && s->left->color == RED && x->parent->left == x){
      s->color = RED;
      s->left->color = BLACK;
      Rotate(s, 'R');
      FixPop(x);
    }
    else if (s->right && s->right->color == RED && x->parent->right == x){
      s->color = RED;
      s->right->color = BLACK;
      Rotate(s, 'L');
      FixPop(x);
    }
  
    //Kasus dua anak sibling hitam
    else {
      s->color = RED;
      if (x->parent->color == RED){
        x->parent->color = BLACK;
        return;
      }
      else FixPop(x->parent);
    }
      
  }
  return;
}

Node* Push (Node* root, int num){
  if (!root) return newNode(num);
  else if (num < root->num){
    root->left = Push(root->left, num);
    root->left->parent = root;
  }
  else if (num > root->num){
    root->right = Push(root->right, num);
    root->right->parent = root;
  }
  return root;
}

void Pop(Node* root, int num){
  if (!root) return;
  else if (num < root->num) Pop(root->left, num);
  else if (num > root->num) Pop(root->right, num);
  else{
    
    // 0 Anak
    // Kasus ada node terakhir dan bukan
    if (!root->left && !root->right){
      
      // Node terakhir
      if (root->parent == NULL){
        free(root);
        upper = NULL;
        return;
      }
      
      // 
      else if (root->color == BLACK){
        FixPop(root);
      }
      
      // Putuskan relasi
      if (root->parent->left == root) root->parent->left = NULL;
      else root->parent->right = NULL;
      return;
    }
    
    // 1 Anak
    else if (!root->left || !root->right){
      Node *subs = root->left;
      if (!subs) subs = root->right;
      
      if (root->parent == NULL){
        *upper = *subs;
        upper->color = BLACK;
        free(subs);
        return;
      }
      
      // Buat referensi baru, nguli
      root->num = subs->num;
      int subsC = subs->color;
      if (root->left == subs) root->left = NULL;
      else root->right = NULL;
      free(subs);
      
      // Cek, ini untuk si BLACK-RED (RED-BLACK ga mungkin)
      if (subsC == RED) root->color = BLACK;
      else FixPop(root); // Passing Double Black
      return;
    }
    else {
      Node *subs = root->left;
      while (subs->right) subs = subs->right;
      
      root->num = subs->num;
      Pop(subs, subs->num);
      return;
    }
  }
}

void SpecialOrder(Node* root, int s){
  if (root){
    SpecialOrder(root->right, s+6);
    for (int i=0; i<s; i++) printf(" ");
    char a = (root->color == RED) ? 'R' : 'B';
    
    printf("%d-%c-", root->num, a);
    if (root->parent) printf("%d\n", root->parent->num);
    else printf("OOF\n");
    SpecialOrder(root->left, s+6);
  }
  
}

void Print(){
  puts("=================================================");
  SpecialOrder(upper, 0);
  puts("=================================================");
}

int main(){
  
  int insert[] = {16, 27, 9, 11, 36, 54, 81, 63, 72};
  for (int i=0; i<9; i++){
    upper = Push(upper, insert[i]);
    Fix(recent);
    Print();
  }
  
  Pop(upper, 72);
  Pop(upper, 9);
  Pop(upper, 11);
  Pop(upper, 36);
  Pop(upper, 16);
  Pop(upper, 81);
  
  
return 0;
}

