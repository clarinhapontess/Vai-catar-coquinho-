#include "raylib.h"
#include "game.h"
#include "coco.h"
#include "player.h"

int main() {
    InitWindow(1000, 600, "Vai Catar Coquinho");
    SetTargetFPS(60);

    InitGame();

    // Loop principal do jogo
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        UpdateGame(deltaTime);

        BeginDrawing();
            DrawGame();
        EndDrawing();
    }

    // --- CORREÇÃO AQUI: Descarregar o áudio ANTES de fechar a janela ---
    UnloadMusicStream(musicaFundo);
    UnloadSound(ganhouPontos);
    UnloadSound(perdeuPontos);
    UnloadSound(morreu);
    UnloadSound(maisVidas);
    CloseAudioDevice();
    
    CloseWindow(); // Sempre por último!

    return 0;
}