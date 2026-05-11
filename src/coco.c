#include "raylib.h"
#include "coco.h"

#include <stdlib.h>

// REQUISITO: Ponteiro
// Ponteiro para o primeiro elemento da lista encadeada
Node *listaCocos = NULL;

// REQUISITO: Matriz
// Matriz que controla áreas ocupadas pelos cocos
int spawnAreas[2][5] = {0};



// cria um novo coco //
void AddCoco() {

    // REQUISITO: Alocação dinâmica
    // Cria um novo nó dinamicamente na memória
    Node *novo = malloc(sizeof(Node));

    int linha;
    int coluna;

    // procura uma posição livre na matriz //
    do {

        linha = GetRandomValue(0, 1);
        coluna = GetRandomValue(0, 4);

    } while (spawnAreas[linha][coluna] == 1);

    // marca essa posição como ocupada //
    spawnAreas[linha][coluna] = 1;

    // tamanho das áreas da tela //
    int larguraArea = 200;
    int alturaArea = 300;

    // posiciona o coco horizontalmente //
    novo->coco.x = coluna * larguraArea + 100;

    // posiciona o coco verticalmente acima da tela //
    novo->coco.y = -(linha * alturaArea)
                    - GetRandomValue(50, 300);

    // velocidade inicial do coco //
    novo->coco.speed = GetRandomValue(2, 3);

    // tamanho do coco //
    novo->coco.radius = 20;

    // REQUISITO: Lista encadeada + ponteiros
    // o novo coco aponta para o antigo primeiro coco da lista
    novo->next = listaCocos;

    // agora o novo coco vira o primeiro da lista
    listaCocos = novo;
}



// inicia os cocos //
void InitCocos() {

    // cria 3 cocos no início do jogo
    for (int i = 0; i < 3; i++) {

        AddCoco();
    }
}



// atualiza //
void UpdateCocos() {

    // começa pelo primeiro coco da lista
    Node *atual = listaCocos;

    // percorre toda a lista encadeada
    while (atual != NULL) {

        // move o coco para baixo
        atual->coco.y += atual->coco.speed;

        // verifica se saiu da tela
        if (atual->coco.y > 600) {

            // reposiciona acima da tela
            atual->coco.y = GetRandomValue(-400, -30);

            // nova posição horizontal
            atual->coco.x = GetRandomValue(100, 900);
        }

        // passa para o próximo coco da lista
        atual = atual->next;
    }
}



// desenha //
void DrawCocos() {

    // começa pelo primeiro coco
    Node *atual = listaCocos;

    // percorre todos os cocos
    while (atual != NULL) {

        // desenha o coco na tela
        DrawCircle(
            atual->coco.x,
            atual->coco.y,
            atual->coco.radius,
            DARKBROWN
        );

        // vai para o próximo coco
        atual = atual->next;
    }
}