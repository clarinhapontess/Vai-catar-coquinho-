#include "raylib.h"
#include "coco.h"
#include <stdlib.h>

// lista encadeada
Node *listaCocos = NULL;

// matriz de spawn
int spawnAreas[2][5] = {0};

// escolhe tipo do coco
static void SetCocoType(Coco *coco) {

    int chance = GetRandomValue(1, 100);

    if (chance <= 70) {
        coco->type = 0;
        coco->color = GREEN;
        coco->speed = 2;
    }
    else if (chance <= 90) {
        coco->type = 1;
        coco->color = RED;
        coco->speed = 2;
    }
    else if (chance <= 95) {
        coco->type = 2;
        coco->color = YELLOW;
        coco->speed = 1;
    }
    else {
        coco->type = 3;
        coco->color = BLUE;
        coco->speed = 1;
    }
}

// adiciona coco
void AddCoco() {

    Node *novo = malloc(sizeof(Node));
    if (novo == NULL) return;

    int linha, coluna;

    do {
        linha = GetRandomValue(0, 1);
        coluna = GetRandomValue(0, 4);
    } while (spawnAreas[linha][coluna] == 1);

    spawnAreas[linha][coluna] = 1;

    novo->coco.row = linha;
    novo->coco.col = coluna;

    int larguraArea = 200;
    int alturaArea = 300;

    novo->coco.x = coluna * larguraArea + 100;
    novo->coco.y = -(linha * alturaArea) - GetRandomValue(50, 300);

    novo->coco.radius = 20;

    SetCocoType(&novo->coco);

    novo->next = listaCocos;
    listaCocos = novo;
}

// inicializa
void InitCocos() {
    for (int i = 0; i < 5; i++) {
        AddCoco();
    }
}

// update
void UpdateCocos() {

    Node *atual = listaCocos;

    while (atual != NULL) {

        atual->coco.y += atual->coco.speed;

        if (atual->coco.y > 600) {

            spawnAreas[atual->coco.row][atual->coco.col] = 0;

            int linha, coluna;

            do {
                linha = GetRandomValue(0, 1);
                coluna = GetRandomValue(0, 4);
            } while (spawnAreas[linha][coluna] == 1);

            spawnAreas[linha][coluna] = 1;

            atual->coco.row = linha;
            atual->coco.col = coluna;

            atual->coco.x = coluna * 200 + 100;
            atual->coco.y = -(linha * 300) - GetRandomValue(50, 300);

            SetCocoType(&atual->coco);
        }

        atual = atual->next;
    }
}

// excluir cocos
void ClearCocos() {

    Node *atual = listaCocos;

    while (atual != NULL) {

        Node *temp = atual;
        atual = atual->next;

        free(temp);
    }

    listaCocos = NULL;

    // LIMPA MATRIZ DE SPAWN
    for (int i = 0; i < 2; i++) {

        for (int j = 0; j < 5; j++) {

            spawnAreas[i][j] = 0;
        }
    }
}

// draw
void DrawCocos() {

    Node *atual = listaCocos;

    while (atual != NULL) {

        DrawCircle(
            atual->coco.x,
            atual->coco.y,
            atual->coco.radius,
            atual->coco.color
        );

        atual = atual->next;
    }
}