
#include "stdbool.h"
#include "raylib.h"
#include "game.h"
#include "coco.h"
#include "player.h"
int main() {    
    InitWindow(1000, 600, "Vai Catar Coquinho");
    SetTargetFPS(60);
    Font GasoekOne = LoadFont("resources/GasoekOne.ttf");
    if (GasoekOne.texture.id == 0) {
        TraceLog(LOG_WARNING, "Fonte GasoekOne não carregada!");
        GasoekOne = GetFontDefault();
    }
    InitGame();

    // Loop principal do jogo
    while (!WindowShouldClose()) {
             if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
            isPaused = !isPaused;
            if (isPaused) {
                PauseMusicStream(musicaFundo);
            } else {
                ResumeMusicStream(musicaFundo);
            }
            if (isPaused) {
                PauseMusicStream(musicaFundo);
                TraceLog(LOG_INFO, "Jogo pausado");
            } else {
                ResumeMusicStream(musicaFundo);
                TraceLog(LOG_INFO, "Jogo retomado");
            }
        }

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
    UnloadFont(GasoekOne);
    CloseAudioDevice();
    CloseWindow(); // Sempre por último
    return 0;
}