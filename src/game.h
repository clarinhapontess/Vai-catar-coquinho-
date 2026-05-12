#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

void InitGame();
void UpdateGame();
void DrawGame();

extern int score;
extern int vidas;

// Bônus dourado //
extern bool bonusDourado;
extern float bonusTimer;

#endif