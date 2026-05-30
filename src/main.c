#include "stdbool.h"
#include "raylib.h"
#include "game.h"
#include "coco.h"
#include "player.h"
#include "screens.h"

// ✅ Declarar como global
Font GasoekOne;

int main() {    
    InitWindow(1000, 600, "Vai Catar Coquinho");
    SetTargetFPS(60);
    
    // ✅ Carregar a fonte
    GasoekOne = LoadFont("GasoekOne-Regular.ttf");
    if (GasoekOne.texture.id == 0) {
        TraceLog(LOG_WARNING, "Fonte GasoekOne não carregada!");
        GasoekOne = GetFontDefault();
    }
    
    InitGame();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
            isPaused = !isPaused;
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

    UnloadMusicStream(musicaFundo);
    UnloadSound(ganhouPontos);
    UnloadSound(perdeuPontos);
    UnloadSound(morreu);
    UnloadSound(maisVidas);
    UnloadFont(GasoekOne);
    CloseAudioDevice();
    unloadfont(GasoekOne);
    CloseWindow();
    return 0;
}