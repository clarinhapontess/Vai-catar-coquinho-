#include "raylib.h"  // biblioteca para os graficos
#include <stdio.h>
#include <stdlib.h>

void SetupGame(){ // função para as definições do raylib
    const int screenWidth = 800; 
    const int screenHeight = 450;

}


// funções base do jogo
void inicializarJogo(); // incializa todas as variaveis
void atualizarJogo(); // atualiza estado do jogo
void desenharJogo(); // desenha tudo na tela
int verificarGameOver(jogador j); // decide se o jogo acabou 

// funções do jogador
void inicializarJogador(jogador *j); // definie valores iniciais (posição, vidas)
void moverJogador(jogador *j); // le teclado e move
void desenharJogador(jogador *j); // desenha o jogador na tela

// funções dos cocos
coco* criarCoco(int tipo); // cria 1 coco na memoria, define tipo, posição incial
void adicionarCoco(coco **lista); // coloca um coco novo na lista
void atualizarCocos(coco *lista); // move todos os cocos
void desenharCocos(coco *lista); // desenha todos os cocos
void removerCocos(coco **lista); // remove cocos que sairam da tela ou foram coletados
void spawnCocos(coco **lista); // quando cria cocoss

//funções colisão
int verificarColisao(jogador j, coco c); // checa se o jogador encostou no coco
void tratarColisoes(jogador *j, coco **lista); // aplica o efeito do coco quando ha colisão
void aplicarEfeitoCoco(jogador *j, int tipo); // aplica efeito dependendo do tipo 
void atualizarEfeitos(jogador *j); // duração dos efeitos 

// funções RANKING 
void salvarScore(int score); // salva no arquivo txt
void lerScores(); // le e mostra ranking

// funções game over
void desenharHUD(jogador j, int score); // molstra info na tela (vidas, pontuação, efeitos ativos)
void telaGameOver(int score); // mostra tela final

int main(void) { // função main
    
    
}