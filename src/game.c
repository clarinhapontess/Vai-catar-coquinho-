#include "game.h"
#include "player.h"
#include "coco.h"
#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "screens.h"

// Ranking //
int recorde = 0;

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

// Variáveis globais do estado do jogo //
int score = 0;
int vidas = 3;
bool bonusDourado = false;
float bonusTimer = 0;
bool gameOver = false;
float gameTimer = 0;
bool isPaused = false;  
bool tutorial = true;
float playerSpeedMultiplier = 1.0f; 
float cocoSpeedMultiplier = 1.0f;
extern bool naHistoria;

// --- SISTEMA GLOBAL DE SKINS --- //
int skinSelecionada = 0;
int maiorSkinDesbloqueada = 0;
Texture2D texturasSkins[6][5];
const char* nomesSkins[6] = { "player", "maria", "gaiamum", "pirata", "robo", "chico" };

// Adiciona cocos conforme dificuldade aumenta //
int cocosAdicionados = 2;

// Protótipos das funções de ranking e interface //
void DrawTutorial();

// Inicialização do jogo (carrega texturas, sons, configurações iniciais) //
void InitGame() {
    InitAudioDevice();

    // Música de fundo e áudios
    musicaFundo = LoadMusicStream("assets/audio/aPraieraInstrumental.wav");
    SetMusicVolume(musicaFundo, 0.5f);
    ganhouPontos = LoadSound("assets/audio/ganhouPontos.wav");
    perdeuPontos = LoadSound("assets/audio/perdeuPontos.wav");
    morreu = LoadSound("assets/audio/morreu.wav"); 
    maisVidas = LoadSound("assets/audio/maisVidas.wav");

    // --- 1º CARREGA AS TEXTURAS DAS SKINS --- //
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 5; j++) {
            char caminho[100];
            sprintf(caminho, "assets/%s/%s%d.png", nomesSkins[i], nomesSkins[i], j + 1);
            texturasSkins[i][j] = LoadTexture(caminho);
        }
    }

    // Carrega o recorde e define o que está desbloqueado
    CarregarRecorde();
    if (recorde >= 200)      maiorSkinDesbloqueada = 5;
    else if (recorde >= 120) maiorSkinDesbloqueada = 4;
    else if (recorde >= 80)  maiorSkinDesbloqueada = 3;
    else if (recorde >= 50)  maiorSkinDesbloqueada = 2;
    else if (recorde >= 20)  maiorSkinDesbloqueada = 1;
    else                     maiorSkinDesbloqueada = 0;

    // --- 2º INICIALIZA OS ELEMENTOS DO JOGO --- //
    InitScreens();
    InitPlayer();   // Agora o Player inicia sabendo que as texturas existem!
    InitCocos(); 

    // Carrega as texturas de ambiente //
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
    cocosAdicionados = 2;
    playerSpeedMultiplier = 1.0f; 
}

// Atualização da dificuldade do jogo //
void UpdateGameProgression(float deltaTime) {
    gameTimer += deltaTime;
    extern void AddCoco(); 
    
    // Sistema de progressão da branch danda //
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
            playerSpeedMultiplier = 0.6f; 
            break;
        case 1:
            playerSpeedMultiplier = 0.4f; 
            break;
        default:
            playerSpeedMultiplier = 1.0f; 
            break;
    }
}

// Atualização do jogo //
void UpdateGame(float deltaTime) {
    if (isPaused) {
        UpdateMusicStream(musicaFundo);  
        return;
    }  
    
    if (tutorial) {
        // Seleção de skins liberadas usando as setas CIMA e BAIXO
        if (IsKeyPressed(KEY_UP)) {
            skinSelecionada++;
            if (skinSelecionada > maiorSkinDesbloqueada) skinSelecionada = 0;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            skinSelecionada--;
            if (skinSelecionada < 0) skinSelecionada = maiorSkinDesbloqueada;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            tutorial = false;
            PlayMusicStream(musicaFundo);
            SeekMusicStream(musicaFundo, 7.0f); 
        }
        return;  
    }

    UpdateMusicStream(musicaFundo);

    // Restart //
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
        SeekMusicStream(musicaFundo, 7.0f);
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
                // Colisão apenas por cima //
                if (cocoCenter.y < playerRect.y + playerRect.height * 0.3f) {
                    
                    // Coco normal //
                    if (atual->coco.type == 0) {
                        PlaySound(ganhouPontos);
                        if (bonusDourado) score += 2;
                        else score += 1;
                    }
                    // Lixo //
                    else if (atual->coco.type == 1) {
                        PlaySound(perdeuPontos);
                        score -= 1;
                        if (score < 0) score = 0;
                        if (!bonusDourado) vidas--; 
                    }
                    // Coco dourado //
                    else if (atual->coco.type == 2) {
                        PlaySound(ganhouPontos);
                        bonusDourado = true;
                        bonusTimer = 20.0f; 
                    }
                    // Água de coco //
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
        if (score > recorde) {
            recorde = score;
            SalvarRecorde();
            
            // Recalcula o teto de skins caso quebre o recorde geral na rodada
            if (recorde >= 200)      maiorSkinDesbloqueada = 5;
            else if (recorde >= 120) maiorSkinDesbloqueada = 4;
            else if (recorde >= 80)  maiorSkinDesbloqueada = 3;
            else if (recorde >= 50)  maiorSkinDesbloqueada = 2;
            else if (recorde >= 20)  maiorSkinDesbloqueada = 1;
        }
    }
}

// Desenho do jogo //
void DrawGame() {
    // Tela de história (Overlay com texto e imagem) //
    if (naHistoria) {
        UpdateHistory();  
        DrawHistory();    
        return;
    }

    // Tela de tutorial //
    if (tutorial) {
        DrawTutorial();
        return;
    } 

    ClearBackground((Color){154, 244, 255, 255});

    // Tela de pause (overlay)
    if (isPaused) {
        DrawRectangle(0, 0, 1000, 600, Fade(WHITE, 0.6f));

        const char *pauseText = "PAUSADO";
        int pauseFontSize = 80;
        int pauseWidth = MeasureText(pauseText, pauseFontSize);
        DrawText(pauseText, (1000 - pauseWidth) / 2, 150, pauseFontSize, RED);

        const char *instructionText = "Pressione P ou ESC para continuar";
        int instFontSize = 30;
        int instWidth = MeasureText(instructionText, instFontSize);
        DrawText(instructionText, (1000 - instWidth) / 2, 300, instFontSize, WHITE);

        return;  
    }

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

    // Tela de bônus dourado (Overlay translúcido) //
    if (bonusDourado) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(GOLD, 0.15f));
    }

    // --- INTERFACE DE PONTUAÇÃO E VIDAS --- //
    DrawText(TextFormat("Cocos: %i", score), 832, 559, 34, BLACK); 
    DrawText(TextFormat("Cocos: %i", score), 830, 557, 34, RAYWHITE);

    DrawText("Vidas: ", 32, 559, 34, BLACK); 
    DrawText("Vidas: ", 30, 557, 34, RAYWHITE);

    // Desenho dos corações
    float escalaCoracao = 0.6f;
    for (int i = 0; i < vidas; i++) {
        float espacamentoHorizontal = i * 45.0f;
        Vector2 posicao = { 140.0f + espacamentoHorizontal, 555.0f }; 
        DrawTextureEx(coracaoTexture, (Vector2){ posicao.x + 2, posicao.y + 2 }, 0.0f, escalaCoracao, Fade(BLACK, 0.60f));
        DrawTextureEx(coracaoTexture, posicao, 0.0f, escalaCoracao, WHITE);
    }

    // Bônus dourado centralizado //
    if (bonusDourado) {
        char textoBonus[30];
        sprintf(textoBonus, "BÔNUS DOURADO: %.0f s", bonusTimer);
        int larguraTexto = MeasureText(textoBonus, 28); 
        int xCentralizado = (GetScreenWidth() / 2) - (larguraTexto / 2);
        Color corBrilho = (sinf(GetTime() * 10) > 0) ? GOLD : YELLOW; 
        
        DrawText(textoBonus, xCentralizado + 2, 564, 28, BLACK);
        DrawText(textoBonus, xCentralizado, 562, 28, corBrilho);
    }

    // --- PAINEL DE GAME OVER --- //
    // --- PAINEL DE GAME OVER --- //
    if (gameOver) { 
        const int rectWidth = 600;
        const int rectHeight = 400; // Altura ideal para caber tudo com o caranguejo embaixo
        const int rectX = (1000 - rectWidth) / 2;      
        const int rectY = (600 - rectHeight) / 2;      

        DrawRectangle(rectX, rectY, rectWidth, rectHeight, Fade(WHITE, 0.8f));

        // Posições verticais (Y) fixas para garantir que nada suma ou quebre
        int yTitulo     = rectY + 30;
        int yScore      = rectY + 110;
        int yRecorde    = rectY + 170;
        int yRestart    = rectY + 230;
        int yCaranguejo = rectY + 300; // Posição perfeita na parte de baixo do retângulo

        // Título: GAME OVER
        const char *title = "GAME OVER";
        int titleWidth = MeasureText(title, 55);
        DrawText(title, rectX + (rectWidth - titleWidth) / 2, yTitulo, 55, RED);

        // Score final
        char scoreText[50];
        sprintf(scoreText, "Score final: %d", score);
        int scoreWidth = MeasureText(scoreText, 36);
        DrawText(scoreText, rectX + (rectWidth - scoreWidth) / 2, yScore, 36, BLACK);

        // Recorde
        char recordeText[50];
        sprintf(recordeText, "Recorde Máximo: %d", recorde);
        int recordeWidth = MeasureText(recordeText, 36);
        DrawText(recordeText, rectX + (rectWidth - recordeWidth) / 2, yRecorde, 36, GOLD);

        // Instrução de reinício
        const char *restartText = "Pressione ENTER para reiniciar!";
        int restartWidth = MeasureText(restartText, 26);
        DrawText(restartText, rectX + (rectWidth - restartWidth) / 2, yRestart, 26, DARKBLUE);

        float caranguejoGameOverX = 425.0f; // Aumente para ir para a direita, diminua para ir para a esquerda
        float caranguejoGameOverY = 360.0f; // Aumente para ir para baixo, diminua para ir para cima

        // Desenha usando os valores manuais acima
        Vector2 posChorando = { caranguejoGameOverX, caranguejoGameOverY }; 
        DrawTextureEx(texturasSkins[skinSelecionada][4], posChorando, 0.0f, 1.0f, WHITE);
    }
}

// Funções de ranking //
void CarregarRecorde() {
    FILE *arquivo = fopen("recorde.txt", "r");
    if (arquivo == NULL) return;
    fscanf(arquivo, "%d", &recorde);
    fclose(arquivo);
}

void SalvarRecorde() {
    FILE *arquivo = fopen("recorde.txt", "w");
    if (arquivo == NULL) return;
    fprintf(arquivo, "%d\n", recorde);
    fclose(arquivo);
}

// Desenho do tutorial e Seletor //
void DrawTutorial() {
    ClearBackground((Color){154, 244, 255, 255});

    float tempoAtual = GetTime();
    int waveOffset1 = sinf(tempoAtual * 1.5f) * 8;
    DrawTexture(mar1Texture, 0, waveOffset1, WHITE);
    int waveOffset2 = sinf((tempoAtual * 1.5f) + 1.0f) * 8;
    DrawTexture(mar2Texture, 0, waveOffset2, WHITE);
    DrawTexture(areiaTexture, 0, 0, WHITE);
    DrawTexture(coqueirosTexture, 0, 0, WHITE);

    // Fundo branco semitransparente atrás do texto //
    DrawRectangle(100, 30, 800, 540, Fade(WHITE, 0.75f));

    DrawText("VAI CATAR COQUINHO", 1000/2 - MeasureText("VAI CATAR COQUINHO", 50)/2, 45, 50, DARKGREEN);
    DrawText("Como jogar:", 1000/2 - MeasureText("Como jogar:", 26)/2, 115, 26, BLACK);
    DrawText("Use as setas <- -> para mover o caranguejo", 1000/2 - MeasureText("Use as setas <- -> para mover o caranguejo", 22)/2, 150, 22, BLACK);
    
    DrawText("Tipos de itens:", 1000/2 - MeasureText("Tipos de itens:", 26)/2, 195, 26, BLACK);
    DrawText("Coco verde: +1 ponto", 1000/2 - MeasureText("Coco verde: +1 ponto", 22)/2, 230, 22, DARKGREEN);
    DrawText("Coco dourado: dobra os pontos por 20 segundos!", 1000/2 - MeasureText("Coco dourado: dobra os pontos por 20 segundos!", 22)/2, 255, 22, GOLD);
    DrawText("Agua de coco: +1 vida", 1000/2 - MeasureText("Agua de coco: +1 vida", 22)/2, 280, 22, BLUE);
    DrawText("Lixo: -1 vida", 1000/2 - MeasureText("Lixo: -1 vida", 22)/2, 305, 22, RED);
    
    // --- EXIBIÇÃO DA SKIN SELECIONADA (AJUSTADO) --- //
    DrawText("Escolha seu Visual (Setas de CIMA / BAIXO):", 1000/2 - MeasureText("Escolha seu Visual (Setas de CIMA / BAIXO):", 24)/2, 345, 24, BLACK);
    
    // Subi um pouco o Y para 370 e centralizei baseado no tamanho 64x64
    Vector2 posSkinMenu = { (1000 / 2) - 60, 370 };
    
    // Desenha com escala //
    DrawTextureEx(texturasSkins[skinSelecionada][0], posSkinMenu, 0.0f, 1.0f, WHITE);

    char textoSkin[60];
    sprintf(textoSkin, "< Skin: %s >", nomesSkins[skinSelecionada]);
    DrawText(textoSkin, 1000/2 - MeasureText(textoSkin, 24)/2, 490, 24, ORANGE);

    DrawText("Pressione ENTER para comecar!", 1000/2 - MeasureText("Pressione ENTER para comecar!", 26)/2, 530, 26, DARKBLUE);
}