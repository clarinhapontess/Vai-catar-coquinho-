#include "raylib.h"
#include "coco.h"

#include <stdlib.h>

Node *listaCocos = NULL;

// cria um novo coco //
void AddCoco() {

    // Alocação dinâmica //
    Node *novo = malloc(sizeof(Node));

    novo->coco.x = GetRandomValue(50, 750);
    novo->coco.y = GetRandomValue(-1200, -50);

    novo->coco.speed = GetRandomValue(2, 3);
    novo->coco.radius = 20;

    novo->next = listaCocos;
    listaCocos = novo;
}

// inicia os cocos //
void InitCocos() {

    for (int i = 0; i < 3; i++) {

        AddCoco();
    }
}

// atualiza //
void UpdateCocos() {

    Node *atual = listaCocos;

    while (atual != NULL) {

        atual->coco.y += atual->coco.speed;

        if (atual->coco.y > 600) {

            atual->coco.y = GetRandomValue(-400, -30);
            atual->coco.x = GetRandomValue(50, 750);
        }

        atual = atual->next;
    }
}

// desenha //
void DrawCocos() {

    Node *atual = listaCocos;

    while (atual != NULL) {

        DrawCircle(
            atual->coco.x,
            atual->coco.y,
            atual->coco.radius,
            DARKBROWN
        );

        atual = atual->next;
    }
}