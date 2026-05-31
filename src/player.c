#include "raylib.h"
#include "player.h"

Player player; // -> cria o player //

// Função para inicializar o jogador (onde ele começa)//
void InitPlayer() {
    // As texturas não são mais carregadas aqui individualmente! 
    // Elas são carregadas via matriz automática na InitGame().

    player.x = 450;
    player.y = 450;
    player.speed = 7;   //-> velocidade do jogador //
    player.width = 100; //-> largura do jogador //
    player.height = 40; //-> altura do jogador //
}

// Função para mover o jogador com o teclado //
void UpdatePlayer(float deltaTime) {
    extern float playerSpeedMultiplier; // vai multiplicar a velocidade base por esse
    
    float baseSpeed = 230.0f;  // Velocidade base
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
    // Puxa as variáveis de controle globais lá do game.c
    extern int vidas; 
    extern int skinSelecionada;
    extern Texture2D texturasSkins[6][5];

    // Mapeamento das 5 versões de imagem por skin:
    // índice 0 -> nome1.png (Normal - usada no tutorial)
    // índice 1 -> nome2.png (Normal com Cestinha)
    // índice 2 -> nome3.png (Machucado 1)
    // índice 3 -> nome4.png (Machucado 2)
    // índice 4 -> nome5.png (Chorando Game Over - desenhado no game.c)
    
    int indiceVersao = 1; // Padrão de jogo: Vida Cheia (Normal com Cestinha)

    if (vidas == 2) {
        indiceVersao = 2; // Mudança automática para Machucado 1
    } else if (vidas == 1) {
        indiceVersao = 3; // Mudança automática para Machucado 2
    } else if (vidas <= 0) {
        return; // Se morreu, não desenha o player aqui na tela de jogo (o game.c vai desenhar a versão chorando)
    }

    // Desenha a textura da skin selecionada com a versão correspondente à vida atual
    DrawTextureEx(
        texturasSkins[skinSelecionada][indiceVersao],
        (Vector2){player.x, player.y},
        0.0f,
        1.0f,
        WHITE
    );
}