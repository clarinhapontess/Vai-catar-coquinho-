#include "screens.h"
#include "game.h" 
#include <stdio.h>

int paginaAtual = 0;
bool naHistoria = true;

Texture2D historia1Texture;
Texture2D historia2Texture;
Texture2D historia3Texture;

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

    // Se apertar a seta para a direita ou ENTER, avança a página //
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_ENTER)) {
        paginaAtual++;
        
        // Se passou da página 3 (índice 2), fecha a história e liga o tutorial //
        if (paginaAtual > 2) {
            naHistoria = false;
            tutorial = true; 
        }
    }
    
    // Opcional: Permite voltar a página com a seta para a esquerda //
    if (IsKeyPressed(KEY_LEFT) && paginaAtual > 0) {
        paginaAtual--;
    }
}

void DrawHistory() {
    if (!naHistoria) return;

    // 1. Desenha a imagem de fundo da cena atual (que já tem o retângulo colorido integrado) //
    if (paginaAtual == 0) DrawTexture(historia1Texture, 0, 0, WHITE);
    else if (paginaAtual == 1) DrawTexture(historia2Texture, 0, 0, WHITE);
    else if (paginaAtual == 2) DrawTexture(historia3Texture, 0, 0, WHITE);

    // --- TEXTOS DA HISTORINHA DENTRO DO RETÂNGULO DA ESQUERDA (X: 30 a 320) --- //
    // Como o texto é longo, dividimos em parágrafos manuais para caber perfeitamente no bloco colorido.

    if (paginaAtual == 0) {
        DrawText("O Império de", 35, 60, 36, RAYWHITE);
        DrawText("Crabisson", 35, 105, 42, GOLD);

        DrawText("Crabisson passou a", 35, 190, 24, RAYWHITE);
        DrawText("noite inteira organizando", 35, 225, 24, RAYWHITE);
        DrawText("o estoque do quiosque.", 35, 260, 24, RAYWHITE);

        DrawText("Ele estava se sentindo", 35, 320, 24, RAYWHITE);
        DrawText("o rei do coco verde,", 35, 355, 24, RAYWHITE);
        DrawText("pronto para ficar", 35, 390, 24, RAYWHITE);
        DrawText("milionário na praia!", 35, 425, 24, RAYWHITE);
    } 
    else if (paginaAtual == 1) {
        DrawText("O Arrastão", 35, 60, 38, RAYWHITE);
        DrawText("da Gangue", 35, 105, 38, RAYWHITE);

        DrawText("Do nada, a Máfia das", 35, 180, 24, RAYWHITE);
        DrawText("Gaivotas desceu em", 35, 215, 24, RAYWHITE);
        DrawText("bando e fez um", 35, 250, 24, RAYWHITE);
        DrawText("arrastão violento!", 35, 285, 24, RAYWHITE);

        DrawText("Mas como gaivota é", 35, 345, 24, RAYWHITE);
        DrawText("bicho burro, elas", 35, 380, 24, RAYWHITE);
        DrawText("roubaram tudo: os", 35, 415, 24, RAYWHITE);
        DrawText("cocos e os lixos", 35, 450, 24, RAYWHITE);
        DrawText("jogados pela praia.", 35, 485, 24, RAYWHITE);
    } 
    else if (paginaAtual == 2) {
        DrawText("Chuva de", 35, 60, 38, RAYWHITE);
        DrawText("Cocos!", 35, 105, 38, RAYWHITE);

        DrawText("Lá no alto, o bando", 35, 180, 24, RAYWHITE);
        DrawText("percebeu que a carga", 35, 215, 24, RAYWHITE);
        DrawText("estava pesada demais.", 35, 250, 24, RAYWHITE);

        DrawText("Em pânico, começaram", 35, 310, 24, RAYWHITE);
        DrawText("a jogar tudo de volta!", 35, 345, 24, RAYWHITE);

        DrawText("Ajude Crabisson a", 35, 405, 24, YELLOW);
        DrawText("recuperar os cocos,", 35, 440, 24, YELLOW);
        DrawText("mas cuidado com", 35, 475, 24, RAYWHITE);
        DrawText("o lixo caindo!", 35, 510, 24, RED);
    }

    // Indicador sutil de navegação na parte inferior do bloco de texto //
    DrawText("Aperte a Seta Direita ->", 45, 560, 20, RAYWHITE);
}