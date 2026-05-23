#include "raylib.h"
#include "player.h"

Player player; // -> cria o player //

// texturas player
Texture2D crab1Texture;

// Função para inicializar o jogador (onde ele começa)//
void InitPlayer() {
    crab1Texture = LoadTexture(
        "assets/player/crabisson1.png"
    );
    player.x = 450;
    player.y = 450;
    player.speed = 7; //-> velocidade do jogador //
    player.width = 100; //-> largura do jogador //
    player.height = 40; //-> altura do jogador //
}

// Função para mover o jogador com o teclado //
void UpdatePlayer(float deltaTime) {
  extern float playerSpeedMultiplier;//vai multiplicar a velocidade base por esse
    
    float baseSpeed = 300.0f;  // Velocidade base
    float speedAtual = baseSpeed * playerSpeedMultiplier;
    //tecla para movimentar o jogador para a esquerda
    if (IsKeyDown(KEY_LEFT)) {
         player.x -= speedAtual * deltaTime;
    }
    //tecla para movimentar o jogador para a direita
    if (IsKeyDown(KEY_RIGHT)) {
        player.x += speedAtual * deltaTime;
    }

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