#include "game.h"
#include "player.h"
#include "coco.h"
#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Texturas do background
Texture2D areiaTexture;
Texture2D mar1Texture;      // Camada de trás
Texture2D mar2Texture;      // Camada da frente
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

// Inicializa todo o ecossistema do jogo
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

// Controla a dificuldade progressiva (Velocidades e Quantidades)
void UpdateGameProgression(float deltaTime) {
    gameTimer += deltaTime;
    extern void AddCoco(); // Vincula a função que está no coco.c

    // --- PROGRESSÃO DA QUANTIDADE DE COCOS ---
    // Marco de 30 pontos -> adiciona o 3º coco
    if (score >= 30 && cocosAdicionados == 2) {
        AddCoco(); 
        cocosAdicionados = 3;
    }
    // Marco de 100 pontos -> adiciona o 4º coco
    if (score >= 100 && cocosAdicionados == 3) {
        AddCoco();
        cocosAdicionados = 4;
    }
    // Marco de 150 pontos -> adiciona o 5º coco
    if (score >= 150 && cocosAdicionados == 4) {
        AddCoco();
        cocosAdicionados = 5;
    }

    // Player fica mais lento conforme perde vidas
    switch (vidas) {
        case 3:
            playerSpeedMultiplier = 1.0f;   
            break;
        case 2:
            playerSpeedMultiplier = 0.7f;   
            break;
        case 1:
            playerSpeedMultiplier = 0.4f;   
            break;
        default:
            playerSpeedMultiplier = 1.0f;
            break;
    }
}

// Lógica de iteração e regras de negócio do jogo
void UpdateGame(float deltaTime) {
    UpdateMusicStream(musicaFundo);

    // Sistema de RESTART
    if (gameOver && IsKeyPressed(KEY_ENTER)) {
        gameOver = false;
        score = 0;
        vidas = 3;
        gameTimer = 0.0f; 
        bonusDourado = false;
        bonusTimer = 0;
        cocosAdicionados = 2; // Reseta o limite inicial de cocos

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

    // Atualiza o timer do bônus dourado
    if (bonusDourado) {
        bonusTimer -= GetFrameTime();
        if (bonusTimer <= 0) {
            bonusDourado = false;
        }
    }

    // Loop de colisões da lista encadeada
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
                // Colisão apenas na parte superior do Crabsson
                if (cocoCenter.y < playerRect.y + playerRect.height * 0.3f) {

                    // COCO COMUM
                    if (atual->coco.type == 0) {
                        PlaySound(ganhouPontos); 
                        if (bonusDourado) score += 2;
                        else score += 1;
                    }
                    // COCO DOURADO
                    else if (atual->coco.type == 2) {
                        PlaySound(ganhouPontos); 
                        bonusDourado = true;
                        bonusTimer = 20.0f;
                    }
                    // ÁGUA DE COCO
                    else if (atual->coco.type == 3) {
                        PlaySound(maisVidas); 
                        if (vidas < 3) vidas++;
                    }
                    // LIXO
                    else if (atual->coco.type == 1) {
                        PlaySound(perdeuPontos); 
                        if (!bonusDourado) vidas--;
                    }

                    // Reseta o item colidido de volta pro topo
                    atual->coco.y = GetRandomValue(-400, -30);
                    atual->coco.x = GetRandomValue(50, 750);

                    extern void SetCocoType(Coco *coco); // Avisa o C que essa função existe no coco.c
                    SetCocoType(&atual->coco);          // Sorteia um novo tipo e velocidade!
                }
            }
            atual = atual->next;
        }
    }

    // Trigger de verificação do Game Over
    if (vidas <= 0 && !gameOver) {
        gameOver = true;
        StopMusicStream(musicaFundo); 
        PlaySound(morreu);           
    }
}

// Renderização dos elementos visuais
void DrawGame() {
    ClearBackground((Color){154, 244, 255, 255}); // Céu azul
    
    float tempoAtual = GetTime();
    
    // Mar de Trás (Seno normal)
    int waveOffset1 = sinf(tempoAtual * 1.5f) * 8;
    DrawTexture(mar1Texture, 0, waveOffset1, WHITE);
    
    // Mar da Frente (Mesmo balanço com atraso de fase orgânico)
    int waveOffset2 = sinf((tempoAtual * 1.5f) + 1.0f) * 8;
    DrawTexture(mar2Texture, 0, waveOffset2, WHITE);
    
    DrawTexture(areiaTexture, 0, 0, WHITE);
    DrawTexture(coqueirosTexture, 0, 0, WHITE);

    DrawPlayer();
    DrawCocos();

    // UI - Rodapé transparente
    DrawRectangle(0, 560, 1000, 40, Fade(WHITE, 0.3f)); 

    // Textos de Pontuação e Vidas
    DrawText(TextFormat("Cocos: %i", score), 350, 568, 24, BLACK);
    DrawText(TextFormat("Vidas: %i", vidas), 550, 568, 24, BLACK);

    // Alerta visual do bônus ativado
    if (bonusDourado) {
        DrawText(TextFormat("BONUS: %.0f", bonusTimer), 20, 100, 30, GOLD);
    }

    // Tela de Game Over overlay
    if (gameOver) {
        DrawRectangle(0, 0, 1000, 600, Fade(WHITE, 0.7f));
        DrawText("GAME OVER", 1000/2 - MeasureText("GAME OVER", 80)/2, 150, 80, GOLD);
        
        char scoreText[50];
        sprintf(scoreText, "Score final: %d", score);
        DrawText(scoreText, 1000/2 - MeasureText(scoreText, 60)/2, 300, 60, GOLD);
        DrawText("Press ENTER to restart", 1000/2 - MeasureText("Press ENTER to restart", 40)/2, 450, 40, DARKBLUE);
    }
}