#include "game.h"
#include "player.h"
#include "coco.h"

#include "raylib.h"
#include <stdlib.h>
#include <math.h>

// Texturas do background
Texture2D areiaTexture;
Texture2D marTexture;
Texture2D coqueirosTexture;

// musica de fundo
Music musicaFundo;

// efeitos sonoros
Sound ganhouPontos;
Sound perdeuPontos;
Sound morreu;
Sound maisVidas;

// pontuação do jogador
int score = 0;

// quantidade de vidas
int vidas = 3;

// controla o bônus do coco dourado
bool bonusDourado = false;

// tempo restante do bônus
float bonusTimer = 0;

// game over
bool gameOver = false;

// inicializa o jogo
void InitGame() {
    InitAudioDevice(); // inicializa áudios
    musicaFundo= LoadMusicStream("assets/audio/aPraieraInstrumental.mp3");
    PlayMusicStream(musicaFundo);
    SetMusicVolume(musicaFundo, 0.5f); // volume mais baixo para não atrapalhar os efeitos sonoros
    ganhouPontos = LoadSound("assets/audio/ganhouPontos.mp3");
    perdeuPontos = LoadSound("assets/audio/perdeuPontos.mp3");
    morreu = LoadSound("assets/audio/morreu.mp3");
    maisVidas = LoadSound("assets/audio/maisVidas.mp3");


    InitPlayer();
    InitCocos();
   
    areiaTexture = LoadTexture("assets/backgrounds/areia.png");
    marTexture = LoadTexture("assets/backgrounds/mar.png");
    coqueirosTexture = LoadTexture("assets/backgrounds/coqueiros.png");

    score = 0;
    vidas = 3;
    bonusDourado = false;
    bonusTimer = 0;
    gameOver = false;
}

// atualiza o jogo
void UpdateGame() {
        UpdateMusicStream(musicaFundo);

    // restart
    if (gameOver && IsKeyPressed(KEY_ENTER)) {

        // reseta estados
        gameOver = false;

        score = 0;
        vidas = 3;

        bonusDourado = false;
        bonusTimer = 0;

        // LIMPA LISTA ANTIGA
        ClearCocos();

        // recria tudo
        InitPlayer();
        InitCocos();

        return;
    }

    // cocos SEMPRE atualizam
    UpdateCocos();

    // player só atualiza se não estiver morto
    if (!gameOver) {
        UpdatePlayer();
    }

    // atualiza timer do bônus dourado
    if (bonusDourado) {

        bonusTimer -= GetFrameTime();

        if (bonusTimer <= 0) {
            bonusDourado = false;
        }
    }

    // colisão só acontece durante gameplay
    if (!gameOver) {

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

            // colisão base
            if (CheckCollisionCircleRec(
                    cocoCenter,
                    atual->coco.radius,
                    playerRect)) {

                // só topo do player
                if (cocoCenter.y < playerRect.y + playerRect.height * 0.3f) {

                    // COCO COMUM
                    if (atual->coco.type == 0) {
                        PlaySound(ganhouPontos); // toca som de pontuação

                        if (bonusDourado) {
                            score += 2;
                        } else {
                            score += 1;
                        }
                    }

                    // COCO DOURADO
                    else if (atual->coco.type == 2) {
                        PlaySound(ganhouPontos); // toca som de pontuação

                        bonusDourado = true;
                        bonusTimer = 20.0f;
                    }

                    // ÁGUA DE COCO
                    else if (atual->coco.type == 3) {
                        PlaySound(maisVidas); // som + vidas

                        if (vidas < 3) {
                            vidas++;
                        }
                    }

                    // LIXO
                    else if (atual->coco.type == 1) {
                        PlaySound(perdeuPontos); // som de perder pontos

                        if (!bonusDourado) {
                            vidas--;
                        }
                    }

                    // reposiciona coco
                    atual->coco.y = GetRandomValue(-400, -30);
                    atual->coco.x = GetRandomValue(50, 750);
                }
            }

            atual = atual->next;
        }
    }

    // verifica game over
    if (vidas <= 0) {
        gameOver = true;
        PlaySound(morreu); // som morreu
    }
}

// desenha o jogo
void DrawGame() {

    // fundo azul céu
    ClearBackground((Color){154, 244, 255, 255});
    
    // mar
    int waveOffset = sinf(GetTime() * 1.3f) * 14;
    DrawTexture(
        marTexture, 
        0, 
        waveOffset, 
        WHITE);
    // areia
    DrawTexture(areiaTexture, 0, 0, WHITE);
    // coqueiros
    DrawTexture(coqueirosTexture, 0, 0, WHITE);

    DrawPlayer();
    DrawCocos();

    // rodape
    DrawRectangle(0, 560, 1000, 40, Fade(WHITE, 0.3f)); // branco bem transparente

    // score
    DrawText(
        TextFormat("Cocos: %i", score),
        350,
        568,
        24,
        BLACK
    );

    // vidas
    DrawText(
        TextFormat("Vidas: %i", vidas),
        550,
        568,
        24,
        BLACK
    );

    // bônus dourado
    if (bonusDourado) {

        DrawText(
            TextFormat("BONUS: %.0f", bonusTimer),
            20,
            100,
            30,
            GOLD
        );
    }

    // game over
    if (gameOver) {

        DrawRectangle(
            0,
            0,
            1000,
            600,
            Fade(BLACK, 0.7f)
        );

        DrawText(
            "GAME OVER",
            250,
            180,
            50,
            RED
        );

        DrawText(
            TextFormat("Score final: %d", score),
            250,
            250,
            30,
            WHITE
        );

        DrawText(
            "Press ENTER to restart",
            250,
            300,
            20,
            GRAY
        );
    }
}