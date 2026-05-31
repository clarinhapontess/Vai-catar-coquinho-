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
extern Font GasoekOne;
extern Font Rubik;
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
bool naCapa = true; // O jogo começa na capa!

// --- SISTEMA GLOBAL DE SKINS --- //
int skinSelecionada = 0;
int maiorSkinDesbloqueada = 0;
bool temSkinNova = false;
Texture2D texturasSkins[6][5];
const char* nomesSkins[6] = { "player", "maria", "gaiamum", "pirata", "robo", "chico" };

// Adiciona cocos conforme dificuldade aumenta //
int cocosAdicionados = 2;

// Protótipos das funções de ranking e interface //
void DrawTutorial();
void CarregarRecorde();
void SalvarRecorde();

// Inicialização do jogo (carrega texturas, sons, configurações iniciais) //
void InitGame() {
    InitAudioDevice();

    // Música de fundo e áudios
    musicaFundo = LoadMusicStream("assets/audio/aPraieraInstrumental.wav");
    SetMusicVolume(musicaFundo, 0.5f);
    ganhouPontos = LoadSound("assets/audio/ganhouPontos.wav");
    perdeuPontos = LoadSound("assets/audio/perdeuPontos.wav");
    morreu = LoadSound("assets/audio/morreu.wav"); 
    if (FileExists("assets/audio/maisVidas.wav")){
        maisVidas = LoadSound("assets/audio/maisVidas.wav");
    }
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
    if (score >= 10 && cocosAdicionados == 2) {
        AddCoco();
        cocosAdicionados = 3;
    } else if (score >= 25 && cocosAdicionados == 3) {
        AddCoco();
        cocosAdicionados = 4;
    } else if (score >= 50 && cocosAdicionados == 4) {
        AddCoco();
        cocosAdicionados = 5;
    } else if (score >= 80 && cocosAdicionados == 5) {
        AddCoco();
        cocosAdicionados = 6;
    } else if (score >= 120 && cocosAdicionados == 6) {
        AddCoco();
        cocosAdicionados = 7;
    } else if (score >= 200 && cocosAdicionados == 7) {
        AddCoco();
        cocosAdicionados = 8;
    }

    // Player fica mais lento ao perder vidas //
    switch (vidas) {
        case 2:
            playerSpeedMultiplier = 0.65f; 
            break;
        case 1:
            playerSpeedMultiplier = 0.50f; 
            break;
        default:
            playerSpeedMultiplier = 1.0f; 
            break;
    }
}

// Atualização do jogo //
void UpdateGame(float deltaTime) {

    //ADICIONE ESSE BLOCO AQUI NO INÍCIO:
    if (naCapa) {
        if (IsKeyReleased(KEY_ENTER)) {
            naCapa = false;
            naHistoria = true; // Avança para a história
        }
        return;
    }

    if (isPaused) {
        UpdateMusicStream(musicaFundo);  
        return;
    }  
    
    if (tutorial) {
        // Seleção de skins (Navega por TODAS as 6 skins para exibir o cadeado)
        if (IsKeyPressed(KEY_UP)) {
            skinSelecionada++;
            if (skinSelecionada > 5) skinSelecionada = 0;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            skinSelecionada--;
            if (skinSelecionada < 0) skinSelecionada = 5;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            // Só deixa começar se a skin selecionada estiver liberada!
            if (skinSelecionada <= maiorSkinDesbloqueada) {
                tutorial = false;
                if (musicaFundo.stream.buffer != NULL) {
                    PlayMusicStream(musicaFundo);
                    SeekMusicStream(musicaFundo, 7.0f); 
                }
            } else {
                PlaySound(perdeuPontos); // Som de erro ao tentar escolher trancado
            }
        }
        return;  
    }

    UpdateMusicStream(musicaFundo);

    // Restart //
    if (gameOver && IsKeyPressed(KEY_ENTER)) {
        gameOver = false;
        tutorial = true;
        score = 0;
        vidas = 3;
        bonusDourado = false;
        bonusTimer = 0;
        gameTimer = 0;
        cocosAdicionados = 2;

        ClearCocos(); 
        InitCocos(); 
        InitPlayer(); 

        if (musicaFundo.stream.buffer != NULL) {
            PlayMusicStream(musicaFundo);
            SeekMusicStream(musicaFundo, 7.0f);
        }
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
                        if (vidas > 0) vidas--;
                        bonusDourado = false;
                    }
                    // Coco dourado //
                    else if (atual->coco.type == 2) {
                        PlaySound(ganhouPontos);
                        bonusDourado = true;
                        bonusTimer = 20.0f; 
                    }
                    // Água de coco //
                    else if (atual->coco.type == 3) {
                        if (maisVidas.frameCount > 0) PlaySound(maisVidas);
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

    if (naCapa) {
        DrawCapaScreen();
        return;
    }
    
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

        const char *instructionText = "Pressione P para continuar";
        int instFontSize = 30;
        int instWidth = MeasureText(instructionText, instFontSize);
        DrawText(instructionText, (1000 - instWidth) / 2, 300, instFontSize, GOLD);
      
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

    // --- INTERFACE DE PONTUAÇÃO E VIDAS (SÓ DESENHA SE NÃO FOR GAME OVER) --- //
    if (!gameOver) {
        float spacingRubik = 1.0f;
        
        // Sombra do Score
        DrawTextEx(Rubik, TextFormat("Cocos: %i", score), (Vector2){832, 559}, 30, spacingRubik, BLACK); 
        // Texto do Score
        DrawTextEx(Rubik, TextFormat("Cocos: %i", score), (Vector2){830, 557}, 30, spacingRubik, RAYWHITE);

        // Sombra do texto Vidas
        DrawTextEx(Rubik, "Vidas: ", (Vector2){32, 559}, 30, spacingRubik, BLACK); 
        // Texto do texto Vidas
        DrawTextEx(Rubik, "Vidas: ", (Vector2){30, 557}, 30, spacingRubik, RAYWHITE);

        // Desenho dos corações
        float escalaCoracao = 0.6f;
        for (int i = 0; i < vidas; i++) {
            float espacamentoHorizontal = i * 45.0f;
            Vector2 posicao = { 130.0f + espacamentoHorizontal, 555.0f }; 
            DrawTextureEx(coracaoTexture, (Vector2){ posicao.x + 2, posicao.y + 2 }, 0.0f, escalaCoracao, Fade(BLACK, 0.60f));
            DrawTextureEx(coracaoTexture, posicao, 0.0f, escalaCoracao, WHITE);
        }
    }

    // Bônus dourado centralizado //
    if (bonusDourado && !gameOver) {
        char textoBonus[30];
        sprintf(textoBonus, "BÔNUS DOURADO: %.0f s", bonusTimer);
        int larguraTexto = MeasureText(textoBonus, 28); 
        int xCentralizado = (GetScreenWidth() / 2) - (larguraTexto / 2);
        Color corBrilho = (sinf(GetTime() * 10) > 0) ? GOLD : YELLOW; 
        
        DrawText(textoBonus, xCentralizado + 2, 564, 28, BLACK);
        DrawText(textoBonus, xCentralizado, 562, 28, corBrilho);
    }

    if (!gameOver) {
        float spacingRubik = 1.0f;
        // Desenha uma sombrinha preta bem sutil por trás para garantir a leitura na água
        DrawTextEx(Rubik, "Pressione P para pausar", (Vector2){16, 16}, 16, spacingRubik, Fade(BLACK, 0.5f));
        // Texto principal em branco semitransparente para ficar bem discreto
        DrawTextEx(Rubik, "Pressione P para pausar", (Vector2){15, 15}, 16, spacingRubik, Fade(RAYWHITE, 0.8f));
    }

    // --- PAINEL DE GAME OVER --- //
    if (gameOver) {
       DrawGameOverScreen(score, recorde, skinSelecionada, texturasSkins);
    }
} // 👈 Chave adicionada aqui para fechar a DrawGame() corretamente!

// Desenho do tutorial e Seletor //
void DrawTutorial() {
    DrawTutorialScreen(skinSelecionada, maiorSkinDesbloqueada, texturasSkins, nomesSkins, mar1Texture, mar2Texture, areiaTexture, coqueirosTexture);
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