#include "raylib.h"
#ifndef COCO_H
#define COCO_H

// Estrutura para representar o coco //
typedef struct coco {
    float x;
    float y;
    float speed;
    int radius;
    
    // Tipos dos cocos //
    int sprite;
    int type;

    // Matriz (posição do coco no grid) //
    int row;
    int col;
} Coco;

// Lista encadeada + ponteiros //
typedef struct Node {
    Coco coco;
    struct Node *next; // Ponteiro para o próximo coco da lista //
} Node;

// Declaração das funções para o coco //
void InitCocos();
void Updatecoco(float deltaTime);
void DrawCocos();
void AddCoco();
void ClearCocos();

extern Node *listaCocos;

// Criando a matriz //
extern int spawnAreas[2][5];

#endif