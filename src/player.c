#include "raylib.h"
#include "player.h"

Player player; // -> cria o player //

// Função para inicializar o jogador (onde ele começa)//
void InitPlayer() {
    player.x = 450;
    player.y = 520;
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
    if (player.x < 0)
    player.x = 0;

    if (player.x > 900)
    player.x = 900;
}

// Função para desenhar o jogador //
void DrawPlayer() {
    DrawRectangle(
    player.x,
    player.y,
    player.width,
    player.height,
    RED
);
}