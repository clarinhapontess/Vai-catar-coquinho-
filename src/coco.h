#ifndef COCO_H
#define COCO_H

// estrutura para representar o coco //
typedef struct {
    float x;
    float y;
    float speed;
    int radius;
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

#endif