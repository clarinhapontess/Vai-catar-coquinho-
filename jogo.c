#include "raylib.h"  // biblioteca para os graficos
#include <stdio.h>
#include <stdlib.h>

void SetupGame(){ // função para as definições do raylib
    const int screenWidth = 800; 
    const int screenHeight = 450;

}

typedef struct{
    float x, y;
    int largura, altura;
    int vidas;

} Jogador; 
Jogador jogador; 

// funções base do jogo
void inicializarJogo(); // incializa todas as variaveis
void atualizarJogo(); // atualiza estado do jogo
void desenharJogo(); // PRECISA DO RAYLIB

int verificarGameOver(Jogador j){
     if (j.vidas <= 0) // condição de derrota = vidas acabaram
        return 1; // retorna verdadeiro
     return 0; // continua o jogo
}

// funções do jogador
void inicializarJogador(Jogador *j){ 
     j->x = 400; // inicializa o jogador no meio da tela
     j->y = 400;  // posição vertical do jogador
     j->largura = 50; // largura do jogador
     j->altura = 20; // altura do jogador
     j->vidas = 3; // jogador começa com 3 vidas
}
// definie valores iniciais (posição, vidas)
void moverJogador(Jogador *j); // PRECISA DO RAYLIB
void desenharJogador(Jogador *j); // PRECISA DO RAYLIB

// funções dos cocos
coco* criarCoco(int tipo); // cria 1 coco na memoria, define tipo, posição incial
void adicionarCoco(Coco **lista); // coloca um coco novo na lista
void atualizarCocos(Coco *lista); // move todos os cocos
void desenharCocos(Coco *lista); // desenha todos os cocos
void removerCocos(Coco **lista); // remove cocos que sairam da tela ou foram coletados
void spawnCocos(Coco **lista); // quando cria cocoss

//funções colisão
int verificarColisao(Jogador j, Coco c); // checa se o jogador encostou no coco
void tratarColisoes(Jogador *j, Coco **lista); // aplica o efeito do coco quando ha colisão
void aplicarEfeitoCoco(Jogador *j, int tipo); // aplica efeito dependendo do tipo 
void atualizarEfeitos(Jogador *j); // duração dos efeitos 

// funções RANKING 
void salvarScore(int score); // salva no arquivo txt
void lerScores(); // le e mostra ranking

// funções game over
void desenharHUD(jogador j, int score); // molstra info na tela (vidas, pontuação, efeitos ativos)
void telaGameOver(int score); // mostra tela final

int main(void) { // função main
    
    
}