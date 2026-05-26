#ifndef PLAYER_H
#define PLAYER_H

// Estrutura para representar o jogador //
typedef struct {
    float x;
    float y;
    float speed;
    int width;
    int height;
} Player;

// Variáveis globais para o jogador //
void InitPlayer();
void UpdatePlayer(float deltaTime);
void DrawPlayer();

extern Player player; // Permite que outros arquivos usem: player.x e player.y //

#endif