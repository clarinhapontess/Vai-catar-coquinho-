#include "game.h"
#include "player.h"
#include "coco.h"
#include "raylib.h"
#include <stdlib.h>

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
            atual->coco.y = GetRandomValue(-400, -30);
            atual->coco.x = GetRandomValue(50, 750);
        }

        atual = atual->next;
    }
}

void DrawGame() {
    DrawPlayer();
    DrawCocos();
}