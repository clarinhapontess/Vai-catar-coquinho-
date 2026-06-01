#ifndef COCO_H
#define COCO_H

#include "raylib.h"

// estrutura do coco
typedef struct coco {
    float x;
    float y;
    float speed;
    int radius;
    
    // tipo dos cocos
    int sprite;
    int type;
    Color color;

    int row;
    int col;
} Coco;
// no para a lista cocos
typedef struct Node {
    Coco coco;
    struct Node *next; 
} Node;
// funções do coco
void InitCocos();
void Updatecoco(float deltaTime);
void DrawCocos();
void AddCoco(); 
void ClearCocos();

extern Node *listaCocos;
extern int spawnAreas[2][5];

#endif