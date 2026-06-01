#include "game.h"
#include "player.h"
#include "coco.h"
#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "screens.h"

// ranking
extern Font GasoekOne;
extern Font Rubik;
int recorde = 0;

// texturas background
Texture2D areiaTexture;
Texture2D mar1Texture;
Texture2D mar2Texture;
Texture2D coqueirosTexture;
Texture2D coracaoTexture;

// musica e efeitos sonoros
Music musicaFundo; 
Sound ganhouPontos;
Sound perdeuPontos;
Sound morreu;
Sound maisVidas;

// globais
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
bool naCapa = true; // para o jogo começar na capa

// variáveis da funcionalidade de mutar jogo 
bool isMuted = false;
float volumeAnterior = 0.5f;
// skins
int skinSelecionada = 0;
int maiorSkinDesbloqueada = 0;
bool temSkinNova = false;
Texture2D texturasSkins[6][5];
const char* nomesSkins[6] = { "player", "maria", "gaiamum", "pirata", "robo", "chico" };

// adiciona os cocos pela dificuldade
int cocosAdicionados = 3;

void DrawTutorial();
void CarregarRecorde();
void SalvarRecorde();

//função de mutar volume 
void ToggleMute() {
    isMuted = !isMuted;
    //quando a função togle é chamada , isMuted recebe o valor booleano oposto do que ele já é 
    if (isMuted) {
        SetMusicVolume(musicaFundo, 0.0f);
        SetSoundVolume(ganhouPontos, 0.0f);
        SetSoundVolume(perdeuPontos, 0.0f);
        SetSoundVolume(morreu, 0.0f);
        SetSoundVolume(maisVidas, 0.0f);
    } else {
        SetMusicVolume(musicaFundo, volumeAnterior);
        SetSoundVolume(ganhouPontos, 1.0f);
        SetSoundVolume(perdeuPontos, 1.0f);
        SetSoundVolume(morreu, 1.0f);
        SetSoundVolume(maisVidas, 1.0f);
    }
}
// inicialização do jogo
void InitGame() {
    InitAudioDevice();

    // musica de fundo e audios
    musicaFundo = LoadMusicStream("assets/audio/aPraieraInstrumental.wav");
    SetMusicVolume(musicaFundo, 0.5f);
    ganhouPontos = LoadSound("assets/audio/ganhouPontos.wav");
    perdeuPontos = LoadSound("assets/audio/perdeuPontos.wav");
    morreu = LoadSound("assets/audio/morreu.wav"); 
    if (FileExists("assets/audio/maisVidas.wav")){
        maisVidas = LoadSound("assets/audio/maisVidas.wav");
    }
 
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 5; j++) {
            char caminho[100];
            sprintf(caminho, "assets/%s/%s%d.png", nomesSkins[i], nomesSkins[i], j + 1);
            texturasSkins[i][j] = LoadTexture(caminho);
        }
    }

    // recordes para desbloquear skins
    CarregarRecorde();
    if (recorde >= 500)      maiorSkinDesbloqueada = 5;
    else if (recorde >= 300) maiorSkinDesbloqueada = 4;
    else if (recorde >= 180)  maiorSkinDesbloqueada = 3;
    else if (recorde >= 100)  maiorSkinDesbloqueada = 2;
    else if (recorde >= 50)  maiorSkinDesbloqueada = 1;
    else                     maiorSkinDesbloqueada = 0;

    InitScreens();
    InitPlayer();  
    InitCocos(); 

    //texturas do ambiente
    areiaTexture = LoadTexture("assets/backgrounds/areia.png");
    mar1Texture = LoadTexture("assets/backgrounds/mar1.png");
    mar2Texture = LoadTexture("assets/backgrounds/mar2.png");
    coqueirosTexture = LoadTexture("assets/backgrounds/coqueiros.png");
    coracaoTexture = LoadTexture("assets/backgrounds/coracao.png");

    // variaveis
    score = 0;
    vidas = 3;
    bonusDourado = false;
    bonusTimer = 0;
    gameOver = false;
    gameTimer = 0;
    cocosAdicionados = 3;
    playerSpeedMultiplier = 1.0f; 
}

// atualização da dificuldade
void UpdateGameProgression(float deltaTime) {
    gameTimer += deltaTime;
    extern void AddCoco(); 
   
    if (score >= 10 && cocosAdicionados == 3) {
        AddCoco();
        cocosAdicionados = 4;
    } else if (score >= 25 && cocosAdicionados == 4) {
        AddCoco();
        AddCoco();
        cocosAdicionados = 6;
    } else if (score >= 50 && cocosAdicionados == 6) {
        AddCoco();
        cocosAdicionados = 7;
    } else if (score >= 80 && cocosAdicionados == 7) {
        AddCoco(); 
        cocosAdicionados = 8;
    } else if (score >= 120 && cocosAdicionados == 8) {
        AddCoco(); 
        AddCoco();
        cocosAdicionados = 10;
    } else if (score >= 200 && cocosAdicionados == 10) {
        AddCoco();
        AddCoco();
        cocosAdicionados = 12;
    }

    // jogador fica lento quando perde vidas
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

// atualização do jogo
void UpdateGame(float deltaTime) {

    if (naCapa) {
        if (IsKeyReleased(KEY_ENTER)) {
            naCapa = false;
            naHistoria = true; 
        }
        return;
    }

    if (isPaused) {
        UpdateMusicStream(musicaFundo);  
        return;
    }  
    
    if (tutorial) {
        // seleção de skins
        if (IsKeyPressed(KEY_UP)) {
            skinSelecionada++;
            if (skinSelecionada > 5) skinSelecionada = 0;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            skinSelecionada--;
            if (skinSelecionada < 0) skinSelecionada = 5;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            
            if (skinSelecionada <= maiorSkinDesbloqueada) { // garante que a skin so esteja desbloqueada de acordo com a pontuação
                tutorial = false;
                if (musicaFundo.stream.buffer != NULL) {
                    PlayMusicStream(musicaFundo);
                    SeekMusicStream(musicaFundo, 7.0f); 
                }
            } else {
                PlaySound(perdeuPontos); 
            }
        }
        return;  
    }

    UpdateMusicStream(musicaFundo);

    // restart
    if (gameOver && IsKeyPressed(KEY_ENTER)) {
        gameOver = false;
        tutorial = true;
        score = 0;
        vidas = 3;
        bonusDourado = false;
        bonusTimer = 0;
        gameTimer = 0;
        cocosAdicionados = 2;
        temSkinNova = false;

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

    // timer atualizado
    if (bonusDourado) {
        bonusTimer -= GetFrameTime();
        if (bonusTimer <= 0) {
            bonusDourado = false;
        }
    }

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
                if (cocoCenter.y < playerRect.y + playerRect.height * 0.3f) {
                    
                    // coco normal
                    if (atual->coco.type == 0) {
                        PlaySound(ganhouPontos);
                        if (bonusDourado) score += 2;
                        else score += 1;
                    }
                    // lixo
                    else if (atual->coco.type == 1) {
                        PlaySound(perdeuPontos);
                        if (vidas > 0) vidas--;
                        bonusDourado = false;
                    }
                    // coco dourado
                    else if (atual->coco.type == 2) {
                        PlaySound(ganhouPontos);
                        bonusDourado = true;
                        bonusTimer = 20.0f; 
                    }
                    // agua de coco
                    else if (atual->coco.type == 3) {
                        if (maisVidas.frameCount > 0) PlaySound(maisVidas);
                        if (vidas < 3) vidas++;
                    }

                    // coco volta para cima em posição aleatoria
                    atual->coco.y = GetRandomValue(-400, -30);
                    atual->coco.x = GetRandomValue(50, 750);

                    extern void SetCocoType(Coco *coco); 
                    SetCocoType(&atual->coco); 
                }
            }
            atual = atual->next; 
        }
    }

    // game over
    if (vidas <= 0 && !gameOver) {
        PlaySound(morreu);
        gameOver = true;
        StopMusicStream(musicaFundo);
        if (score > recorde) {
            recorde = score;
            SalvarRecorde();
        }
            int antigaMaiorSkin = maiorSkinDesbloqueada; // guarda valor antigo para comparar

            if (recorde >= 200)      maiorSkinDesbloqueada = 5;
            else if (recorde >= 120) maiorSkinDesbloqueada = 4;
            else if (recorde >= 80)  maiorSkinDesbloqueada = 3;
            else if (recorde >= 50)  maiorSkinDesbloqueada = 2;
            else if (recorde >= 20)  maiorSkinDesbloqueada = 1;
            else                    maiorSkinDesbloqueada = 0;
            
            if (maiorSkinDesbloqueada > antigaMaiorSkin) { // se desbloqueou uma nova skin aparece no game over
                temSkinNova = true;
            }
        }
}

// desenho do jogo
void DrawGame() {

    if (naCapa) {
        DrawCapaScreen();
        return;
    }
    
    // historia
    if (naHistoria) {
        UpdateHistory();  
        DrawHistory();    
        return;
    }

    // tutorial
    if (tutorial) {
        DrawTutorial();
        return;
    } 

    ClearBackground((Color){154, 244, 255, 255});

    // pause
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

    // mar
    float tempoAtual = GetTime();
    int waveOffset = sinf(tempoAtual * 1.5f) * 8;
    DrawTexture(mar1Texture, 0, waveOffset, WHITE);

    int waveOffset2 = sinf(tempoAtual * 1.5f + 1.0f) * 8;
    DrawTexture(mar2Texture, 0, waveOffset2, WHITE);

    // areia e coqueiros
    DrawTexture(areiaTexture, 0, 0, WHITE);
    DrawTexture(coqueirosTexture, 0, 0, WHITE);

    DrawCocos();
    DrawPlayer();

    // tela do coco dourado
    if (bonusDourado) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(GOLD, 0.15f));
    }

    if (!gameOver) {
        float spacingRubik = 1.0f;
        // score
        DrawTextEx(Rubik, TextFormat("Cocos: %i", score), (Vector2){832, 559}, 30, spacingRubik, BLACK); 
        DrawTextEx(Rubik, TextFormat("Cocos: %i", score), (Vector2){830, 557}, 30, spacingRubik, RAYWHITE);
        // vidas
        DrawTextEx(Rubik, "Vidas: ", (Vector2){32, 559}, 30, spacingRubik, BLACK); 
        DrawTextEx(Rubik, "Vidas: ", (Vector2){30, 557}, 30, spacingRubik, RAYWHITE);

        // corações
        float escalaCoracao = 0.6f;
        for (int i = 0; i < vidas; i++) {
            float espacamentoHorizontal = i * 45.0f;
            Vector2 posicao = { 130.0f + espacamentoHorizontal, 555.0f }; 
            DrawTextureEx(coracaoTexture, (Vector2){ posicao.x + 2, posicao.y + 2 }, 0.0f, escalaCoracao, Fade(BLACK, 0.60f));
            DrawTextureEx(coracaoTexture, posicao, 0.0f, escalaCoracao, WHITE);
        }
    }

    // bonus dourado
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
        // pause
        DrawTextEx(Rubik, "Pressione P para pausar", (Vector2){16, 16}, 16, spacingRubik, Fade(BLACK, 0.5f));
        DrawTextEx(Rubik, "Pressione P para pausar", (Vector2){15, 15}, 16, spacingRubik, Fade(RAYWHITE, 0.8f));
    }

    if (gameOver) {
       DrawGameOverScreen(score, recorde, skinSelecionada, texturasSkins);
    }
} 

// funções ranking
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

// tutorial
void DrawTutorial() {
    DrawTutorialScreen(skinSelecionada, maiorSkinDesbloqueada, texturasSkins, nomesSkins, mar1Texture, mar2Texture, areiaTexture, coqueirosTexture);
}