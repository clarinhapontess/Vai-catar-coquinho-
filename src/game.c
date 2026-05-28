#include "raylib.h"
#include <stdio.h>
#include <string.h>

// (1) Declaração de Font gameOverFont no topo
Font gameOverFont;

#define MAX_RANKING 5
typedef struct {
    char nome[32];
    int pontuacao;
} Jogador;

Jogador ranking[MAX_RANKING];
int rankingCount = 0;

// Funções de ranking e tutorial (5)
void InicializarRanking() {
    for (int i = 0; i < MAX_RANKING; i++) {
        strcpy(ranking[i].nome, "---");
        ranking[i].pontuacao = 0;
    }
    rankingCount = 0;
}

void AdicionarAoRanking(const char *nome, int pontuacao) {
    if (rankingCount < MAX_RANKING) {
        strcpy(ranking[rankingCount].nome, nome);
        ranking[rankingCount].pontuacao = pontuacao;
        rankingCount++;
    }
}

void OrdenarRanking() {
    for (int i = 0; i < rankingCount - 1; i++) {
        for (int j = i + 1; j < rankingCount; j++) {
            if (ranking[j].pontuacao > ranking[i].pontuacao) {
                Jogador temp = ranking[i];
                ranking[i] = ranking[j];
                ranking[j] = temp;
            }
        }
    }
}

void DesenharRanking() {
    OrdenarRanking();
    int y = 200;
    DrawText("TOP 5", 400, 150, 30, GOLD);
    for (int i = 0; i < rankingCount && i < MAX_RANKING; i++) {
        char linha[64];
        sprintf(linha, "%d. %s - %d", i+1, ranking[i].nome, ranking[i].pontuacao);
        DrawText(linha, 350, y, 20, WHITE);
        y += 30;
    }
}

void DesenharTutorial() {
    DrawText("Use as setas para mover", 300, 400, 20, LIGHTGRAY);
    DrawText("Pressione ESPAÇO para atirar", 280, 430, 20, LIGHTGRAY);
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Game");
    
    // Carrega a fonte para game over
    gameOverFont = LoadFont("resources/gameover.ttf");
    if (gameOverFont.texture.id == 0) {
        gameOverFont = GetFontDefault();
    }
    
    bool gameOver = false;
    bool bonusOuro = false;
    int pontuacao = 0;
    char nomeJogador[32] = "";
    int frameCounter = 0;
    
    InicializarRanking();
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        if (!gameOver) {
            // Simulação de jogo (substituir pela lógica real)
            if (IsKeyPressed(KEY_SPACE)) {
                pontuacao += 10;
            }
            if (IsKeyPressed(KEY_B)) {
                bonusOuro = true;
            }
            // (2) bonus dourado com DrawText
            if (bonusOuro) {
                DrawText("BONUS DOURADO!", screenWidth/2 - 100, screenHeight/2 - 50, 30, GOLD);
                if (frameCounter++ > 120) {
                    bonusOuro = false;
                    frameCounter = 0;
                }
            }
            
            // Simular game over
            if (IsKeyPressed(KEY_G)) {
                gameOver = true;
                sprintf(nomeJogador, "Jogador%d", GetRandomValue(1, 100));
                AdicionarAoRanking(nomeJogador, pontuacao);
            }
            
            DrawText("Aperte ESPAÇO para ganhar pontos", 200, 300, 20, WHITE);
            DrawText(TextFormat("Pontuação: %d", pontuacao), 10, 10, 20, WHITE);
            DesenharTutorial();
        } else {
            // (3) game over com DrawTextEx usando gameOverFont
            Vector2 pos = { screenWidth/2 - 100, screenHeight/2 - 50 };
            DrawTextEx(gameOverFont, "GAME OVER", pos, 40, 2, RED);
            DrawText("Aperte R para reiniciar", 300, 400, 20, WHITE);
            
            // (4) ranking Top 5
            DesenharRanking();
            
            if (IsKeyPressed(KEY_R)) {
                gameOver = false;
                pontuacao = 0;
                bonusOuro = false;
            }
        }
        
        EndDrawing();
    }
    
    UnloadFont(gameOverFont);
    CloseWindow();
    return 0;
}