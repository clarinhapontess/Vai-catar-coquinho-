#ifndef GAME_H
#define GAME_H

#include "stdbool.h"
#include "raylib.h"

#define RANKING 5

// ranking
extern int ranking[RANKING];

// Texturas //
extern Texture2D areiaTexture;
extern Texture2D mar1Texture;
extern Texture2D mar2Texture;
extern Texture2D coqueirosTexture;
extern Texture2D coracaoTexture;

// musica e efeitos sonoros
extern Music musicaFundo;
extern Sound ganhouPontos;
extern Sound perdeuPontos;
extern Sound morreu;
extern Sound maisVidas;

// globais
extern int recorde;
extern bool bonusDourado;
extern bool gameOver;
extern float bonusTimer;
extern float gameTimer;
extern float cocoSpeedMultiplier;
extern float playerSpeedMultiplier;
extern bool isPaused;
//booleano de permissão de som 
extern bool isMuted;
extern bool tutorial; 
extern bool naHistoria;
extern bool isPaused;
extern Font GasoekOne;
extern bool naCapa;

// funções do jogo
void InitGame();
void UpdateGame(float deltaTime);
void DrawGame();
void UpdateGameProgression(float deltaTime);
void DrawTutorial(); 
void ToggleMute();
void CarregarRecorde();
void SalvarRecorde();

// funções ranking
void CarregarRanking();
void SalvarRanking();
void InserirNoRanking(int novoScore);

#endif