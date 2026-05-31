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
                SeekMusicStream(musicaFundo, 7.0f); }
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
    if (gameOver) {
        // Fundo semitransparente cobrindo a tela
        DrawRectangle(0, 0, 1000, 600, Fade(WHITE, 0.7f));

        // Tamanhos de fonte proporcionais baseados na GasoekOne
        float gameOverFontSize = 120.0f;           // Base
        float scoreFontSize = 84.0f;               // 30% menor
        float recordeFontSize = 60.0f;             // 50% menor
        float instructionFontSize = 35.0f;         // Instrução

        // Spacing com borda
        float gameOverSpacing = 1.2f;
        float scoreSpacing = 1.0f;
        float recordeSpacing = 0.9f;
        float instructionSpacing = 0.75f;

        // Montagem dos textos
        const char *gameOverText = "GAME OVER";
        char scoreText[50];
        sprintf(scoreText, "Score final: %d", score);
        char recordeText[50];
        sprintf(recordeText, "Maior Recorde: %d", recorde);
        const char *instructionText = "Pressione ENTER para reiniciar!";

        // Medir tamanhos exatos com DrawTextEx
        Vector2 gameOverSize = MeasureTextEx(GasoekOne, gameOverText, gameOverFontSize, gameOverSpacing);
        Vector2 scoreSize = MeasureTextEx(GasoekOne, scoreText, scoreFontSize, scoreSpacing);
        Vector2 recordeSize = MeasureTextEx(GasoekOne, recordeText, recordeFontSize, recordeSpacing);
        Vector2 instructionSize = MeasureTextEx(GasoekOne, instructionText, instructionFontSize, instructionSpacing);

        // Posições verticais e horizontais centralizadas na tela
        Vector2 gameOverPos = { (1000 - gameOverSize.x) / 2.0f, 60.0f };
        Vector2 scorePos    = { (1000 - scoreSize.x) / 2.0f, 200.0f };
        Vector2 recordePos  = { (1000 - recordeSize.x) / 2.0f, 310.0f };
        Vector2 instructionPos = { (1000 - instructionSize.x) / 2.0f, 430.0f };

        // 1. Desenhar GAME OVER com borda preta (Texto principal: GOLD)
        for (int dx = -2; dx <= 2; dx++) {
            for (int dy = -2; dy <= 2; dy++) {
                if (dx != 0 || dy != 0) {
                    DrawTextEx(GasoekOne, gameOverText, (Vector2){gameOverPos.x + dx, gameOverPos.y + dy}, gameOverFontSize, gameOverSpacing, BLACK);
                }
            }
        }
        DrawTextEx(GasoekOne, gameOverText, gameOverPos, gameOverFontSize, gameOverSpacing, GOLD);

        // 2. Desenhar Score com borda preta (Texto principal: WHITE)
        for (int dx = -2; dx <= 2; dx++) {
            for (int dy = -2; dy <= 2; dy++) {
                if (dx != 0 || dy != 0) {
                    DrawTextEx(GasoekOne, scoreText, (Vector2){scorePos.x + dx, scorePos.y + dy}, scoreFontSize, scoreSpacing, BLACK);
                }
            }
        }
        DrawTextEx(GasoekOne, scoreText, scorePos, scoreFontSize, scoreSpacing, WHITE);

        // 3. Desenhar Recorde com borda preta (Texto principal: RED)
        for (int dx = -2; dx <= 2; dx++) {
            for (int dy = -2; dy <= 2; dy++) {
                if (dx != 0 || dy != 0) {
                    DrawTextEx(GasoekOne, recordeText, (Vector2){recordePos.x + dx, recordePos.y + dy}, recordeFontSize, recordeSpacing, BLACK);
                }
            }
        }
        DrawTextEx(GasoekOne, recordeText, recordePos, recordeFontSize, recordeSpacing, RED);

        // 4. Desenhar Instrução com borda preta (Texto principal: WHITE)
        for (int dx = -2; dx <= 2; dx++) {
            for (int dy = -2; dy <= 2; dy++) {
                if (dx != 0 || dy != 0) {
                    DrawTextEx(GasoekOne, instructionText, (Vector2){instructionPos.x + dx, instructionPos.y + dy}, instructionFontSize, instructionSpacing, BLACK);
                }
            }
        }
        DrawTextEx(GasoekOne, instructionText, instructionPos, instructionFontSize, instructionSpacing, WHITE);

        // 🛠️ AJUSTE MANUAL DO CARANGUEJO CHORANDO (Embaixo de todos os textos)
        float caranguejoGameOverX = 468.0f; 
        float caranguejoGameOverY = 500.0f; 

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

    // --- MUDANÇA APENAS DE FONTE (USANDO GASOEKONE NOS TÍTULOS E BOTÕES) --- //
    const char *tituloTexto = "VAI CATAR COQUINHO";
    float tituloSize = 46.0f;
    Vector2 tituloMedidas = MeasureTextEx(GasoekOne, tituloTexto, tituloSize, 1.0f);
    DrawTextEx(GasoekOne, tituloTexto, (Vector2){(1000 - tituloMedidas.x)/2, 45}, tituloSize, 1.0f, DARKGREEN);

    // Textos informativos menores permanecem legíveis com fonte padrão limpa
    DrawText("Como jogar:", 1000/2 - MeasureText("Como jogar:", 26)/2, 115, 26, BLACK);
    DrawText("Use as setas <- -> para mover o caranguejo", 1000/2 - MeasureText("Use as setas <- -> para mover o caranguejo", 22)/2, 150, 22, BLACK);
    
    DrawText("Tipos de itens:", 1000/2 - MeasureText("Tipos de itens:", 26)/2, 195, 26, BLACK);
    DrawText("Coco verde: +1 ponto", 1000/2 - MeasureText("Coco verde: +1 ponto", 22)/2, 230, 22, DARKGREEN);
    DrawText("Coco dourado: dobra os pontos por 20 segundos!", 1000/2 - MeasureText("Coco dourado: dobra os pontos por 20 segundos!", 22)/2, 255, 22, GOLD);
    DrawText("Agua de coco: +1 vida", 1000/2 - MeasureText("Agua de coco: +1 vida", 22)/2, 280, 22, BLUE);
    DrawText("Lixo: -1 vida", 1000/2 - MeasureText("Lixo: -1 vida", 22)/2, 305, 22, RED);
    
    // --- EXIBIÇÃO DA SKIN SELECIONADA --- //
    const char *instrucaoSkin = "Escolha seu Visual (Setas de CIMA / BAIXO):";
    DrawText(instrucaoSkin, 1000/2 - MeasureText(instrucaoSkin, 24)/2, 345, 24, BLACK);
    
    float skinMenuX = 435.0f; 
    float skinMenuY = 360.0f; 
    
    bool estaBloqueada = (skinSelecionada > maiorSkinDesbloqueada);

    if (estaBloqueada) {
        DrawTexture(texturasSkins[skinSelecionada][0], (int)skinMenuX, (int)skinMenuY, GRAY);
    } else {
        DrawTexture(texturasSkins[skinSelecionada][0], (int)skinMenuX, (int)skinMenuY, WHITE);
    }

    char textoSkin[60];
    if (estaBloqueada) {
        sprintf(textoSkin, "< %s (Bloqueado) >", nomesSkins[skinSelecionada]);
        DrawText(textoSkin, 1000/2 - MeasureText(textoSkin, 24)/2, 480, 24, RED);
    } else {
        sprintf(textoSkin, "< Skin: %s >", nomesSkins[skinSelecionada]);
        DrawText(textoSkin, 1000/2 - MeasureText(textoSkin, 24)/2, 480, 24, ORANGE);
    }

    // Botão de Iniciar usando a GasoekOne
    const char *botaoTexto = "Pressione ENTER para comecar!";
    float botaoSize = 24.0f;
    Vector2 botaoMedidas = MeasureTextEx(GasoekOne, botaoTexto, botaoSize, 1.0f);
    DrawTextEx(GasoekOne, botaoTexto, (Vector2){(1000 - botaoMedidas.x)/2, 530}, botaoSize, 1.0f, DARKBLUE);
}