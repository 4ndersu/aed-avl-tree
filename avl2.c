#include <stdio.h>
#include <stdlib.h>

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
                    return root;
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

int main() {
    Node *root = NULL;
    root = insert(root, 2);
    root = insert(root, 10);
    root = insert(root, 1);
    printTree(root, 0);

    root = removeNode(root, 2);
    printTree(root, 0);

    return EXIT_SUCCESS;
}