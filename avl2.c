#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct node {
    int value;
    struct node *left, *right;
    short heig;
}Node;

Node* newNode(int x) {
    Node *new = malloc(sizeof(Node));

    if(new) {
        new->value = x;
        new->left = NULL;
        new->right = NULL;
        new->heig = 0;
    } else {
        printf("\nerro ao alocar um novo nó.");
    }
    
    return new;
}

//descobre a altura da subarvores a esquerda e direita e compara os valores
short bigger(short a, short b) {
    return (a > b)? a:b;
}

//dado um nó ela retorna a altura dele
short heigNode(Node *node) {
    if(node == NULL) {
        return -1;
    } else {
        return node->heig;
    }
}

//calcular e retornar o fator de balanceamento de um nó
short balancingFactor(Node *node) {
    if(node) {
        return (heigNode(node->left) - heigNode(node->right));
    } else {
        return 0;
    }
}

//função de rotação a esquerda
Node* leftRotation(Node *r) {
    Node *y, *f;

    y = r->right;
    f = y->left;

    y->left = r;
    r->right = f;

    //recauculando altura após a rotação
    r->heig = bigger(heigNode(r->left), heigNode(r->right)) + 1;
    y->heig = bigger(heigNode(y->left), heigNode(y->right)) + 1;

    return y;
}

//função de rotação a direita
Node* rightRotation(Node *r) {
    Node *y, *f;

    y = r->left;
    f = y->right;

    y->right = r;
    r->left = f;

    //recauculando altura após a rotação
    r->heig = bigger(heigNode(r->left), heigNode(r->right)) + 1;
    y->heig = bigger(heigNode(y->left), heigNode(y->right)) + 1;

    return y;
}

//Rotação dupla direita esquerda
Node* rightLeftRotation(Node *r) {
    r->right = rightRotation(r->right);
    return leftRotation(r);
}

//Rotação dupla esquerda direita
Node* leftRightRotation(Node *r) {
    r->left = leftRotation(r->left);
    return rightRotation(r);
}

Node* balance(Node *root) {
    short fb = balancingFactor(root);

    //o fator de balanceamento decide como a árvore vai rotacionar

    //rotação a esquerda
    if(fb < -1 && balancingFactor(root->right) <= 0) {
        root = leftRotation(root);
    } 
    //rotação a direita
    else if(fb > 1 && balancingFactor(root->left) >= 0){
        root = rightRotation(root);
    } 
    //rotação dupla a esquerda
    else if(fb > 1 && balancingFactor(root->left) < 0) {
        root = leftRightRotation(root);
    } 
    //rotação dupla a direita
    else if(fb < -1 && balancingFactor(root->right) > 0) {
        root = rightLeftRotation(root);
    }

    return root;
}

/*
    Insere o novo nó na árvore
    raiz -> raiz da árvore
    x -> valor a ser inserido
*/

Node* insert(Node *root, int x) {
    if(root == NULL) {
        return newNode(x);
    } else {
        if(x < root->value) {
            root->left = insert(root->left, x);
        } else if(x > root->value) {
            root->right = insert(root->right, x);
        } else {
            printf("\ninserção não realizada.");
        }
    }

    //recalcula a altura de todos os nós entre a raiz e o novo nó inserido
    root->heig = bigger(heigNode(root->left), heigNode(root->right)) + 1;

    root = balance(root);

    return root;
}

//remoção de um nó
Node* removeNode(Node *root, int key) {
    if(root == NULL) {
        printf("valor não encontrado\n");
        return NULL;
    } else { //procura um nó para remover
        if(root->value == key) {
            //remover nós folhas (nós sem filhos)
            if(root->left == NULL && root->right == NULL){
                free(root);
                printf("elemento folha removido: %d !\n", key);
                return NULL;
            }
            else {
                //remoção de nós com dois filhos
                if(root->left != NULL && root->right != NULL) {
                    Node *aux = root->left;
                    while(aux->right != NULL) {
                        aux = aux->right;
                    }
                    root->value = aux->value;
                    aux->value = key;
                    printf("elemento trocado: %d !\n", key);
                    root->left = removeNode(root->left, key);

                    //recalculando altura e chamando balance
                    root->heig = bigger(heigNode(root->left), heigNode(root->right)) + 1;
                    return balance(root);
                } else {
                    //remoção de nós com só um filho
                    Node *aux;
                    if(root->left != NULL) {
                        aux = root->left;
                    } else {
                        aux = root->right;
                    }
                    free(root);
                    printf("elemento com 1 filho removido: %d !\n", key);
                    return aux;
                }
            }
        } else {
            if(key < root->value) {
                root->left = removeNode(root->left, key);
            } else {
                root->right = removeNode(root->right, key);
            }
        }

        root->heig = bigger(heigNode(root->left), heigNode(root->right)) + 1;

        root = balance(root);

        return root;
    }
}

void printTree(Node *root, int level) {
    if(root == NULL) return;

    printTree(root->right, level + 1);

    for(int i = 0; i < level; i++) printf("    ");
    printf("%d\n", root->value);

    printTree(root->left, level + 1);
}

/*------------------------------------------------------------------------------
 * Permutação de um Arranjo
 *
 * Implementan o algoritmo iterativo de Narayana Pandita para a permutação de
 * um arranjo em ordem lexicográfica.
 */

 //Função auxiliar para trocar dois elementos de um arranjo
void swap(int *a, int *b)
{
    const int temp = *a;
    *a = *b;
    *b = temp;
}

//Função auxiliar para inverter um arranjo entre os índices inicio e fim
void perm_invert(int *arr, int inicio, int fim)
{
    while (inicio < fim) {
        swap(&arr[inicio], &arr[fim]);
        inicio++;
        fim--;
    }
}

//Função que gera a próxima permutação lexicográfica de um arranjo
bool perm_next(int *arr, int tamanho)
{
    int i = tamanho - 2;

    while (i >= 0 && arr[i] >= arr[i + 1]) {
        i--;
    }

    if (i < 0) {
        return false;
    }

    int j = tamanho - 1;
    while (arr[j] <= arr[i]) {
        j--;
    }

    swap(&arr[i], &arr[j]);

    perm_invert(arr, i + 1, tamanho - 1);

    return true;
}

/*------------------------------------------------------------------------------
 * Funções Auxiliares
 */

 //Função que imprime os elementos de um arranjo
void data_print(const int * const data, const int N)
{
    printf("data: [ ");
    for (int i = 0; i < N; i++) {
        printf("%02d ", data[i]);
    }
    printf("]\n");
}

//Função que remove todas as ocorrências de um valor em um arranjo e retorna o novo tamanho do arranjo
int arr_remove(int *arr, int N, int value)
{
    for (int i = 0; i < N; i++) {
        if (arr[i] == value) {
            for (int j = i; j < N - 1; j++) {
                arr[j] = arr[j + 1];
            }
            N--;
            i--;
        }
    }

    return N;
}

int main() {
    Node *root = NULL;
    root = insert(root, 2);
    root = insert(root, 10);
    root = insert(root, 1);
    root = insert(root, 3);
    root = insert(root, 4);
    root = insert(root, 5);
    printTree(root, 0);

    root = removeNode(root, 2);
    printTree(root, 0);

    /*    int DATA_INSERT[] = {1, 2, 3, 4, 5}; // DEVE estar ordenado!
    int DATA_REMOVE[] = {1, 2, 3, 4, 5}; // DEVE estar ordenado!
    const int N = 5; // tamanho dos arranjos de inserção e remoção

    int *data_insert, *data_remove;

    data_insert = (int *)malloc(sizeof(int) * N);
    memcpy(data_insert, DATA_INSERT, sizeof(int) * N);
    
    do { // Loop de Inserção
    
        data_remove = (int *)malloc(sizeof(int) * N);
        memcpy(data_remove, DATA_REMOVE, sizeof(int) * N);

        do { // Loop de Remoção
            BST *T = bst_alloc();

            printf("--------------------------------------------\n");
            printf("Dados para Insercao:\n\t");
            data_print(data_insert, N);
            
            for (int i = 0; i < N; i++) {
                printf("Inserindo: %02d\n", data_insert[i]);
                Node *nd = node_alloc(data_insert[i]);
                bst_insert(T, nd);
                bst_print(T);
                assert(bst_check(T->root, data_insert, i + 1));
            }
            
            printf("Arvore apos todas as INSERCOES:\n");
            bst_printTree(T->root);

            printf("Dados para Remocao:\n\t");
            data_print(data_remove, N);
            int *arr = (int *)malloc(sizeof(int) * N);
            int asize = N;
            memcpy(arr, data_insert, sizeof(int) * asize);
            
            for (int i = 0; i < N; i++) {
                printf("Removendo: %02d\n", data_remove[i]);
                bst_delete(T, bst_search(T->root, data_remove[i]));
                bst_print(T);
                asize = arr_remove(arr, asize, data_remove[i]);
                assert(bst_check(T->root, arr, asize));
            }
            
            printf("Arvore apos todas as REMOCOES:\n");
            bst_printTree(T->root);

            free(arr);
            bst_free(T);

        } while (perm_next(data_remove, N));
        
        free(data_remove);

    } while (perm_next(data_insert, N));
    
    free(data_insert); 
    */

    return EXIT_SUCCESS;
}
