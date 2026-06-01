#include "raylib.h"
#include "player.h"

Player player; // estrutura do jogador

void InitPlayer() {

    player.x = GetScreenWidth() * 0.45f;
    player.y = GetScreenHeight() * 0.75f;
    player.speed = 7;   // velocidade do jogador 
    player.width = 100; // largura do jogador 
    player.height = 40; // altura do jogador 
}

// move o jogador 
void UpdatePlayer(float deltaTime) {
    extern float playerSpeedMultiplier; // atualiza velocidade do jogador
    
    float baseSpeed = 230.0f; 
    float speedAtual = baseSpeed * playerSpeedMultiplier;
    
    // tecla para movimentar o jogador para a esquerda
    if (IsKeyDown(KEY_LEFT)) {
         player.x -= speedAtual * deltaTime;
    }
    // tecla para movimentar o jogador para a direita
    if (IsKeyDown(KEY_RIGHT)) {
        player.x += speedAtual * deltaTime;
    }

    // delimita o jogador a area visivel da tela
    if (player.x < 45)
        player.x = 45;

    if (player.x > 820)
        player.x = 820;
}

// desenha o jogador
void DrawPlayer() {
    extern int vidas; 
    extern int skinSelecionada;
    extern Texture2D texturasSkins[6][5];

    //versões das skins
    // índice 0 -> nome1.png (tutorial)
    // índice 1 -> nome2.png (com cestinha)
    // índice 2 -> nome3.png (machucado 1)
    // índice 3 -> nome4.png (machucado 2)
    // índice 4 -> nome5.png (chorando game over)
    
    int indiceVersao = 1; 
    if (vidas == 2) {
        indiceVersao = 2; // muda para machucado1
    } else if (vidas == 1) {
        indiceVersao = 3; // muda para machucado2
    } else if (vidas <= 0) {
        return; // não desenha o jogador se não tiver vidas
    }

    // desenha o jogador com a skin selecionada e versão correspondente às vidas
    DrawTextureEx(
        texturasSkins[skinSelecionada][indiceVersao],
        (Vector2){player.x, player.y},
        0.0f,
        1.0f,
        WHITE
    );
}