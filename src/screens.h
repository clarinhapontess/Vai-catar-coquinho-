#ifndef SCREENS_H
#define SCREENS_H

#include "raylib.h"

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

#endif