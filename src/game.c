#include "game.h"
#include "player.h"
#include "coco.h"
#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Texturas do background //
Texture2D areiaTexture;
Texture2D mar1Texture;
Texture2D mar2Texture;
Texture2D coqueirosTexture;
Texture2D coracaoTexture;

// Música e efeitos sonoros //
Music musicaFundo; 
Sound ganhouPontos;
Sound perdeuPontos;
Sound morreu;
Sound maisVidas;

// ranking //
#define RANKING 5
int ranking[RANKING] = {0, 0, 0, 0, 0};

// Variáveis globais do estado do jogo //
int score = 0;
int vidas = 3;
bool bonusDourado = false;
float bonusTimer = 0;
bool gameOver = false;
float gameTimer = 0;
bool tutorial = true;
float playerSpeedMultiplier = 1.0f; // Definição oficial da variável global! //

// adiciona cocos conforme dificuldade aumenta //
int cocosAdicionados = 2;

// Protótipos das funções de ranking //
void CarregarRanking();
void SalvarRanking();
void InserirNoRanking(int novoScore);
void DrawTutorial();

// inicializa o jogo //
void InitGame() {
    InitAudioDevice();

    // MUSICA DE FUNDO //
    musicaFundo = LoadMusicStream("assets/audio/aPraieraInstrumental.mp3");
    PlayMusicStream(musicaFundo);
    SetMusicVolume(musicaFundo, 0.5f);

    // EFEITOS SONOROS //
    ganhouPontos = LoadSound("assets/audio/ganhouPontos.mp3");
    perdeuPontos = LoadSound("assets/audio/perdeuPontos.mp3");
    morreu = LoadSound("assets/audio/morreu.mp3"); // ALTERADO: minúsculo //
    maisVidas = LoadSound("assets/audio/maisVidas.mp3");

    InitPlayer(); // Inicializa o jogador //
    InitCocos(); // Inicializa os cocos //

    // CARREGA AS TEXTURAS DO BACKGROUND //
    areiaTexture = LoadTexture("assets/backgrounds/areia.png");
    mar1Texture = LoadTexture("assets/backgrounds/mar1.png");
    mar2Texture = LoadTexture("assets/backgrounds/mar2.png");
    coqueirosTexture = LoadTexture("assets/backgrounds/coqueiros.png");
    coracaoTexture = LoadTexture("assets/backgrounds/coracao.png");

    // Inicializa as variáveis do jogo //
    score = 0;
    vidas = 3;
    bonusDourado = false;
    bonusTimer = 0;
    gameOver = false;
    gameTimer = 0;
    playerSpeedMultiplier = 1.0f; // Reseta a velocidade ao iniciar //
}

// Dificuldade progressiva //
void UpdateGameProgression(float deltaTime) {
    gameTimer += deltaTime;
    extern void AddCoco(); // funcao em coco.c //

    if (score >= 30 && cocosAdicionados == 2) {
        AddCoco();
        cocosAdicionados = 3;
    } else if (score >= 50 && cocosAdicionados == 3) {
        AddCoco();
        cocosAdicionados = 4;
    } else if (score >= 100 && cocosAdicionados == 4) {
        AddCoco();
        AddCoco();
        cocosAdicionados = 6;
    }

    // Player fica mais lento ao perder vidas //
    switch (vidas) {
        case 2:
            playerSpeedMultiplier = 0.6f; // Velocidade reduzida //
            break;
        case 1:
            playerSpeedMultiplier = 0.4f; // Velocidade reduzida //
            break;
        default:
            playerSpeedMultiplier = 1.0f; // Velocidade normal //
            break;
    }
}

// Atualiza o estado do jogo //
void UpdateGame (float deltaTime) {
    // Se o tutorial estiver ativo, não atualiza o jogo //
    if (tutorial) {
        if (IsKeyPressed(KEY_ENTER)) tutorial = false;
        return;  // não atualiza o jogo enquanto tutorial estiver ativo
    }

    UpdateMusicStream(musicaFundo);

    // RESTART //
    if (gameOver && IsKeyPressed(KEY_ENTER)) {
        gameOver = false;
        score = 0;
        vidas = 3;
        bonusDourado = false;
        bonusTimer = 0;
        gameTimer = 0;
        cocosAdicionados = 2;

        ClearCocos(); 
        InitCocos(); 
        InitPlayer(); 

        PlayMusicStream(musicaFundo); 
        return;
    }

    UpdateGameProgression(deltaTime);
    Updatecoco(deltaTime);

    if (!gameOver) {
        UpdatePlayer(deltaTime);
    }

    // Atualiza timer do bônus dourado //
    if (bonusDourado) {
        bonusTimer -= GetFrameTime();
        if (bonusTimer <= 0) {
            bonusDourado = false;
        }
    }

    // Loop de colisão //
    if (!gameOver) {
        Node *atual = listaCocos;

        while (atual != NULL) {
            Rectangle playerRect = {
                player.x,
                player.y,
                player.width,
                player.height
            };
            
            Vector2 cocoCenter = {
                atual->coco.x,
                atual->coco.y
            };

            if (CheckCollisionCircleRec(cocoCenter, atual->coco.radius, playerRect)) {
                // colisão apenas por cima //
                if (cocoCenter.y < playerRect.y + playerRect.height * 0.3f) {
                    
                    // COCO NORMAL //
                    if (atual->coco.type == 0) {
                        PlaySound(ganhouPontos);
                        if (bonusDourado) {
                            score += 2;
                        } else {
                            score += 1;
                        }
                    }
                    // LIXO //
                    else if (atual->coco.type == 1) {
                        PlaySound(perdeuPontos);
                        score -= 1;
                        if (score < 0) score = 0;
                        vidas--; 
                    }
                    // COCO DOURADO //
                    else if (atual->coco.type == 2) {
                        PlaySound(ganhouPontos);
                        bonusDourado = true;
                        bonusTimer = 20.0f; 
                    }
                    // AGUA DE COCO //
                    else if (atual->coco.type == 3) {
                        PlaySound(maisVidas);
                        if (vidas < 3) vidas++;
                    }

                    // Reseta o coco para o topo da tela //
                    atual->coco.y = GetRandomValue(-400, -30);
                    atual->coco.x = GetRandomValue(50, 750);

                    extern void SetCocoType(Coco *coco); 
                    SetCocoType(&atual->coco); 
                }
            }
            atual = atual->next; 
        }
    }

    // Verificando Game Over //
    if (vidas <= 0 && !gameOver) {
        PlaySound(morreu);
        gameOver = true;
        StopMusicStream(musicaFundo);
    }
}

// Desenha o jogo na tela //
void DrawGame() {

    // Tela de tutorial //
    if (tutorial) {
        DrawTutorial();
        return;
    }

    ClearBackground((Color){154, 244, 255, 255});

    // Mar //
    float tempoAtual = GetTime();

    int waveOffset = sinf(tempoAtual * 1.5f) * 8;
    DrawTexture(mar1Texture, 0, waveOffset, WHITE);

    int waveOffset2 = sinf(tempoAtual * 1.5f + 1.0f) * 8;
    DrawTexture(mar2Texture, 0, waveOffset2, WHITE);

    // Areia e coqueiros //
    DrawTexture(areiaTexture, 0, 0, WHITE);
    DrawTexture(coqueirosTexture, 0, 0, WHITE);

    DrawCocos();
    DrawPlayer();

    // Tela de bônus dourado //
    if (bonusDourado) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(GOLD, 0.15f));
    }

    // Interface de pontuação e vidas //
    DrawText(TextFormat("Cocos: %i", score), 862, 564, 26, BLACK); // Sombra de contraste
    DrawText(TextFormat("Cocos: %i", score), 860, 562, 26, RAYWHITE);

    DrawText("Vidas: ", 32, 564, 26, BLACK); // Sombra de contraste
    DrawText("Vidas: ", 30, 562, 26, RAYWHITE);

    // Escala para os corações //
    float escalaCoracao = 0.6f;
    for (int i = 0; i < vidas; i++) {
        float espacamentoHorizontal = i * 45.0f;
        Vector2 posicao = { 110.0f + espacamentoHorizontal, 555.0f };
        
        // Sombra do coração para destacar na areia
        DrawTextureEx(coracaoTexture, (Vector2){ posicao.x + 2, posicao.y + 2 }, 0.0f, escalaCoracao, Fade(BLACK, 0.60f));
        
        // Coração principal
        DrawTextureEx(
            coracaoTexture,
            posicao,
            0.0f,
            escalaCoracao,
            WHITE
        );
    }

    // Bônus dourado //
    if (bonusDourado) {
        char textoBonus[30];
        sprintf(textoBonus, "BÔNUS DOURADO: %.0f s", bonusTimer);
        
        int larguraTexto = MeasureText(textoBonus, 28); 
        int xCentralizado = (GetScreenWidth() / 2) - (larguraTexto / 2);

        Color corBrilho = (sinf(GetTime() * 10) > 0) ? GOLD : YELLOW; 
        
        // Sombra do bônus centralizado
        DrawText(textoBonus, xCentralizado + 2, 564, 28, BLACK);
        DrawText(textoBonus, xCentralizado, 562, 28, corBrilho);
    }

    // Tela de Game Over //
    if (gameOver) {
        DrawRectangle(0, 0, 1000, 600, Fade(WHITE, 0.7f));
        DrawText("GAME OVER", 1000/2 - MeasureText("GAME OVER", 80)/2, 150, 80, GOLD);
        
        char scoreText[50];
        sprintf(scoreText, "Score final: %d", score);
        DrawText(scoreText, 1000/2 - MeasureText(scoreText, 60)/2, 300, 60, GOLD);
        DrawText("Press ENTER to restart", 1000/2 - MeasureText("Press ENTER to restart", 40)/2, 450, 40, DARKBLUE);
    }
}

// Carrega o ranking do arquivo //
void CarregarRanking() {
    FILE *arquivo = fopen("ranking.txt", "r");
    if (arquivo == NULL) return;
    for (int i = 0; i < RANKING; i++)
        fscanf(arquivo, "%d", &ranking[i]);
    fclose(arquivo);
}

// Salva o ranking no arquivo //
void SalvarRanking() {
    FILE *arquivo = fopen("ranking.txt", "w");
    if (arquivo == NULL) return;
    for (int i = 0; i < RANKING; i++)
        fprintf(arquivo, "%d\n", ranking[i]);
    fclose(arquivo);
}

// Insere um novo score no ranking, mantendo a ordem //
void InserirNoRanking(int novoScore) {
    for (int i = 0; i < RANKING; i++) {
        if (ranking[i] == novoScore) return;
    }
    for (int i = 0; i < RANKING; i++) {
        if (novoScore > ranking[i]) {
            for (int j = RANKING - 1; j > i; j--)
                ranking[j] = ranking[j-1];
            ranking[i] = novoScore;
            SalvarRanking();
            return;
        }
    }
}

// Desenha a tela de tutorial //
void DrawTutorial() {
    ClearBackground((Color){154, 244, 255, 255});

    float tempoAtual = GetTime();
    int waveOffset1 = sinf(tempoAtual * 1.5f) * 8;
    DrawTexture(mar1Texture, 0, waveOffset1, WHITE);
    int waveOffset2 = sinf((tempoAtual * 1.5f) + 1.0f) * 8;
    DrawTexture(mar2Texture, 0, waveOffset2, WHITE);
    DrawTexture(areiaTexture, 0, 0, WHITE);
    DrawTexture(coqueirosTexture, 0, 0, WHITE);

    // fundo branco semitransparente atras do texto
    DrawRectangle(100, 40, 800, 520, Fade(WHITE, 0.75f));

    DrawText("VAI CATAR COQUINHO",
        1000/2 - MeasureText("VAI CATAR COQUINHO", 60)/2, 50, 60, DARKGREEN);

    DrawText("Como jogar:",
        1000/2 - MeasureText("Como jogar:", 30)/2, 140, 30, BLACK);

    DrawText("Use as setas <- -> para mover o caranguejo",
        1000/2 - MeasureText("Use as setas <- -> para mover o caranguejo", 24)/2, 185, 24, BLACK);

    DrawText("Tipos de itens:",
        1000/2 - MeasureText("Tipos de itens:", 30)/2, 240, 30, BLACK);

    DrawText("Coco verde: +1 ponto",
        1000/2 - MeasureText("Coco verde: +1 ponto", 24)/2, 285, 24, DARKGREEN);

    DrawText("Coco dourado: dobra os pontos por 20 segundos!",
        1000/2 - MeasureText("Coco dourado: dobra os pontos por 20 segundos!", 24)/2, 320, 24, GOLD);

    DrawText("Agua de coco: +1 vida",
        1000/2 - MeasureText("Agua de coco: +1 vida", 24)/2, 355, 24, BLUE);

    DrawText("Lixo: -1 vida",
        1000/2 - MeasureText("Lixo: -1 vida", 24)/2, 390, 24, RED);

    DrawText("Pressione ENTER para comecar!",
        1000/2 - MeasureText("Pressione ENTER para comecar!", 30)/2, 460, 30, DARKBLUE);
}