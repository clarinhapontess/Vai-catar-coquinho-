#include "raylib.h"
#include "coco.h"
#include <stdlib.h>
#include "game.h"  // Isso dá acesso ao extern gameTimer
// lista encadeada
Node *listaCocos = NULL;
//criando o head da lista encadeada e atribuindo à Null
// texturas
Texture2D cocoTexture;
Texture2D lataTexture;
Texture2D garrafaTexture;
Texture2D douradoTexture;
Texture2D aguaTexture;

// matriz de spawn
int spawnAreas[2][5] = {0};

// escolhe tipo do coco
static void SetCocoType(Coco *coco) {

    int chance = GetRandomValue(1, 100);

    // coco normal
    if (chance <= 60) {

        coco->type = 0;
        coco->speed = 2;
    }

    // lixo
    else if (chance <= 95) {

        coco->type = 1;
        coco->speed = 2;
        coco->sprite = GetRandomValue(0, 1);
    }

    // coco dourado
    else if (chance <= 98) {

        coco->type = 2;
        coco->speed = 1;
    }

    // água de coco
    else {

        coco->type = 3;
        coco->speed = 1;
    }
}

// adiciona coco
void AddCoco() {

    Node *novo = (Node*)malloc(sizeof(Node));

    if (novo == NULL) return;

    int linha, coluna;

    do {

        linha = GetRandomValue(0, 1);
        coluna = GetRandomValue(0, 4);

    } while (spawnAreas[linha][coluna] == 1);
    //vai ficar sorteando valores para linha e coluna ocupados pelo coco até dar um espaço não ocupado 
    //while (spawnAreas[linha][coluna] == 0);

    spawnAreas[linha][coluna] = 1;

    novo->coco.row = linha;
    novo->coco.col = coluna;

    int larguraArea = 200;
    int alturaArea = 300;

    novo->coco.x = coluna * larguraArea + 100;
    novo->coco.y = -(linha * alturaArea) - GetRandomValue(50, 300);

    novo->coco.radius = 20;

    SetCocoType(&novo->coco);

    novo->next = listaCocos;
    listaCocos = novo;
}

// inicializa
void InitCocos() {

    // carrega texturas
    cocoTexture = LoadTexture("assets/cocos/coco.png");

    lataTexture = LoadTexture("assets/cocos/lata.png");

    garrafaTexture = LoadTexture("assets/cocos/garrafa.png");

    douradoTexture = LoadTexture("assets/cocos/dourado.png");

    aguaTexture = LoadTexture("assets/cocos/agua.png");
    

    // cria cocos
    for (int i = 0; i < 2; i++) {

        AddCoco();
    }
}

// update
void Updatecoco(float deltaTime) {

  

    Node *atual = listaCocos;

    while (atual != NULL) {

        atual->coco.y += atual->coco.speed;

        // respawn quando sai da tela
        if (atual->coco.y > 650) {

            spawnAreas[atual->coco.row][atual->coco.col] = 0;

            int linha, coluna;

            do {

                linha = GetRandomValue(0, 1);
                coluna = GetRandomValue(0, 4);

            } while (spawnAreas[linha][coluna] == 1);

            spawnAreas[linha][coluna] = 1;

            atual->coco.row = linha;
            atual->coco.col = coluna;

            atual->coco.x = coluna * 200 + 100;
            atual->coco.y = -(linha * 300) - GetRandomValue(50, 300);

            SetCocoType(&atual->coco);
        }

        atual = atual->next;
    }
}

// excluir cocos
void ClearCocos() {

    Node *atual = listaCocos;

    while (atual != NULL) {

        Node *temp = atual;

        atual = atual->next;

        free(temp);
    }

    listaCocos = NULL;

    // limpa matriz
    for (int i = 0; i < 2; i++) {

        for (int j = 0; j < 5; j++) {

            spawnAreas[i][j] = 0;
        }
    }
}

// draw
void DrawCocos() {

    Node *atual = listaCocos;

    while (atual != NULL) {

        Texture2D texturaAtual;

        // coco normal
        if (atual->coco.type == 0) {

            texturaAtual = cocoTexture;
        }

        // lixo
        else if (atual->coco.type == 1) {

            // escolhe entre lata e garrafa
            if (atual->coco.sprite == 0){

                texturaAtual = lataTexture;

            } else {

                texturaAtual = garrafaTexture;
            }
        }

        // coco dourado
        else if (atual->coco.type == 2) {

            texturaAtual = douradoTexture;
        }

        // água de coco
        else {

            texturaAtual = aguaTexture;
        }

        DrawTextureEx(

            texturaAtual,

            (Vector2){
                atual->coco.x - 32,
                atual->coco.y - 32
            },

            0.0f,

            1.0f,

            WHITE
        );

        atual = atual->next;
    }
}