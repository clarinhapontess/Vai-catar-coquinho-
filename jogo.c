#include "raylib.h"

int main() {
    InitWindow(800, 450, "Teste Raylib");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Raylib funcionando!", 200, 200, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}