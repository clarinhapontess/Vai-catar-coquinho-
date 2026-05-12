#include "raylib.h"
#include "player.h"

Player player; // -> cria o player //

// texturas player
Texture2D crab1Texture;

// Função para inicializar o jogador (onde ele começa)//
void InitPlayer() {
    crab1Texture = LoadTexture(
        "assets/sprites/crabisson1.png"
    );
    player.x = 450;
    player.y = 450;
    player.speed = 7; //-> velocidade do jogador //
    player.width = 100; //-> largura do jogador //
    player.height = 40; //-> altura do jogador //
}

// Função para mover o jogador com o teclado //
void UpdatePlayer() {
    //teclas para movimentar o jogador//
    if (IsKeyDown(KEY_LEFT))
        player.x -= player.speed;

    if (IsKeyDown(KEY_RIGHT))
        player.x += player.speed;

    //delimita o jogador a area visivel da tela//
    if (player.x < 45)
    player.x = 45;

    if (player.x > 820)
    player.x = 820;
}

// Função para desenhar o jogador //
void DrawPlayer() {
   DrawTextureEx(
        crab1Texture,
        (Vector2){player.x, player.y},
        0.0f,
        1.0f,
        WHITE
    );
}