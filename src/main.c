#include "stdbool.h"
#include "raylib.h"
#include "game.h"
#include "coco.h"
#include "player.h"
#include "screens.h"

// fontes
Font GasoekOne;
Font Rubik;

int main() {    
    InitWindow(1000, 600, "Vai Catar Coquinho");
    SetTargetFPS(60);
    
    const char *caracteresSuportados = 
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789.,!?-+=_();:<> /"
        "áàâãéèêíïóòôõúüçÁÀÂÃÉÈÊÍÏÓÒÔÕÚÜÇ";

    int contagemCodepoints = 0;
    int *codepoints = LoadCodepoints(caracteresSuportados, &contagemCodepoints);
    
    // carrega fontes
    GasoekOne = LoadFontEx("assets/fonts/GasoekOne-Regular.ttf", 64, codepoints, contagemCodepoints);
    Rubik = LoadFontEx("assets/fonts/Rubik-Bold.ttf", 48, codepoints, contagemCodepoints);
    
    UnloadCodepoints(codepoints);

    if (GasoekOne.texture.id == 0) GasoekOne = GetFontDefault();
    if (Rubik.texture.id == 0) Rubik = GetFontDefault();
    
    // inicializa jogo
    InitGame();
    InitScreens();

    while (!WindowShouldClose()) {
        if (!tutorial && !gameOver && (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE))) {
            isPaused = !isPaused;
            if (isPaused) {
                PauseMusicStream(musicaFundo);
            } else {
                ResumeMusicStream(musicaFundo);
            }
        }
        if (IsKeyPressed(KEY_M)) {
            ToggleMute();
        }
        float deltaTime = GetFrameTime();
        UpdateGame(deltaTime);

        BeginDrawing();
            DrawGame();
        EndDrawing();
       
    }
    // limpeza de recursos
    UnloadMusicStream(musicaFundo);
    UnloadSound(ganhouPontos);
    UnloadSound(perdeuPontos);
    UnloadSound(morreu);
    UnloadSound(maisVidas);
    UnloadScreens();
    UnloadFont(Rubik);
    CloseAudioDevice();
    UnloadFont(GasoekOne);
    CloseWindow();
    return 0;
}