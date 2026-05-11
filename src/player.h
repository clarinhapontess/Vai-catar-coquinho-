#ifndef PLAYER_H
#define PLAYER_H

// Estrutura para representar o jogador //
typedef struct {
    float x;
    float y;
    float speed;
} Player;

// Variável global para o jogador //
void InitPlayer();
void UpdatePlayer();
void DrawPlayer();

#endif