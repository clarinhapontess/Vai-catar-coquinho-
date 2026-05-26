#include "game.h"
#include "player.h"
#include "coco.h"
#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Texturas do background //
Texture2D areiaTexture;
Texture2D mar1Texture;
Texture2D mar2Texture;
Texture2D coqueirosTexture;

// Música e efeitos sonoros //
Music musicaFundo; 
Sound ganhouPontos;
Sound perdeuPontos;
Sound morreu;
Sound maisVidas;

// Variáveis globais do estado do jogo //
int score = 0;
int vidas = 3;
bool bonusDourado = false;
float bonusTimer = 0;
bool gameOver = false;
float gameTimer = 0;
float playerSpeedMultiplier = 1.0f; // CRIADO: Definição oficial da variável global! //

// adiciona cocos conforme dificuldade aumenta //
int cocosAdicionados = 2;

// inicializa o jogo //
void InitGame() {
    InitAudioDevice();

    // MUSICA DE FUNDO //
    musicaFundo = LoadMusicStream("assets/audio/aPraieraInstrumental.mp3");
    PlayMusicStream(musicaFundo);
    SetMusicVolume(musicaFundo, 0.5f);

    // EFEITOS SONOROS //
    ganhouPontos = LoadSound("assets/audio/ganhouPontos.mp3");
    perdeuPontos = LoadSound("assets/audio/perdeuPontos.mp3");
    morreu = LoadSound("assets/audio/morreu.mp3"); // ALTERADO: minúsculo //
    maisVidas = LoadSound("assets/audio/maisVidas.mp3");

    InitPlayer(); // Inicializa o jogador //
    InitCocos(); // Inicializa os cocos //

    // CARREGA AS TEXTURAS DO BACKGROUND //
    areiaTexture = LoadTexture("assets/backgrounds/areia.png");
    mar1Texture = LoadTexture("assets/backgrounds/mar1.png");
    mar2Texture = LoadTexture("assets/backgrounds/mar2.png");
    coqueirosTexture = LoadTexture("assets/backgrounds/coqueiros.png");

    // Inicializa as variáveis do jogo //
    score = 0;
    vidas = 3;
    bonusDourado = false;
    bonusTimer = 0;
    gameOver = false;
    gameTimer = 0;
    playerSpeedMultiplier = 1.0f; // Reseta a velocidade ao iniciar //
}

// Dificuldade progressiva //
void UpdateGameProgression(float deltaTime) {
    gameTimer += deltaTime;
    extern void AddCoco(); // funcao em coco.c //

    if (score >= 30 && cocosAdicionados == 2) {
        AddCoco();
        cocosAdicionados = 3;
    } else if (score >= 50 && cocosAdicionados == 3) {
        AddCoco();
        cocosAdicionados = 4;
    } else if (score >= 100 && cocosAdicionados == 4) {
        AddCoco();
        AddCoco();
        cocosAdicionados = 6;
    }

    // Player fica mais lento ao perder vidas //
    switch (vidas) {
        case 2:
            playerSpeedMultiplier = 0.6f; // Velocidade reduzida //
            break;
        case 1:
            playerSpeedMultiplier = 0.4f; // Velocidade reduzida //
            break;
        default:
            playerSpeedMultiplier = 1.0f; // Velocidade normal //
            break;
    }
}

// Atualiza o estado do jogo //
void UpdateGame (float deltaTime) {
    UpdateMusicStream(musicaFundo);

    // RESTART //
    if (gameOver && IsKeyPressed(KEY_ENTER)) {
        gameOver = false;
        score = 0;
        vidas = 3;
        bonusDourado = false;
        bonusTimer = 0;
        gameTimer = 0;
        cocosAdicionados = 2;

        ClearCocos(); 
        InitCocos(); 
        InitPlayer(); 

        PlayMusicStream(musicaFundo); 
        return;
    }

    UpdateGameProgression(deltaTime);
    Updatecoco(deltaTime);

    if (!gameOver) {
        UpdatePlayer(deltaTime);
    }

    // Atualiza timer do bônus dourado //
    if (bonusDourado) {
        bonusTimer -= GetFrameTime();
        if (bonusTimer <= 0) {
            bonusDourado = false;
        }
    }

    // Loop de colisão //
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
                // colisão apenas por cima //
                if (cocoCenter.y < playerRect.y + playerRect.height * 0.3f) {
                    
                    // COCO NORMAL //
                    if (atual->coco.type == 0) {
                        PlaySound(ganhouPontos);
                        if (bonusDourado) {
                            score += 2;
                        } else {
                            score += 1;
                        }
                    }
                    // LIXO //
                    else if (atual->coco.type == 1) {
                        PlaySound(perdeuPontos);
                        score -= 1;
                        if (score < 0) score = 0;
                        vidas--; 
                    }
                    // COCO DOURADO //
                    else if (atual->coco.type == 2) {
                        PlaySound(ganhouPontos);
                        bonusDourado = true;
                        bonusTimer = 20.0f; 
                    }
                    // AGUA DE COCO //
                    else if (atual->coco.type == 3) {
                        PlaySound(maisVidas);
                        if (vidas < 3) vidas++;
                    }

                    // Reseta o coco para o topo da tela //
                    atual->coco.y = GetRandomValue(-400, -30);
                    atual->coco.x = GetRandomValue(50, 750);

                    extern void SetCocoType(Coco *coco); 
                    SetCocoType(&atual->coco); 
                }
            }
            atual = atual->next; 
        }
    }

    // Verificando Game Over //
    if (vidas <= 0 && !gameOver) {
        PlaySound(morreu);
        gameOver = true;
        StopMusicStream(musicaFundo);
    }
}

// Desenha o jogo na tela //
void DrawGame() {
    ClearBackground((Color){154, 244, 255, 255});

    // Mar //
    float tempoAtual = GetTime();

    int waveOffset = sinf(tempoAtual * 1.5f) * 8;
    DrawTexture(mar1Texture, 0, waveOffset, WHITE);

    int waveOffset2 = sinf(tempoAtual * 1.5f + 1.0f) * 8;
    DrawTexture(mar2Texture, 0, waveOffset2, WHITE);

    // Areia e coqueiros //
    DrawTexture(areiaTexture, 0, 0, WHITE);
    DrawTexture(coqueirosTexture, 0, 0, WHITE);

    DrawCocos();
    DrawPlayer();

    // Rodape transparente //
    DrawRectangle(0, 560, 1000, 40, Fade(WHITE, 0.3f));

    // Exibe pontuação e vidas //
    DrawText(TextFormat("Cocos: %i", score), 150, 568, 24, BLACK);
    DrawText(TextFormat("Vidas: %i", vidas), 550, 568, 24, BLACK);

    // Bonus dourado ativo //
    if (bonusDourado) {
        DrawText(TextFormat("Bônus: %.0f", bonusTimer), 300, 568, 24, GOLD);
    }

    // Tela de Game Over //
    if (gameOver) {
        DrawRectangle(0, 0, 1000, 600, Fade(WHITE, 0.7f));
        DrawText("GAME OVER", 1000/2 - MeasureText("GAME OVER", 80)/2, 150, 80, GOLD);
        
        char scoreText[50];
        sprintf(scoreText, "Score final: %d", score);
        DrawText(scoreText, 1000/2 - MeasureText(scoreText, 60)/2, 300, 60, GOLD);
        DrawText("Press ENTER to restart", 1000/2 - MeasureText("Press ENTER to restart", 40)/2, 450, 40, DARKBLUE);
    }
}