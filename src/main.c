#include "stdbool.h"
#include "raylib.h"
#include "game.h"
#include "coco.h"
#include "player.h"
#include "screens.h"

// Criação e alocação real das fontes globais
Font GasoekOne;
Font Rubik;

int main() {    
    InitWindow(1000, 600, "Vai Catar Coquinho");
    SetTargetFPS(60);
    
    // Lista unificada de acentos e caracteres
    const char *caracteresSuportados = 
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789.,!?-+=_();:<> /"
        "áàâãéèêíïóòôõúüçÁÀÂÃÉÈÊÍÏÓÒÔÕÚÜÇ";

    int contagemCodepoints = 0;
    int *codepoints = LoadCodepoints(caracteresSuportados, &contagemCodepoints);
    
    // Carrega as duas fontes do jeito certo com suporte a acentos
    GasoekOne = LoadFontEx("assets/fonts/GasoekOne-Regular.ttf", 64, codepoints, contagemCodepoints);
    Rubik = LoadFontEx("assets/fonts/Rubik-Bold.ttf", 48, codepoints, contagemCodepoints);
    
    UnloadCodepoints(codepoints);

    // Proteção caso as fontes sumam da pasta
    if (GasoekOne.texture.id == 0) GasoekOne = GetFontDefault();
    if (Rubik.texture.id == 0) Rubik = GetFontDefault();
    
    // Inicializa o jogo e as telas da história
    InitGame();
    InitScreens();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
            isPaused = !isPaused;
            if (isPaused) {
                PauseMusicStream(musicaFundo);
            } else {
                ResumeMusicStream(musicaFundo);
            }
        }

        float deltaTime = GetFrameTime();
        UpdateGame(deltaTime);

        BeginDrawing();
            DrawGame();
        EndDrawing();
    }

    // Finalizações e descarregamento
    UnloadMusicStream(musicaFundo);
    UnloadSound(ganhouPontos);
    UnloadSound(perdeuPontos);
    UnloadSound(morreu);
    UnloadSound(maisVidas);
    
    UnloadScreens();

    UnloadFont(GasoekOne);
    UnloadFont(Rubik);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}