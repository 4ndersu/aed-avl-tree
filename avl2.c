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

short heigNode(Node *node) {
    if(node == NULL) {
        return -1;
    } else {
        return node->heig;
    }
}