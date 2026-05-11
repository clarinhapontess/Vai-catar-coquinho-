#include "game.h"
#include "player.h"
#include "coco.h"

// Game functions //
void InitGame() {
    InitPlayer();
    InitCocos();
}

void UpdateGame() {
    UpdatePlayer();
    UpdateCocos();
}

void DrawGame() {
    DrawPlayer();
    DrawCocos();
}