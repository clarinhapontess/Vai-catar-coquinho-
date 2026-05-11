#include "game.h"
#include "player.h"
#include "coco.h"
#include "raylib.h"
#include <stdlib.h>

int score = 0;

// Game functions //
void InitGame() {
    InitPlayer();
    InitCocos();
}

void UpdateGame() {

    UpdatePlayer();
    UpdateCocos();

    Node *atual = listaCocos;

    while (atual != NULL) {

        Rectangle playerRect = {
            player.x,
            player.y,
            player.width,
            player.height
        };

        Vector2 cocoCenter = {
            atual->coco.x,
            atual->coco.y
        };

        if (CheckCollisionCircleRec(
            cocoCenter,
            atual->coco.radius,
            playerRect))
        {
            score++; // -> aumenta 1 ponto quando ha colisao

            atual->coco.y = GetRandomValue(-400, -30);
            atual->coco.x = GetRandomValue(50, 750);
        }

        atual = atual->next;
    }
}

void DrawGame() {
    DrawPlayer();
    DrawCocos();

    // mostra o score na tela //
    DrawText(
        TextFormat("Pontos: %i", score),
        20, 
        20, 
        30, 
        DARKGREEN
    );
}