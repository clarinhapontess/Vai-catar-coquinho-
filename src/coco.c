#include "raylib.h"
#include "coco.h"
#include <stdlib.h>
#include "game.h"  

Node *listaCocos = NULL;

// texturas
Texture2D cocoTexture;
Texture2D lataTexture;
Texture2D garrafaTexture;
Texture2D douradoTexture;
Texture2D aguaTexture;

// matriz de controle de spawn para evitar que dois cocos apareçam no mesmo lugar
int spawnAreas[2][5] = {0};

// define tipo de coco e velocidade
void SetCocoType(Coco *coco) {
    extern int score; 

    int chance = GetRandomValue(1, 100);
    
    float minSpeed = 2;
    float maxSpeed = 2.5f;

    if (score >= 10) {
        minSpeed = 2;
        maxSpeed = 3.5f;
    } else if (score >= 20) {
        minSpeed = 3;
        maxSpeed = 4;
    } else if (score >= 40) {
        minSpeed = 3;
        maxSpeed = 4.5f;
    } else if (score >= 60) {
        minSpeed = 3.5f;
        maxSpeed = 5;
    }else if (score >= 100) {
        minSpeed = 4;
        maxSpeed = 6;
    } else {
        minSpeed = 2; 
        maxSpeed = 2.5f;
    }

    // coco normal
    if (chance <= 58) {
        coco->type = 0;
        coco->speed = GetRandomValue(minSpeed, maxSpeed);
    }
    // lixo
    else if (chance <= 95) {
        coco->type = 1;
        coco->speed = GetRandomValue(minSpeed, maxSpeed);
        coco->sprite = GetRandomValue(0, 1);
    }
    // dourado
    else if (chance <= 98) {
        coco->type = 2;
        coco->speed = minSpeed; 
    }
    // agua de coco
    else {
        coco->type = 3;
        coco->speed = maxSpeed; 
    }
}
// adiciona cocos de acordo com a pontuação
void AddCoco() {
    Node *novo = (Node*)malloc(sizeof(Node));
    if (novo == NULL) return;

    int linha, coluna;

    do {
        linha = GetRandomValue(0, 1);
        coluna = GetRandomValue(0, 4);
    } while (spawnAreas[linha][coluna] == 1);

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
// incializa as skins dos cocos
void InitCocos() {
    cocoTexture = LoadTexture("assets/cocos/coco.png");
    lataTexture = LoadTexture("assets/cocos/lata.png");
    garrafaTexture = LoadTexture("assets/cocos/garrafa.png");
    douradoTexture = LoadTexture("assets/cocos/dourado.png");
    aguaTexture = LoadTexture("assets/cocos/agua.png");
    
    for (int i = 0; i < 3; i++) {
        AddCoco();
    }
}

// atualiza posição de quedas
void Updatecoco(float deltaTime) {
    extern float cocoSpeedMultiplier; 
    Node *atual = listaCocos;

    while (atual != NULL) {
        // move o coco p baixo
        atual->coco.y += atual->coco.speed * cocoSpeedMultiplier;
        // respawn do coco
        if (atual->coco.y > (GetScreenHeight()+50)) {
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

// limpa memoria
void ClearCocos() {
    Node *atual = listaCocos;
    while (atual != NULL) {
        Node *temp = atual;
        atual = atual->next;
        free(temp);
    }
    listaCocos = NULL;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            spawnAreas[i][j] = 0;
        }
    }
}

// desenha cocos
void DrawCocos() {
    Node *atual = listaCocos;
    while (atual != NULL) {
        Texture2D texturaAtual;

        if (atual->coco.type == 0) {
            texturaAtual = cocoTexture;
        }
        else if (atual->coco.type == 1) {
            if (atual->coco.sprite == 0){
                texturaAtual = lataTexture;
            } else {
                texturaAtual = garrafaTexture;
            }
        }
        else if (atual->coco.type == 2) {
            texturaAtual = douradoTexture;
        }
        else {
            texturaAtual = aguaTexture;
        }

        DrawTextureEx(
            texturaAtual,
            (Vector2){atual->coco.x - 32, atual->coco.y - 32},
            0.0f,
            1.0f,
            WHITE
        );

        atual = atual->next;
    }
}