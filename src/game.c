#include "game.h"
#include "player.h"
#include "coco.h"
#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Ranking //
int recorde = 0;

// Texturas do background //
Texture2D areiaTexture;
Texture2D mar1Texture;
Texture2D mar2Texture;
Texture2D coqueirosTexture;
Texture2D coracaoTexture;

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
bool tutorial = true;
float playerSpeedMultiplier = 1.0f; 
float cocoSpeedMultiplier = 1.0f;

// Adiciona cocos conforme dificuldade aumenta //
int cocosAdicionados = 2;

// Protótipos das funções de ranking e interface //

void DrawTutorial();

// Inicialização do jogo //
void InitGame() {
    InitAudioDevice();

    // Música de fundo //
    musicaFundo = LoadMusicStream("assets/audio/aPraieraInstrumental.wav");
    PlayMusicStream(musicaFundo);
    SetMusicVolume(musicaFundo, 0.5f);

    // Efeitos sonoros //
    ganhouPontos = LoadSound("assets/audio/ganhouPontos.wav");
    perdeuPontos = LoadSound("assets/audio/perdeuPontos.wav");
    morreu = LoadSound("assets/audio/morreu.wav"); 
    maisVidas = LoadSound("assets/audio/maisVidas.wav");

    InitPlayer(); 
    InitCocos(); 
    CarregarRecorde();

    // Carrega as texturas //
    areiaTexture = LoadTexture("assets/backgrounds/areia.png");
    mar1Texture = LoadTexture("assets/backgrounds/mar1.png");
    mar2Texture = LoadTexture("assets/backgrounds/mar2.png");
    coqueirosTexture = LoadTexture("assets/backgrounds/coqueiros.png");
    coracaoTexture = LoadTexture("assets/backgrounds/coracao.png");

    // Inicializa as variáveis do jogo //
    score = 0;
    vidas = 3;
    bonusDourado = false;
    bonusTimer = 0;
    gameOver = false;
    gameTimer = 0;
    cocosAdicionados = 2;
    playerSpeedMultiplier = 1.0f; 
}

// Atualização da dificuldade do jogo //
void UpdateGameProgression(float deltaTime) {
    gameTimer += deltaTime;
    extern void AddCoco(); 

    // Sistema de progressão da branch danda //
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
            playerSpeedMultiplier = 0.6f; 
            break;
        case 1:
            playerSpeedMultiplier = 0.4f; 
            break;
        default:
            playerSpeedMultiplier = 1.0f; 
            break;
    }
}

// Atualização do jogo //
void UpdateGame(float deltaTime) {
    if (tutorial) {
        if (IsKeyPressed(KEY_ENTER)) tutorial = false;
        return;  
    }

    UpdateMusicStream(musicaFundo);

    // Restart //
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
                // Colisão apenas por cima //
                if (cocoCenter.y < playerRect.y + playerRect.height * 0.3f) {
                    
                    // Coco normal //
                    if (atual->coco.type == 0) {
                        PlaySound(ganhouPontos);
                        if (bonusDourado) score += 2;
                        else score += 1;
                    }
                    // Lixo //
                    else if (atual->coco.type == 1) {
                        PlaySound(perdeuPontos);
                        score -= 1;
                        if (score < 0) score = 0;
                        if (!bonusDourado) vidas--; 
                    }
                    // Coco dourado //
                    else if (atual->coco.type == 2) {
                        PlaySound(ganhouPontos);
                        bonusDourado = true;
                        bonusTimer = 20.0f; 
                    }
                    // Água de coco //
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
        if (score > recorde) {
            recorde = score;
            SalvarRecorde();
        }
    }
}

// Desenho do jogo //
void DrawGame() {
    // Tela de tutorial //
    if (tutorial) {
        DrawTutorial();
        return;
    }

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

    // Tela de bônus dourado (Overlay translúcido) //
    if (bonusDourado) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(GOLD, 0.15f));
    }

    // --- INTERFACE DE PONTUAÇÃO E VIDAS (Fontes aumentadas para 34 e sem barra) --- //
    DrawText(TextFormat("Cocos: %i", score), 832, 559, 34, BLACK); // Sombra
    DrawText(TextFormat("Cocos: %i", score), 830, 557, 34, RAYWHITE);

    DrawText("Vidas: ", 32, 559, 34, BLACK); // Sombra
    DrawText("Vidas: ", 30, 557, 34, RAYWHITE);

    // Escala e posicionamento dos corações //
    float escalaCoracao = 0.6f;
    for (int i = 0; i < vidas; i++) {
        float espacamentoHorizontal = i * 45.0f;
        Vector2 posicao = { 140.0f + espacamentoHorizontal, 555.0f }; // Ajustado para X = 140 para não sobrepor o texto maior
        
        // Sombra do coração para destacar na areia
        DrawTextureEx(coracaoTexture, (Vector2){ posicao.x + 2, posicao.y + 2 }, 0.0f, escalaCoracao, Fade(BLACK, 0.60f));
        
        // Coração principal
        DrawTextureEx(coracaoTexture, posicao, 0.0f, escalaCoracao, WHITE);
    }

    // Bônus dourado centralizado //
    if (bonusDourado) {
        char textoBonus[30];
        sprintf(textoBonus, "BÔNUS DOURADO: %.0f s", bonusTimer);
        
        int larguraTexto = MeasureText(textoBonus, 28); 
        int xCentralizado = (GetScreenWidth() / 2) - (larguraTexto / 2);

        Color corBrilho = (sinf(GetTime() * 10) > 0) ? GOLD : YELLOW; 
        
        // Sombra e texto principal
        DrawText(textoBonus, xCentralizado + 2, 564, 28, BLACK);
        DrawText(textoBonus, xCentralizado, 562, 28, corBrilho);
    }
    if(gameOver){ // Dimensões do retângulo semitransparente
    const int rectWidth = 600;
    const int rectHeight = 350;
    const int rectX = (1000 - rectWidth) / 2;      // Centralizado horizontalmente
    const int rectY = (600 - rectHeight) / 2;      // Centralizado verticalmente
    const int padding = 30;                         // Espaço interno

    // Desenhar retângulo semitransparente
    DrawRectangle(rectX, rectY, rectWidth, rectHeight, Fade(WHITE, 0.5f));

    // Posição inicial dos textos
    int textY = rectY + padding;

    // Título: GAME OVER
    const char *title = "GAME OVER";
    int titleWidth = MeasureText(title, 60);
    DrawText(title, rectX + (rectWidth - titleWidth) / 2, textY, 60, GOLD);
    textY += 80;

    // Score final
    char scoreText[50];
    sprintf(scoreText, "Score final: %d", score);
    int scoreWidth = MeasureText(scoreText, 40);
    DrawText(scoreText, rectX + (rectWidth - scoreWidth) / 2, textY, 40, WHITE);
    textY += 60;

    // Recorde
    char recordeText[50];
    sprintf(recordeText, "Recorde: %d", recorde);
    int recordeWidth = MeasureText(recordeText, 40);
    DrawText(recordeText, rectX + (rectWidth - recordeWidth) / 2, textY, 40, GOLD);
    textY += 60;

    // Instrução de reinício
    const char *restartText = "Pressione ENTER para reiniciar!";
    int restartWidth = MeasureText(restartText, 30);
    DrawText(restartText, rectX + (rectWidth - restartWidth) / 2, textY, 30, DARKBLUE);}
 
}


// Funções de ranking //
void CarregarRecorde() {
    FILE *arquivo = fopen("recorde.txt", "r");
    if (arquivo == NULL) return;
    fscanf(arquivo, "%d", &recorde);
    fclose(arquivo);
}

void SalvarRecorde() {
    FILE *arquivo = fopen("recorde.txt", "w");
    if (arquivo == NULL) return;
    fprintf(arquivo, "%d\n", recorde);
    fclose(arquivo);
}

// Desenho do tutorial //
void DrawTutorial() {
    ClearBackground((Color){154, 244, 255, 255});

    float tempoAtual = GetTime();
    int waveOffset1 = sinf(tempoAtual * 1.5f) * 8;
    DrawTexture(mar1Texture, 0, waveOffset1, WHITE);
    int waveOffset2 = sinf((tempoAtual * 1.5f) + 1.0f) * 8;
    DrawTexture(mar2Texture, 0, waveOffset2, WHITE);
    DrawTexture(areiaTexture, 0, 0, WHITE);
    DrawTexture(coqueirosTexture, 0, 0, WHITE);

    // Fundo branco semitransparente atrás do texto //
    DrawRectangle(100, 40, 800, 520, Fade(WHITE, 0.75f));

    DrawText("VAI CATAR COQUINHO", 1000/2 - MeasureText("VAI CATAR COQUINHO", 60)/2, 50, 60, DARKGREEN);
    DrawText("Como jogar:", 1000/2 - MeasureText("Como jogar:", 30)/2, 140, 30, BLACK);
    DrawText("Use as setas <- -> para mover o caranguejo", 1000/2 - MeasureText("Use as setas <- -> para mover o caranguejo", 24)/2, 185, 24, BLACK);
    DrawText("Tipos de itens:", 1000/2 - MeasureText("Tipos de itens:", 30)/2, 240, 30, BLACK);
    DrawText("Coco verde: +1 ponto", 1000/2 - MeasureText("Coco verde: +1 ponto", 24)/2, 285, 24, DARKGREEN);
    DrawText("Coco dourado: dobra os pontos por 20 segundos!", 1000/2 - MeasureText("Coco dourado: dobra os pontos por 20 segundos!", 24)/2, 320, 24, GOLD);
    DrawText("Agua de coco: +1 vida", 1000/2 - MeasureText("Agua de coco: +1 vida", 24)/2, 355, 24, BLUE);
    DrawText("Lixo: -1 vida", 1000/2 - MeasureText("Lixo: -1 vida", 24)/2, 390, 24, RED);
    DrawText("Pressione ENTER para comecar!", 1000/2 - MeasureText("Pressione ENTER para comecar!", 30)/2, 460, 30, DARKBLUE);
}