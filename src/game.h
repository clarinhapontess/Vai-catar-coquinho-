#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "raylib.h"
extern Music musicaFundo;

void InitGame();
void UpdateGame();
void DrawGame();
void CarregarRanking();
void SalvarRanking();
void InserirNoRanking(int novoScore);

extern int score;
extern int vidas;

// salvar ranking e score
#define RANKING 5 // quantidade de posições no ranking
void CarregarRanking();
void SalvarRanking();
void VerRanking(int novoScore);

// Bônus dourado //
extern bool bonusDourado;
extern float bonusTimer;

#endif