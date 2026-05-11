#include "raylib.h"
#include "game.h"
#include "coco.h"
#include "player.h"

int main() {
    InitWindow(900, 600, "Vai Catar Coquinho"); //tamanho da tela//
    SetTargetFPS(60); // resolução do jogo//

    InitGame();

    // Loop principal do jogo //
    while (!WindowShouldClose()) {

        UpdateGame();

        BeginDrawing();
            ClearBackground(SKYBLUE);

            DrawGame();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}