#include <stdlib.h>
#include <stdio.h>
 
typedef struct treeNode{
   int key;
   struct treeNode *left, *right;
   int height;
}TreeNode;
 
//
//----------------------------------------------------------------------------------
// Protótipos das funções
//----------------------------------------------------------------------------------
//
 
TreeNode* new_node(int num);
int maior_height(int a, int b);
int altura_node(TreeNode *treeNode);
int balance_factor(TreeNode *treeNode);
TreeNode* letf_rotation(TreeNode *root);
TreeNode* right_rotation(TreeNode *root);
TreeNode* rotation_right_left(TreeNode *root);
TreeNode* rotation_left_right(TreeNode *root);
TreeNode* balanced_tree(TreeNode *treeRoot);
TreeNode* insert_node(TreeNode *treeRoot, int x);
TreeNode* remove_node(TreeNode *treeRoot, int x);
void print_avl_tree(TreeNode *treeRoot, int x);
 
//
//----------------------------------------------------------------------------------
// Implementação das funções
//----------------------------------------------------------------------------------
//
 
/*
   Função que cria um novo nó
   num == valor a ser inserido nesse nó  
*/
TreeNode* new_node(int num){
   TreeNode *new = malloc(sizeof(TreeNode));
 
   if(new){
       new->key = num;
       new->left = NULL;
       new->right = NULL;
       new->height = 0;
   }
   else{
       printf("\nErro de alocacao!\n");
   }
   return new;
}
/*
   Função que retorna o maior entre dois valores
   a, b == altura de dois nós da árvore
*/
int maior_height(int a, int b){
   return (a > b)? a : b;
}
/*
   Função que retorna a altura de um nó ou -1 se ele for NULL
*/
int altura_node(TreeNode *treeNode){
   if(treeNode == NULL){
       return -1;
   }
   else{
       return treeNode->height;
   }
}
/*
   Função que calcula o fator de balanceamento de um nó
   Se o fator de balanceamento for negativo a árvore pende à direita, se for positivo a árvore pende à esquerda 
*/
int balance_factor(TreeNode *treeNode){
   if(treeNode){
       return (altura_node(treeNode->left) - altura_node(treeNode->right));
   }
   else{
       return 0;
   }
}
/*
   Função para a rotação a esquerda
*/
TreeNode* letf_rotation(TreeNode *root){
   TreeNode *a, *b;
 
   a = root->right;
   b = a->left;
 
   a->left = root;
   root->right = b;
 
   root->height = maior_height(altura_node(root->left), altura_node(root->right)) + 1;
   a->height = maior_height(altura_node(a->left), altura_node(a->right)) + 1;
 
   return a;
}
/*
   Função para a rotação a direita
*/
TreeNode* right_rotation(TreeNode *root){
   TreeNode *a, *b;
 
   a = root->left;
   b = a->right;
 
   a->right = root;
   root->left = b;
 
   root->height = maior_height(altura_node(root->left), altura_node(root->right)) + 1;
   a->height = maior_height(altura_node(a->left), altura_node(a->right)) + 1;
 
   return a;
}
/*
   Funcao para as rotações duplas quando a sub-arvore pende a direita depois esquerda
*/
TreeNode* rotation_right_left(TreeNode *root){
   root->right = right_rotation(root->right);
   return letf_rotation(root);
}
/*
   Funcao para as rotações duplas quando a sub-arvore pende a esquerda depois direita
*/
TreeNode* rotation_left_right(TreeNode *root){
   root->left = letf_rotation(root->left);
   return right_rotation(root);
}
/*
   Funcao que realiza o balanceamento da árvore após uma insercao ou remocao
*/
TreeNode* balanced_tree(TreeNode *treeRoot){ //recebe o nó que está desbalanceado
   int aux =  balance_factor(treeRoot);
 
   if(aux < -1 && balance_factor(treeRoot->right) <= 0){ //rotacao a esquerda
       treeRoot = letf_rotation(treeRoot);
   }
   else if(aux > 1 && balance_factor(treeRoot->left) >= 0){ //rotacao a direita
       treeRoot = right_rotation(treeRoot);
   }
   else if(aux > 1 && balance_factor(treeRoot->left) < 0){ //rotacao dupla a esquerda
       treeRoot = rotation_left_right(treeRoot);
   }
   else if(aux < -1 && balance_factor(treeRoot->right) > 0){ //rotacao dupla a direita
       treeRoot = rotation_right_left(treeRoot);
   }
 
   return treeRoot; //retorna a nova raiz após o balanceamento
}
/*
   Funcao para inserir um valor à árvore balanceada
   treeRoot == raiz da arvore
   x == valor da chave a ser inserido
   Se a árvore for vazia, cria uma nova árvore com um nó
   Essa funcao retorta o endereço do novo nó ou nova raiz após o balanceamento 
*/
TreeNode* insert_node(TreeNode *treeRoot, int x){
   if(treeRoot == NULL){ //verifica se a arvore é vazia, se a arvore for vazia cria uma nova árvore com um nó
       return new_node(x);
   }
   else{ // verifica se a insercao será a esquerda ou direita
       if(x < treeRoot->key){
           treeRoot->left = insert_node(treeRoot->left, x);
       }else if(x > treeRoot->key){
           treeRoot->right = insert_node(treeRoot->right, x);
       }else{
           printf("\nInsercao nao relizada!\nO elemento %d ja existe!\n", x); //nao é permitido a insercao de elementos iguais
       }
   }
   //recalcula a altura de todos os nós entre a raiz e o novo nó inserido
   treeRoot->height = maior_height(altura_node(treeRoot->left), altura_node(treeRoot->right)) + 1;
 
   //verifica se existe a necessidade de rebalancear a árvore
   treeRoot = balanced_tree(treeRoot); //funcao de balanceamento
 
   return treeRoot;
}
/*
   Funcao para remover um valor da árvore
   X == valor da chave a ser removida
*/
TreeNode* remove_node(TreeNode *treeRoot, int x){
   if(treeRoot == NULL){
       printf("\nEste valor nao pertece a árvore!\n");
       return NULL;
   }
   else{ //realiza a busca do nó a ser removido
       if(treeRoot->key == x){
           //remove nós folhas
           if(treeRoot->left == NULL && treeRoot->right == NULL){
               free(treeRoot);
               printf("\nElemento %d removido com sucesso!\n", x);
               return NULL;
           }
           else{
               //remove nós que possuem dois filhos
               if(treeRoot->left != NULL && treeRoot->right != NULL){
                   TreeNode *aux = treeRoot->left;
                   while(aux->right != NULL){
                       aux = aux->right;
                   }
                   treeRoot->key = aux->key;
                   aux->key = x;
                   treeRoot->left = remove_node(treeRoot->left, x);
                   return treeRoot;
               }
               else{
                   //remove nós que possuem 1 filho
                   TreeNode *aux;
                   if(treeRoot->left != NULL){
                       aux = treeRoot->left;
                   }
                   else{
                       aux = treeRoot->right;
                   }
                   free(treeRoot);
                   printf("\nElemento %d removido com sucesso!\n", x);
                   return aux;
               }
           }
       }
       else{
           if(x < treeRoot->key){
               treeRoot->left = remove_node(treeRoot->left, x);
           }
           else{
               treeRoot->right = remove_node(treeRoot->right, x);
           }
       }
       //recalcula a altura de todos os nós entre a raiz e o novo nó inserido
       treeRoot->height = maior_height(altura_node(treeRoot->left), altura_node(treeRoot->right)) + 1;
 
       //verifica se existe a necessidade de balancear a árvore
       treeRoot = balanced_tree(treeRoot);
 
       return treeRoot;
   }
}
/*
   Funcao para imprimir a arvore
   X == nivel da árvore
*/
void print_avl_tree(TreeNode *treeRoot, int x){
   int i;
   if(treeRoot){
       print_avl_tree(treeRoot->right, x + 1);
       printf("\n\n");
 
       for(i = 0; i < x; i++){
           printf("\t");
       }
       printf("%d", treeRoot->key);
       print_avl_tree(treeRoot->left, x + 1);
   }
}
 
int main(){
   int menu, valor;
   TreeNode *treeRoot = NULL;
 
   printf("\n\t\tARVORE AVL");
 
   while(menu != 1){
       printf("\n\n\t1 - Sair\n\t2 - Inserir Elemento\n\t3 - Remover Elevento\n\t4 - Imprimir Arvore\n\n");
       scanf("%d", &menu);
 
       switch (menu)
       {
       case 1:
           printf("\tSaindo...!\n");
          
           break;
      
       case 2:
           printf("\tDigite o valor a ser inserido na arvore: ");
           scanf("%d", &valor);
           treeRoot = insert_node(treeRoot, valor);
          
           break;
 
       case 3:
           printf("\tDigite o valor a ser reovido da arvore: ");
           scanf("%d", &valor);
           treeRoot = remove_node(treeRoot, valor);
 
           break;
      
       case 4:
           print_avl_tree(treeRoot, 1);
          
           break;
      
       default:
           printf("\tOpcao Invalida\n");
 
           break;
       }
   }
}
