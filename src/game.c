#include "game.h"
#include "player.h"
#include "coco.h"
#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// ranking
#define RANKING 5
int ranking[RANKING] = {0, 0, 0, 0, 0};

// Texturas do background
Texture2D areiaTexture;
Texture2D mar1Texture;
Texture2D mar2Texture;
Texture2D coqueirosTexture;

// Áudio
Music musicaFundo;
Sound ganhouPontos;
Sound perdeuPontos;
Sound morreu;
Sound maisVidas;

// Variáveis globais do estado do jogo
int score = 0;
int vidas = 3;
bool bonusDourado = false;
float bonusTimer = 0;
bool gameOver = false;
float gameTimer = 0.0f;

// Multiplicadores de velocidade
float cocoSpeedMultiplier = 1.0f;
float playerSpeedMultiplier = 1.0f;

// Controle de progressão da quantidade de cocos na tela
int cocosAdicionados = 2;

// Protótipos das funções de ranking
void CarregarRanking();
void SalvarRanking();
void InserirNoRanking(int novoScore);

void InitGame() {
    InitAudioDevice();
    musicaFundo = LoadMusicStream("assets/audio/aPraieraInstrumental.mp3");
    PlayMusicStream(musicaFundo);
    SetMusicVolume(musicaFundo, 0.5f);

    ganhouPontos = LoadSound("assets/audio/ganhouPontos.mp3");
    perdeuPontos = LoadSound("assets/audio/perdeuPontos.mp3");
    morreu = LoadSound("assets/audio/morreu.mp3");
    maisVidas = LoadSound("assets/audio/maisVidas.mp3");

    InitPlayer();
    InitCocos();
    CarregarRanking();

    areiaTexture = LoadTexture("assets/backgrounds/areia.png");
    mar1Texture = LoadTexture("assets/backgrounds/mar1.png");
    mar2Texture = LoadTexture("assets/backgrounds/mar2.png");
    coqueirosTexture = LoadTexture("assets/backgrounds/coqueiros.png");

    score = 0;
    vidas = 3;
    bonusDourado = false;
    bonusTimer = 0;
    gameOver = false;
    cocosAdicionados = 2;
    gameTimer = 0.0f;
}

void UpdateGameProgression(float deltaTime) {
    gameTimer += deltaTime;
    extern void AddCoco();

    if (score >= 30 && cocosAdicionados == 2) {
        AddCoco();
        cocosAdicionados = 3;
    }
    if (score >= 100 && cocosAdicionados == 3) {
        AddCoco();
        cocosAdicionados = 4;
    }
    if (score >= 150 && cocosAdicionados == 4) {
        AddCoco();
        cocosAdicionados = 5;
    }

    switch (vidas) {
        case 3: playerSpeedMultiplier = 1.0f; break;
        case 2: playerSpeedMultiplier = 0.7f; break;
        case 1: playerSpeedMultiplier = 0.4f; break;
        default: playerSpeedMultiplier = 1.0f; break;
    }
}

void UpdateGame(float deltaTime) {
    UpdateMusicStream(musicaFundo);

    if (gameOver && IsKeyPressed(KEY_ENTER)) {
        gameOver = false;
        score = 0;
        vidas = 3;
        gameTimer = 0.0f;
        bonusDourado = false;
        bonusTimer = 0;
        cocosAdicionados = 2;

        ClearCocos();
        InitPlayer();
        InitCocos();

        PlayMusicStream(musicaFundo);
        return;
    }

    UpdateGameProgression(deltaTime);
    Updatecoco(deltaTime);

    if (!gameOver) {
        UpdatePlayer(deltaTime);
    }

    if (bonusDourado) {
        bonusTimer -= GetFrameTime();
        if (bonusTimer <= 0) {
            bonusDourado = false;
        }
    }

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

            if (CheckCollisionCircleRec(cocoCenter, atual->coco.radius, playerRect)) {
                if (cocoCenter.y < playerRect.y + playerRect.height * 0.3f) {

                    if (atual->coco.type == 0) {
                        PlaySound(ganhouPontos);
                        if (bonusDourado) score += 2;
                        else score += 1;
                    }
                    else if (atual->coco.type == 2) {
                        PlaySound(ganhouPontos);
                        bonusDourado = true;
                        bonusTimer = 20.0f;
                    }
                    else if (atual->coco.type == 3) {
                        PlaySound(maisVidas);
                        if (vidas < 3) vidas++;
                    }
                    else if (atual->coco.type == 1) {
                        PlaySound(perdeuPontos);
                        if (!bonusDourado) vidas--;
                    }

                    atual->coco.y = GetRandomValue(-400, -30);
                    atual->coco.x = GetRandomValue(50, 750);

                    extern void SetCocoType(Coco *coco);
                    SetCocoType(&atual->coco);
                }
            }
            atual = atual->next;
        }
    }

    if (vidas <= 0 && !gameOver) {
        gameOver = true;
        StopMusicStream(musicaFundo);
        PlaySound(morreu);
        InserirNoRanking(score);
    }
}

void DrawGame() {
    ClearBackground((Color){154, 244, 255, 255});

    float tempoAtual = GetTime();

    int waveOffset1 = sinf(tempoAtual * 1.5f) * 8;
    DrawTexture(mar1Texture, 0, waveOffset1, WHITE);

    int waveOffset2 = sinf((tempoAtual * 1.5f) + 1.0f) * 8;
    DrawTexture(mar2Texture, 0, waveOffset2, WHITE);

    DrawTexture(areiaTexture, 0, 0, WHITE);
    DrawTexture(coqueirosTexture, 0, 0, WHITE);

    DrawPlayer();
    DrawCocos();

    DrawRectangle(0, 560, 1000, 40, Fade(WHITE, 0.3f));
    DrawText(TextFormat("Cocos: %i", score), 350, 568, 24, BLACK);
    DrawText(TextFormat("Vidas: %i", vidas), 550, 568, 24, BLACK);

    if (bonusDourado) {
        DrawText(TextFormat("BONUS: %.0f", bonusTimer), 20, 100, 30, GOLD);
    }

    if (gameOver) {
        DrawRectangle(0, 0, 1000, 600, Fade(WHITE, 0.7f));
        DrawText("GAME OVER", 1000/2 - MeasureText("GAME OVER", 80)/2, 150, 80, GOLD);

        char scoreText[50];
        sprintf(scoreText, "Score final: %d", score);
        DrawText(scoreText, 1000/2 - MeasureText(scoreText, 60)/2, 250, 60, GOLD);

        DrawText("Top 5 recordes:", 1000/2 - MeasureText("Top 5 recordes:", 30)/2, 330, 30, DARKBLUE);
        for (int i = 0; i < RANKING; i++) {
            DrawText(
                TextFormat("%d. %d", i + 1, ranking[i]),
                1000/2 - 50,
                365 + i * 30,
                26,
                DARKBLUE
            );
        }

        DrawText("Press ENTER to restart", 1000/2 - MeasureText("Press ENTER to restart", 40)/2, 520, 40, DARKBLUE);
    }
}

void CarregarRanking() {
    FILE *arquivo = fopen("ranking.txt", "r");
    if (arquivo == NULL) return;
    for (int i = 0; i < RANKING; i++)
        fscanf(arquivo, "%d", &ranking[i]);
    fclose(arquivo);
}

void SalvarRanking() {
    FILE *arquivo = fopen("ranking.txt", "w");
    if (arquivo == NULL) return;
    for (int i = 0; i < RANKING; i++)
        fprintf(arquivo, "%d\n", ranking[i]);
    fclose(arquivo);
}

void InserirNoRanking(int novoScore) {
    for (int i = 0; i < RANKING; i++) {
        if (ranking[i] == novoScore) return;
    }
    for (int i = 0; i < RANKING; i++) {
        if (novoScore > ranking[i]) {
            for (int j = RANKING - 1; j > i; j--)
                ranking[j] = ranking[j-1];
            ranking[i] = novoScore;
            SalvarRanking();
            return;
        }
    }
}