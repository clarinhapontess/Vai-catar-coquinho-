#include "raylib.h"
#include "player.h"

Player player; // cria o player //

extern int vidas;
Texture2D crab1Texture;
Texture2D crab2Texture; 
Texture2D crab3Texture;

// função para inicializar o player //
void InitPlayer() {
    crab1Texture = LoadTexture("assets/player/crabissonjogo1.png");
    crab2Texture = LoadTexture("assets/player/crabissonjogo2.png");
    crab3Texture = LoadTexture("assets/player/crabissonjogo3.png");
    player.x = 450;
    player.y = 450;
    player.width = 100;
    player.height = 40;
}

// função para atualizar o player //
void UpdatePlayer(float deltaTime) {
    extern float playerSpeedMultiplier; // variável global para multiplicar a velocidade do player //

    float baseSpeed = 300.0f;
    float speedAtual = baseSpeed * playerSpeedMultiplier;

    // movendo o player //
    if (IsKeyDown(KEY_LEFT)) {
        player.x -= speedAtual * deltaTime;
    } else if (IsKeyDown(KEY_RIGHT)) {
        player.x += speedAtual * deltaTime;
    }

    // mantendo o player dentro da tela //
    if (player.x < 45) {
        player.x = 45;
    } else if (player.x > 820) {
        player.x = 820;
    }
}

// função para desenhar o player //
void DrawPlayer() {
    Texture2D texturaAtual;

    // Escolhe a textura baseada na quantidade exata de vidas //
    if (vidas == 2) {
        texturaAtual = crab2Texture;
    } else if (vidas == 1) {
        texturaAtual = crab3Texture;
    } else {
        texturaAtual = crab1Texture;
    }
    
    DrawTextureEx(
        texturaAtual,
        (Vector2){ player.x, player.y },
        0.0f,
        1.0f,
        WHITE
    );
}