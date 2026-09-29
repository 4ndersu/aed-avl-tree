//arquivo ArvoreAVL.h
typedef struct NO* treeAVL;

//arquivo ArvoreAVL.c
#include <stdio.h>
#include <stdlib.h>
//#include "ArvoreAVL.h" 

struct NO {
    int info;
    int heig; //altura da sub-árvore
    struct NO *left;
    struct NO *right;
};

treeAVL* root;

//criando (alocando) um novo nó
struct NO* createNode(int value) {
    struct NO *no;
    no = (struct NO*)malloc(sizeof(struct NO));

    if(no == NULL) {
        return NULL;
    }

    no->info = value;
    no->heig = 0;
    no->left = NULL;
    no->right = NULL;
    return no;
}

//liberando (desalocando) um único nó
void freeNode(struct NO* no) {
    free(no);
}

//liberando (desalocando) toda a árvore recursivamente
void freeTreeAVL(treeAVL *root) {
    if(*root == NULL) {
        return;
    }

    freeTreeAVL(&(*root)->left);
    freeTreeAVL(&(*root)->right);
    freeNode(*root);
    *root = NULL;
}

//calculando a altura de um nó
int heig_NO(struct NO* no) {
    if(no == NULL) {
        return -1;
    } else {
        return no->heig;
    }
}

//calculando o fator de balanceamento de um nó
int balancingFactor(struct NO* no) {
    return labs(heig_NO(no->left) - heig_NO(no->right));
}

//calculando maior valor 
int bigger(int x, int y) {
    if(x > y) {
        return x;
    } else {
        return y;
    }
}

//rotação LL (simples)
void rotationLL(treeAVL *root) {//root -> nó que quero balancear
    struct NO *no;

    //nó auxiliar recebe filho a esquerda
    no =  (*root)->left; 
    //filho a esquerda recebe filho a direita dele
    (*root)->left = no->right;
    //filho a direita recebe a raiz
    no->right = *root;

    //calculando novas alturas 
    (*root)->heig = bigger(heig_NO((*root)->left), heig_NO((*root)->right)) + 1;
    no->heig = bigger(heig_NO(no->left), (*root)->heig) + 1;

    *root = no;
}

//rotação RR (simples)
void rotationRR(treeAVL *root) {
    struct NO *no;

    //nó auxiliar recebe filho a direita
    no = (*root)->right;
    //filho a direita recebe filho a esquerda dele
    (*root)->right = no->left;
    //filho a esquerda recebe a raiz
    no->left = (*root);

    //calculando novas alturas
    (*root)->heig = bigger(heig_NO((*root)->left), heig_NO((*root)->right)) + 1;
    no->heig = bigger(heig_NO(no->right), (*root)->heig) + 1;

    (*root) = no;
}

//rotação LR (dupla | esquerda para direita)
void rotationLR(treeAVL *root) {
    rotationRR(&(*root)->left);
    rotationLL(root);
}

//rotação RL (dupla | direita para esquerda)
void rotationRL(treeAVL *root) {
    rotationLL(&(*root)->right);
    rotationRR(root);
}

//inserção
int insert_TreeAVL(treeAVL *root, int value) {
    int res;

    //caso a árvore esteja vazia (raiz = NULL)
    if(*root == NULL) {
        struct NO *no = createNode(value);

        if(no == NULL) {
            return 0;
        }

        *root = no;
        return 1;
    }

    //caso não esteja vazia | current = atual -> recebe o valor da raiz
    struct NO *current = *root;
    //caso o value seja menor do que o nó atual, será inserido na esquerda
    if(value < current->info) {
        //se a inserção for igual a 1 ela funcionou
        if((res=insert_TreeAVL(&(current->left), value)) == 1) {
            //verificando e rebalanceando nó se necessário
            if(balancingFactor(current) >= 2) {
                if(value < (*root)->left->info) {
                    rotationLL(root);
                } else {
                    rotationLR(root);
                }
            }
        } 
    } 
    //caso o value seja menor do que o nó atual, será inserido na direita
    else if (value > current->info) {
        if((res=insert_TreeAVL(&(current->right), value)) == 1) {
            if(balancingFactor(current) >= 2) {
                if((*root)->right->info < value) {
                    rotationRR(root);
                } else {
                    rotationRL(root);
                }
            }
        } else {
            //nessa implementação não aceita valor duplicado
            printf("valor duplicado");
            return 0;
        }
    } else {
        printf("valor duplicado.");
        return 0;
    }

    current->heig = bigger(heig_NO(current->left), heig_NO(current->right)) + 1;
    return 1;
}

struct NO* smallest(struct NO* current) {
    struct NO *no1 = current;
    struct NO *no2 = current->left;

    //enquanto o nó 2 for diferente de NULL, anda cada vez mais a esquerda
    while(no2 != NULL) {
        no1 = no2;
        no2 = no2->left;
    }
    return no1;
}

//*remoção de um nó
int remove_TreeAVL(treeAVL *root, int value) {
    //caso a raiz não exista
    if(*root == NULL) {
        printf("valor não existe.");
        return 0;
    }


    int res = 0; //resposta da remoção
    //caso o value seja menor do que o nó atual, será removido na subarvore a esquerda
    if(value < (*root)->info) {
        //se o retorno for 1, ou seja, conseguiu remover, então fazemos o rebalanciamento 
        if((res=remove_TreeAVL(&(*root)->left, value)) == 1) {
            //atualizando a altura do nó
            (*root)->heig = bigger(heig_NO((*root)->left), heig_NO((*root)->right)) + 1;

            if(balancingFactor(*root) >= 2) {
                if(heig_NO((*root)->right->left) <= heig_NO((*root)->right->right)) {
                    rotationRR(root);
                } else {
                    rotationRL(root);
                }
            }
        }
    }
    //caso o value seja maior do que o nó atual, será removido na subarvore a direita
    else if((*root)->info < value) {
        if((res=remove_TreeAVL(&(*root)->right, value)) == 1) {
            (*root)->heig = bigger(heig_NO((*root)->left), heig_NO((*root)->right)) + 1;

            if(balancingFactor(*root) >= 2) {
                if(heig_NO((*root)->left->right) <= heig_NO((*root)->left->left)) {
                    rotationLL(root);
                } else {
                    rotationLR(root);
                }
            }
        }
    }

    //se forem iguais
    else {
        //nó pai tem 1 ou nenhum filho
        if(((*root)->left == NULL || (*root)->right == NULL)) {
            struct NO *oldNode = (*root);

            //verificando qual nó é NULL ou se os dois são
            if((*root)->left != NULL) {
                *root = (*root)->left;
            } else {
                *root = (*root)->right;
            }
            freeNode(oldNode);
        } else { //nó pai tem dois filhos
            //procurando o menor
            struct NO* temp = smallest((*root)->right);
            (*root)->info = temp->info;
            //chamando a remoção recursivamente
            remove_TreeAVL(&(*root)->right, (*root)->info);

            //atualizando a altura do nó
            (*root)->heig = bigger(heig_NO((*root)->left), heig_NO((*root)->right)) + 1;

            //balanceando
            if(balancingFactor(*root) >= 2) {
                if(heig_NO((*root)->left->right) <= heig_NO((*root)->left->left)) {
                    rotationLL(root);
                } else {
                    rotationLR(root);
                }
            }
            return 1;
        }
        return 1;
    }

    return res;
}

//funções de print feitas pelo gemini
void print2DUtil(struct NO *root, int space) {
    if (root == NULL) return;
    
    space += 5;
    
    print2DUtil(root->right, space);
    
    printf("\n");
    for (int i = 5; i < space; i++) {
        printf(" ");
    }
    printf("%d(h:%d)\n", root->info, root->heig);
    
    print2DUtil(root->left, space);
}

void mostrarArvore(treeAVL *root) {
    printf("\n[ Estrutura Atual da Arvore (Deitada) ]\n");
    if (*root == NULL) {
        printf("  (Arvore Vazia)\n");
    } else {
        print2DUtil(*root, 0);
        printf("\n");
    }
    printf("-----------------------------------------\n");
}

int main() {
    treeAVL raiz = NULL;

    printf("=========== TESTE ARVORE AVL ===========\n");

    printf("\n1. Inserindo 10, 20, 30 (Forcando Rotacao RR):");
    insert_TreeAVL(&raiz, 10);
    insert_TreeAVL(&raiz, 20);
    insert_TreeAVL(&raiz, 30);
    mostrarArvore(&raiz);

    printf("\n2. Inserindo 5, 3 (Forcando Rotacao LL):");
    insert_TreeAVL(&raiz, 5);
    insert_TreeAVL(&raiz, 3);
    mostrarArvore(&raiz);

    printf("\n3. Inserindo 25, 27 (Testando Rotacao RL):");
    insert_TreeAVL(&raiz, 25);
    insert_TreeAVL(&raiz, 27);
    mostrarArvore(&raiz);

    printf("\n4. Removendo 20 (No interno com filhos):");
    remove_TreeAVL(&raiz, 20);
    mostrarArvore(&raiz);

    printf("\n5. Removendo 3 (No folha):");
    remove_TreeAVL(&raiz, 3);
    mostrarArvore(&raiz);

    printf("================ FIM ===================\n");
    
    freeTreeAVL(&raiz);

    return EXIT_SUCCESS;
}