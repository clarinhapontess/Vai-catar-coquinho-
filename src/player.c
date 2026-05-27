#include "raylib.h"
#include "player.h"

Player player; // -> cria o player //

// Texturas para cada estado de vida do Crabsson
Texture2D crab3VidasTexture; // crabissonjogo1.png
Texture2D crab2VidasTexture; // crabissonjogo2.png
Texture2D crab1VidaTexture;  // crabissonjogo3.png

// Função para inicializar o jogador (onde ele começa)//
void InitPlayer() {
    // Carrega as 3 skins desenhadas à mão de acordo com os nomes que você passou
    crab3VidasTexture = LoadTexture("assets/player/crabissonjogo1.png");
    crab2VidasTexture = LoadTexture("assets/player/crabissonjogo2.png");
    crab1VidaTexture  = LoadTexture("assets/player/crabissonjogo3.png");

    player.x = 450;
    player.y = 450;
    player.speed = 7;   //-> velocidade do jogador //
    player.width = 100; //-> largura do jogador //
    player.height = 40; //-> altura do jogador //
}

// Função para mover o jogador com o teclado //
void UpdatePlayer(float deltaTime) {
    extern float playerSpeedMultiplier; // vai multiplicar a velocidade base por esse
    
    float baseSpeed = 300.0f;  // Velocidade base
    float speedAtual = baseSpeed * playerSpeedMultiplier;
    
    // tecla para movimentar o jogador para a esquerda
    if (IsKeyDown(KEY_LEFT)) {
         player.x -= speedAtual * deltaTime;
    }
    // tecla para movimentar o jogador para a direita
    if (IsKeyDown(KEY_RIGHT)) {
        player.x += speedAtual * deltaTime;
    }

    // delimita o jogador a area visivel da tela//
    if (player.x < 45)
        player.x = 45;

    if (player.x > 820)
        player.x = 820;
}

// Função para desenhar o jogador //
void DrawPlayer() {
    // Puxa a variável de vidas lá do game.c para saber qual skin usar
    extern int vidas; 

    // Cria uma variável temporária para segurar a textura certa neste frame
    Texture2D texturaAtual;

    // Escolhe a textura baseada na quantidade de vidas atuais
    switch (vidas) {
        case 3:
            texturaAtual = crab3VidasTexture;
            break;
        case 2:
            texturaAtual = crab2VidasTexture;
            break;
        case 1:
            texturaAtual = crab1VidaTexture;
            break;
        default:
            // Caso aconteça algum bug e fique com 0 ou menos vidas antes do Game Over, mantém a última
            texturaAtual = crab1VidaTexture; 
            break;
    }

    // Desenha a textura escolhida pelo switch
    DrawTextureEx(
        texturaAtual,
        (Vector2){player.x, player.y},
        0.0f,
        1.0f,
        WHITE
    );
}