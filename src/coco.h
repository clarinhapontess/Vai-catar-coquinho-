#ifndef COCO_H
#define COCO_H

#include "raylib.h"

// estrutura para representar o coco //
typedef struct {
    float x;
    float y;
    float speed;
    int radius;
    
    //tipo e cor para diferenciar os cocos //
    int sprite;
    int type;
    Color color;

    // REQUISITO: matriz (posição do coco no grid)
    int row;
    int col;
} Coco;

//Lista encadeada + ponteiros //
typedef struct Node {

    Coco coco;
    struct Node *next; // -> ponteiro para o próximo nó (coco) da lista //

} Node;

// declaração das funções para o coco //
void InitCocos();
void UpdateCocos();
void DrawCocos();
void AddCocos();
void ClearCocos();

extern Node *listaCocos;

// Matriz //
extern int spawnAreas[2][5];

#endif