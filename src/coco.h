#ifndef COCO_H
#define COCO_H

#include "raylib.h"

// Estrutura para representar o coco //
// O coco é um item da lista de cocos 
typedef struct coco {
    float x;
    float y;
    float speed;
    int radius;
    
    // Tipo e cor para diferenciar os cocos //
    int sprite;
    int type;
    Color color;

    // REQUISITO: matriz (posição do coco no grid)
    int row;
    int col;
} Coco;

// Lista encadeada + ponteiros //
typedef struct Node {
    Coco coco;
    struct Node *next; // -> ponteiro para o próximo nó (coco) da lista //
} Node;

// Declaração das funções para o coco //
void InitCocos();
void Updatecoco(float deltaTime);
void DrawCocos();
void AddCoco(); // <-- Corrigido aqui para o singular (combinando com o coco.c)
void ClearCocos();

extern Node *listaCocos;

// Matriz //
extern int spawnAreas[2][5];

#endif