#include "screens.h"
#include "raylib.h"
#include "game.h" 
#include <stdio.h>
#include <math.h>

int paginaAtual = 0;
bool naHistoria = true;
extern bool temSkinNova; 
extern bool naCapa;

Texture2D historia1Texture;
Texture2D historia2Texture;
Texture2D historia3Texture;

// Avisa o arquivo que as fontes já foram criadas no main.c
extern Font GasoekOne;
extern Font Rubik;

void InitScreens() {
    // Carrega as 3 cenas oficiais da história //
    historia1Texture = LoadTexture("assets/backgrounds/cena1.png");
    historia2Texture = LoadTexture("assets/backgrounds/cena2.png");
    historia3Texture = LoadTexture("assets/backgrounds/cena3.png");

    paginaAtual = 0;
    naHistoria = true;
}

void UnloadScreens() {
    UnloadTexture(historia1Texture);
    UnloadTexture(historia2Texture);
    UnloadTexture(historia3Texture);
}

void UpdateHistory() {
    if (!naHistoria) return;

    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_ENTER)) {
        paginaAtual++;
        if (paginaAtual > 2) {
            naHistoria = false;
            tutorial = true; 
        }
    }
    
    if (IsKeyPressed(KEY_LEFT) && paginaAtual > 0) {
        paginaAtual--;
    }
}

void DrawHistory() {
    if (!naHistoria) return;

    if (paginaAtual == 0) DrawTexture(historia1Texture, 0, 0, WHITE);
    else if (paginaAtual == 1) DrawTexture(historia2Texture, 0, 0, WHITE);
    else if (paginaAtual == 2) DrawTexture(historia3Texture, 0, 0, WHITE);

    float spacing = 1.0f;

    if (paginaAtual == 0) {
        DrawTextEx(GasoekOne, "O Império de", (Vector2){35, 60}, 36, spacing, RAYWHITE);
        DrawTextEx(GasoekOne, "Crabisson", (Vector2){35, 105}, 42, spacing, GOLD);

        DrawTextEx(Rubik, "Crabisson passou a", (Vector2){35, 190}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "noite inteira organizando", (Vector2){35, 225}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "o estoque do quiosque.", (Vector2){35, 260}, 24, spacing, RAYWHITE);

        DrawTextEx(Rubik, "Ele estava se sentindo", (Vector2){35, 320}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "o rei do coco verde,", (Vector2){35, 355}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "pronto para ficar", (Vector2){35, 390}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "milionário na praia!", (Vector2){35, 425}, 24, spacing, RAYWHITE);
    } 
    else if (paginaAtual == 1) {
        DrawTextEx(GasoekOne, "O Arrastão", (Vector2){35, 60}, 38, spacing, RAYWHITE);
        DrawTextEx(GasoekOne, "da Gangue", (Vector2){35, 105}, 38, spacing, RAYWHITE);

        DrawTextEx(Rubik, "Do nada, a Máfia das", (Vector2){35, 180}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "Gaivotas desceu em", (Vector2){35, 215}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "bando e fez um", (Vector2){35, 250}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "arrastão violento!", (Vector2){35, 285}, 24, spacing, RAYWHITE);

        DrawTextEx(Rubik, "Mas como gaivota é", (Vector2){35, 345}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "bicho burro, elas", (Vector2){35, 380}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "roubaram tudo: os", (Vector2){35, 415}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "cocos e os lixos", (Vector2){35, 450}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "jogados pela praia.", (Vector2){35, 485}, 24, spacing, RAYWHITE);
    } 
    else if (paginaAtual == 2) {
        DrawTextEx(GasoekOne, "Chuva de", (Vector2){35, 60}, 38, spacing, RAYWHITE);
        DrawTextEx(GasoekOne, "Cocos!", (Vector2){35, 105}, 38, spacing, RAYWHITE);

        DrawTextEx(Rubik, "Lá no alto, o bando", (Vector2){35, 180}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "percebeu que a carga", (Vector2){35, 215}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "estava pesada demais.", (Vector2){35, 250}, 24, spacing, RAYWHITE);

        DrawTextEx(Rubik, "Em pânico, começaram", (Vector2){35, 310}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "a jogar tudo de volta!", (Vector2){35, 345}, 24, spacing, RAYWHITE);

        DrawTextEx(Rubik, "Ajude Crabisson a", (Vector2){35, 405}, 24, spacing, YELLOW);
        DrawTextEx(Rubik, "recuperar os cocos,", (Vector2){35, 440}, 24, spacing, YELLOW);
        DrawTextEx(Rubik, "mas cuidado com", (Vector2){35, 475}, 24, spacing, RAYWHITE);
        DrawTextEx(Rubik, "o lixo caindo!", (Vector2){35, 510}, 24, spacing, RED);
    }

    DrawTextEx(Rubik, "Aperte a Seta Direita ->", (Vector2){45, 560}, 20, spacing, RAYWHITE);
}

void DrawTutorialScreen(int skinSelecionada, int maiorSkinDesbloqueada, Texture2D texturasSkins[6][5], const char* nomesSkins[6], Texture2D mar1Texture, Texture2D mar2Texture, Texture2D areiaTexture, Texture2D coqueirosTexture) {
    ClearBackground((Color){154, 244, 255, 255});

    float tempoAtual = GetTime();
    int waveOffset1 = sinf(tempoAtual * 1.5f) * 8;
    DrawTexture(mar1Texture, 0, waveOffset1, WHITE);
    int waveOffset2 = sinf((tempoAtual * 1.5f) + 1.0f) * 8;
    DrawTexture(mar2Texture, 0, waveOffset2, WHITE);
    DrawTexture(areiaTexture, 0, 0, WHITE);
    DrawTexture(coqueirosTexture, 0, 0, WHITE);

    DrawRectangle(100, 30, 800, 540, Fade(WHITE, 0.75f));

    // Título Principal
    const char *tituloTexto = "VAI CATAR COQUINHO";
    float tituloSize = 46.0f;
    Vector2 tituloMedidas = MeasureTextEx(GasoekOne, tituloTexto, tituloSize, 1.0f);
    DrawTextEx(GasoekOne, tituloTexto, (Vector2){(1000 - tituloMedidas.x)/2, 45}, tituloSize, 1.0f, DARKGREEN);

    // Menu Alinhado
    const char *comoJogar = "Como jogar:";
    Vector2 sizeComoJogar = MeasureTextEx(Rubik, comoJogar, 26, 1.0f);
    DrawTextEx(Rubik, comoJogar, (Vector2){500 - sizeComoJogar.x/2, 115}, 26, 1.0f, BLACK);

    const char *instrucaoMover = "Use as setas <- -> para mover o caranguejo";
    Vector2 sizeInstrucaoMover = MeasureTextEx(Rubik, instrucaoMover, 22, 1.0f);
    DrawTextEx(Rubik, instrucaoMover, (Vector2){500 - sizeInstrucaoMover.x/2, 150}, 22, 1.0f, BLACK);
    
    const char *tiposItens = "Tipos de itens:";
    Vector2 sizeTiposItens = MeasureTextEx(Rubik, tiposItens, 26, 1.0f);
    DrawTextEx(Rubik, tiposItens, (Vector2){500 - sizeTiposItens.x/2, 195}, 26, 1.0f, BLACK);

    const char *itemVerde = "Coco verde: +1 ponto";
    Vector2 sizeItemVerde = MeasureTextEx(Rubik, itemVerde, 22, 1.0f);
    DrawTextEx(Rubik, itemVerde, (Vector2){500 - sizeItemVerde.x/2, 230}, 22, 1.0f, DARKGREEN);

    const char *itemDourado = "Coco dourado: dobra os pontos por 20 segundos!";
    Vector2 sizeItemDourado = MeasureTextEx(Rubik, itemDourado, 22, 1.0f);
    DrawTextEx(Rubik, itemDourado, (Vector2){500 - sizeItemDourado.x/2, 255}, 22, 1.0f, GOLD);

    const char *itemAgua = "Agua de coco: +1 vida";
    Vector2 sizeItemAgua = MeasureTextEx(Rubik, itemAgua, 22, 1.0f);
    DrawTextEx(Rubik, itemAgua, (Vector2){500 - sizeItemAgua.x/2, 280}, 22, 1.0f, BLUE);

    const char *itemLixo = "Lixo: -1 vida";
    Vector2 sizeItemLixo = MeasureTextEx(Rubik, itemLixo, 22, 1.0f);
    DrawTextEx(Rubik, itemLixo, (Vector2){500 - sizeItemLixo.x/2, 305}, 22, 1.0f, RED);
    
    const char *instrucaoSkin = "Escolha seu Visual (Setas de CIMA / BAIXO):";
    Vector2 sizeInstrucaoSkin = MeasureTextEx(Rubik, instrucaoSkin, 24, 1.0f);
    DrawTextEx(Rubik, instrucaoSkin, (Vector2){500 - sizeInstrucaoSkin.x/2, 345}, 24, 1.0f, BLACK);
    
    // Configurações do desenho da Skin
    float skinMenuX = 435.0f; 
    float skinMenuY = 360.0f; 
    bool estaBloqueada = (skinSelecionada > maiorSkinDesbloqueada);

    // Desenha o sprite base da skin
    if (estaBloqueada) {
        DrawTexture(texturasSkins[skinSelecionada][0], (int)skinMenuX, (int)skinMenuY, GRAY);
        
        // 🛠️ NOVO: Retângulo semitransparente em cima do sprite trancado
        int rectW = 130;
        int rectH = 45;
        int rectX = 500 - (rectW / 2);
        int rectY = (int)skinMenuY + 50; // Centralizado na altura do sprite
        
        // Desenha o fundo branco com borda preta fina
        DrawRectangle(rectX, rectY, rectW, rectH, Fade(WHITE, 0.90f));
        DrawRectangleLines(rectX, rectY, rectW, rectH, BLACK);

        // Define o texto baseado nos recordes do seu game.c
        const char* textoRequisito = "0 Cocos";
        if (skinSelecionada == 1) textoRequisito = "20 Cocos";
        if (skinSelecionada == 2) textoRequisito = "50 Cocos";
        if (skinSelecionada == 3) textoRequisito = "80 Cocos";
        if (skinSelecionada == 4) textoRequisito = "120 Cocos";
        if (skinSelecionada == 5) textoRequisito = "200 Cocos";

        // Desenha o texto de pontos dentro do retângulo branco
        Vector2 sizeReq = MeasureTextEx(Rubik, textoRequisito, 18, 1.0f);
        DrawTextEx(Rubik, textoRequisito, (Vector2){500 - sizeReq.x/2, rectY + 13}, 18, 1.0f, MAROON);
        
    } else {
        DrawTexture(texturasSkins[skinSelecionada][0], (int)skinMenuX, (int)skinMenuY, WHITE);
    }

    char textoSkin[60];
    if (estaBloqueada) {
        sprintf(textoSkin, "< %s (Bloqueado) >", nomesSkins[skinSelecionada]);
        Vector2 sizeTextoSkin = MeasureTextEx(Rubik, textoSkin, 24, 1.0f);
        DrawTextEx(Rubik, textoSkin, (Vector2){500 - sizeTextoSkin.x/2, 490}, 24, 1.0f, RED);
    } else {
        sprintf(textoSkin, "< Skin: %s >", nomesSkins[skinSelecionada]);
        Vector2 sizeTextoSkin = MeasureTextEx(Rubik, textoSkin, 24, 1.0f);
        DrawTextEx(Rubik, textoSkin, (Vector2){500 - sizeTextoSkin.x/2, 490}, 24, 1.0f, ORANGE);

        if (temSkinNova && skinSelecionada == maiorSkinDesbloqueada) {
            float pisca = sinf(GetTime() * 8.0f); 
            if (pisca > 0.0f) {
                const char* textoNovo = "[ NOVO DESBLOQUEIO! ]";
                Vector2 sizeNovo = MeasureTextEx(Rubik, textoNovo, 20, 1.0f);
                DrawTextEx(Rubik, textoNovo, (Vector2){500 - sizeNovo.x/2, 462}, 20, 1.0f, LIME);
            }
        }
    }

    const char *botaoTexto = "Pressione ENTER para comecar!";
    float botaoSize = 24.0f;
    Vector2 botaoMedidas = MeasureTextEx(GasoekOne, botaoTexto, botaoSize, 1.0f);
    DrawTextEx(GasoekOne, botaoTexto, (Vector2){(1000 - botaoMedidas.x)/2, 530}, botaoSize, 1.0f, DARKBLUE);
}

void DrawGameOverScreen(int score, int recorde, int skinSelecionada, Texture2D texturasSkins[6][5]) {
    // Fundo semitransparente
    DrawRectangle(0, 0, 1000, 600, Fade(WHITE, 0.7f));

    float gameOverFontSize = 120.0f;           
    float scoreFontSize = 84.0f;               
    float recordeFontSize = 60.0f;             
    float instructionFontSize = 35.0f;         

    float gameOverSpacing = 1.2f;
    float scoreSpacing = 1.0f;
    float recordeSpacing = 0.9f;
    float instructionSpacing = 0.75f;

    const char *gameOverText = "GAME OVER";
    char scoreText[50];
    sprintf(scoreText, "Score final: %d", score);
    char recordeText[50];
    sprintf(recordeText, "Maior Recorde: %d", recorde);
    const char *instructionText = "Pressione ENTER para reiniciar!";

    Vector2 gameOverSize = MeasureTextEx(GasoekOne, gameOverText, gameOverFontSize, gameOverSpacing);
    Vector2 scoreSize = MeasureTextEx(GasoekOne, scoreText, scoreFontSize, scoreSpacing);
    Vector2 recordeSize = MeasureTextEx(GasoekOne, recordeText, recordeFontSize, recordeSpacing);
    Vector2 instructionSize = MeasureTextEx(GasoekOne, instructionText, instructionFontSize, instructionSpacing);

    Vector2 gameOverPos = { (1000 - gameOverSize.x) / 2.0f, 40.0f };
    Vector2 scorePos    = { (1000 - scoreSize.x) / 2.0f, 170.0f };
    Vector2 recordePos  = { (1000 - recordeSize.x) / 2.0f, 270.0f };
    Vector2 instructionPos = { (1000 - instructionSize.x) / 2.0f, 430.0f };

    // --- DESENHO DOS TEXTOS ---
    
    // GAME OVER
    for (int dx = -2; dx <= 2; dx++) {
        for (int dy = -2; dy <= 2; dy++) {
            if (dx != 0 || dy != 0) DrawTextEx(GasoekOne, gameOverText, (Vector2){gameOverPos.x + dx, gameOverPos.y + dy}, gameOverFontSize, gameOverSpacing, BLACK);
        }
    }
    DrawTextEx(GasoekOne, gameOverText, gameOverPos, gameOverFontSize, gameOverSpacing, GOLD);

    // SCORE FINAL
    for (int dx = -2; dx <= 2; dx++) {
        for (int dy = -2; dy <= 2; dy++) {
            if (dx != 0 || dy != 0) DrawTextEx(GasoekOne, scoreText, (Vector2){scorePos.x + dx, scorePos.y + dy}, scoreFontSize, scoreSpacing, BLACK);
        }
    }
    DrawTextEx(GasoekOne, scoreText, scorePos, scoreFontSize, scoreSpacing, WHITE);

    // MAIOR RECORDE
    for (int dx = -2; dx <= 2; dx++) {
        for (int dy = -2; dy <= 2; dy++) {
            if (dx != 0 || dy != 0) DrawTextEx(GasoekOne, recordeText, (Vector2){recordePos.x + dx, recordePos.y + dy}, recordeFontSize, recordeSpacing, BLACK);
        }
    }
    DrawTextEx(GasoekOne, recordeText, recordePos, recordeFontSize, recordeSpacing, RED);

    // --- LÓGICA DE NOVO PERSONAGEM DESBLOQUEADO ---
    // Verifica se o score atual bateu um dos marcos de desbloqueio
    bool novoDesbloqueio = false;
    if (score >= recorde) { // Só avisa se for recorde novo ou igualado
        if ((score >= 20 && score < 50) || (score >= 50 && score < 80) || 
            (score >= 80 && score < 120) || (score >= 120 && score < 200) || (score >= 200)) {
            novoDesbloqueio = true;
        }
    }

    if (novoDesbloqueio) {
        const char* unlockText = "NOVO PERSONAGEM DESBLOQUEADO!";
        // Faz o texto pulsar entre Amarelo e Verde Lima
        float pulse = (sinf(GetTime() * 10.0f) + 1.0f) / 2.0f; 
        Color corPulso = (pulse > 0.5f) ? YELLOW : LIME;

        Vector2 unlockSize = MeasureTextEx(Rubik, unlockText, 30, 1.0f);
        Vector2 unlockPos = { (1000 - unlockSize.x) / 2.0f, 340.0f }; // Posição entre o Recorde e a Instrução

        // Borda do texto de desbloqueio
        DrawTextEx(Rubik, unlockText, (Vector2){unlockPos.x + 2, unlockPos.y + 2}, 30, 1.0f, BLACK);
        DrawTextEx(Rubik, unlockText, unlockPos, 30, 1.0f, corPulso);
    }

    // INSTRUÇÃO REINICIAR
    for (int dx = -2; dx <= 2; dx++) {
        for (int dy = -2; dy <= 2; dy++) {
            if (dx != 0 || dy != 0) DrawTextEx(GasoekOne, instructionText, (Vector2){instructionPos.x + dx, instructionPos.y + dy}, instructionFontSize, instructionSpacing, BLACK);
        }
    }
    DrawTextEx(GasoekOne, instructionText, instructionPos, instructionFontSize, instructionSpacing, WHITE);

    // CARANGUEJO CHORANDO
    float caranguejoGameOverX = 445.0f; 
    float caranguejoGameOverY = 460.0f; // Ajustei um pouco para baixo para não bater no texto

    Vector2 posChorando = { caranguejoGameOverX, caranguejoGameOverY }; 
    DrawTextureEx(texturasSkins[skinSelecionada][4], posChorando, 0.0f, 1.0f, WHITE);
}

// --- TELA DE CAPA / MENU INICIAL --- //
void DrawCapaScreen() {
    // Fundo azul praia estático para a capa
    ClearBackground((Color){154, 244, 255, 255});
    
    float spacingGasoek = 2.0f;
    float spacingRubik = 1.0f;

    // 1. TÍTULO DO JOGO (Grande e imponente)
    const char* tituloText = "VAI CATAR COQUINHO!";
    Vector2 sizeTitulo = MeasureTextEx(GasoekOne, tituloText, 64, spacingGasoek);
    Vector2 posTitulo = { 500.0f - (sizeTitulo.x / 2.0f), 180.0f };
    
    // Borda preta no título
    DrawTextEx(GasoekOne, tituloText, (Vector2){posTitulo.x - 3, posTitulo.y - 3}, 64, spacingGasoek, BLACK);
    DrawTextEx(GasoekOne, tituloText, (Vector2){posTitulo.x + 3, posTitulo.y - 3}, 64, spacingGasoek, BLACK);
    DrawTextEx(GasoekOne, tituloText, (Vector2){posTitulo.x - 3, posTitulo.y + 3}, 64, spacingGasoek, BLACK);
    DrawTextEx(GasoekOne, tituloText, (Vector2){posTitulo.x + 3, posTitulo.y + 3}, 64, spacingGasoek, BLACK);
    DrawTextEx(GasoekOne, tituloText, posTitulo, 64, spacingGasoek, ORANGE);

    // 2. SUBTÍTULO OU FRASE DE EFEITO
    const char* subText = "As aventuras de crabisson o caranguejo!";
    Vector2 sizeSub = MeasureTextEx(Rubik, subText, 24, spacingRubik);
    DrawTextEx(Rubik, subText, (Vector2){500.0f - (sizeSub.x / 2.0f), 260.0f}, 24, spacingRubik, DARKGRAY);

    // 3. TEXTO PISCANTE PARA COMEÇAR
    const char* startText = "Pressione ENTER para iniciar a jornada";
    float pisca = sinf(GetTime() * 5.0f); // Faz o texto piscar suavemente
    Color corTexto = (pisca > 0.0f) ? DARKBLUE : Fade(DARKBLUE, 0.3f);

    Vector2 sizeStart = MeasureTextEx(GasoekOne, startText, 26, spacingGasoek);
    DrawTextEx(GasoekOne, startText, (Vector2){500.0f - (sizeStart.x / 2.0f), 420.0f}, 26, spacingGasoek, corTexto);

    // Créditos simples no rodapé
    DrawTextEx(Rubik, "Desenvolvido em Raylib", (Vector2){30, 560}, 16, spacingRubik, GRAY);
}