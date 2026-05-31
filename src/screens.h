#ifndef SCREENS_H
#define SCREENS_H

#include "raylib.h"

// Fontes Globais //
extern Font GasoekOne;
extern Font Rubik;

// Variáveis de controle da história //
extern int paginaAtual;
extern bool naHistoria;

// Texturas das páginas //
extern Texture2D historia1Texture;
extern Texture2D historia2Texture;
extern Texture2D historia3Texture;

// Funções //
void InitScreens();
void UnloadScreens();
void UpdateHistory();
void DrawHistory();

// Funções de interface movidas para organização //
void DrawTutorialScreen(int skinSelecionada, int maiorSkinDesbloqueada, Texture2D texturasSkins[6][5], const char* nomesSkins[6], Texture2D mar1Texture, Texture2D mar2Texture, Texture2D areiaTexture, Texture2D coqueirosTexture);
void DrawGameOverScreen(int score, int recorde, int skinSelecionada, Texture2D texturasSkins[6][5]);

#endif