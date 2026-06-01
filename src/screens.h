#ifndef SCREENS_H
#define SCREENS_H

#include "raylib.h"
// variáveis de controle de telas
extern Font GasoekOne;
extern Font Rubik;
// história
extern int paginaAtual;
extern bool naHistoria;
// texturas da história
extern Texture2D historia1Texture;
extern Texture2D historia2Texture;
extern Texture2D historia3Texture;

void InitScreens();
void UnloadScreens();
void UpdateHistory();
void DrawHistory();
void DrawCapaScreen();

void DrawTutorialScreen(int skinSelecionada, int maiorSkinDesbloqueada, Texture2D texturasSkins[6][5], const char* nomesSkins[6], Texture2D mar1Texture, Texture2D mar2Texture, Texture2D areiaTexture, Texture2D coqueirosTexture);
void DrawGameOverScreen(int score, int recorde, int skinSelecionada, Texture2D texturasSkins[6][5]);

#endif