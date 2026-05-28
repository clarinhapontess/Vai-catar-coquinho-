#include "raylib.h"

// Declarações externas das funções e variáveis do jogo
extern Font gameOverFont;
extern Music musicaFundo;
extern Sound ganhouPontos, perdeuPontos, morreu, maisVidas;

extern void InitGame(void);
extern void UpdateGame(float dt);
extern void DrawGame(void);

int main(void)
{
    // Inicialização da janela
    InitWindow(1000, 600, "Vai Catar Coquinho");
    SetTargetFPS(60);

    InitGame();

    // Carregamento da fonte
    gameOverFont = LoadFontEx("Fredoka-VariableFont_wdth,wght.ttf", 80, NULL, 0);

    // Loop principal do jogo
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        UpdateGame(dt);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawGame();
        EndDrawing();
    }

    // Descarregamento na ordem correta
    UnloadFont(gameOverFont);
    UnloadMusicStream(musicaFundo);
    UnloadSound(ganhouPontos);
    UnloadSound(perdeuPontos);
    UnloadSound(morreu);
    UnloadSound(maisVidas);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}