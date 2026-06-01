#ifndef PLAYER_H
#define PLAYER_H

// estrutura do jogador
typedef struct {
    float x;
    float y;
    float speed;
    int width;
    int height;
} Player;

// funções do jogador
void InitPlayer();
void UpdatePlayer(float deltaTime);
void DrawPlayer();
extern bool temSkinNova; 
extern Player player;

#endif