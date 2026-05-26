#include "raylib.h"
#include "coco.h"
#include <stdlib.h>
#include "game.h" 

// Lista encadeada, criando o head//
Node *listaCocos = NULL;

// Texturas dos cocos //
Texture2D cocoTexture;
Texture2D lataTexture;
Texture2D garraTexture;
Texture2D douradoTexture;
Texture2D aguaTexture;

// Matriz de spawn //
int spawnAreas[2][5] = {0};

// função que escolhe tipo do coco, e define sua velocidade de acordo com o nivel //
void SetCocoType(Coco *coco){
    extern int score; // recebe o score atual //

    int chance = GetRandomValue(0, 100); // Define as probabilidades para cada tipo de coco //

    // Velocidades baseadas no score, para aumentar a dificuldade conforme o jogador avança //
    int minSpeed = 120;
    int maxSpeed = 180;

    // Lógica corrigida: testamos do MAIOR score para o MENOR
    if (score > 100) {
        minSpeed = 260;
        maxSpeed = 320;
    } else if (score > 50) {
        minSpeed = 220;
        maxSpeed = 280;
    } else if (score > 30) {
        minSpeed = 180;
        maxSpeed = 240;
    } else if (score > 15) {
        minSpeed = 150;
        maxSpeed = 200;
    } else {
        minSpeed = 120;
        maxSpeed = 180;
    }

    // Tipos de coco e suas probabilidades //

    // COCO NORMAL //
    if (chance <= 58) {
        coco->type = 0;
        coco->speed = (float)GetRandomValue(minSpeed, maxSpeed);
    }
    // LIXO //
    else if (chance <= 95) {
        coco->type = 1;
        coco->speed = (float)GetRandomValue(minSpeed, maxSpeed);
        coco->sprite = GetRandomValue(0, 1); // (lata ou garrafa) //
    }
    // COCO DOURADO //
    else if (chance <= 98) {
        coco->type = 2;
        coco->speed = (float)minSpeed; // Sorteia a velocidade mínima do tier atual
    }
    // ÁGUA DE COCO //
    else {
        coco->type = 3;
        coco->speed = (float)maxSpeed; // Sorteia a velocidade máxima do tier atual
    }
}

// Adiciona um coco à lista encadeada //
void AddCoco() {
    Node *novo = (Node *)malloc(sizeof(Node));
    if (novo == NULL) return;

    int linha, coluna;
    int tentativas = 0;

    // Trava de segurança com limite de 20 tentativas para evitar loops infinitos se a tela lotar
    do {
        linha = GetRandomValue(0, 1);
        coluna = GetRandomValue(0, 4);
        tentativas++;
    } while (spawnAreas[linha][coluna] == 1 && tentativas < 20);

    spawnAreas[linha][coluna] = 1; // Marca a área de spawn como ocupada //

    novo->coco.row = linha;
    novo->coco.col = coluna;

    int larguraArea = 200; // Largura de cada área de spawn //
    int alturaArea = 300;  // Altura de cada área de spawn //

    novo->coco.x = coluna * larguraArea + 100;
    novo->coco.y = -(linha * alturaArea) - GetRandomValue(50, 300);
    novo->coco.radius = 20;
    
    SetCocoType(&novo->coco);
    
    novo->next = listaCocos;
    listaCocos = novo;
}

// Inicializa as texturas dos cocos //
void InitCocos() {
    cocoTexture = LoadTexture("assets/cocos/coco.png");
    lataTexture = LoadTexture("assets/cocos/lata.png");
    garraTexture = LoadTexture("assets/cocos/garrafa.png");
    douradoTexture = LoadTexture("assets/cocos/dourado.png");
    aguaTexture = LoadTexture("assets/cocos/agua.png");

    // comeca com 2 cocos na lista //
    for (int i = 0; i < 2; i++) {
       AddCoco();
    }
}

// atualiza a posição dos cocos e remove os que saem da tela //
void Updatecoco(float deltaTime) {
    Node *atual = listaCocos;
    while (atual != NULL) {
        atual->coco.y += atual->coco.speed * deltaTime;
        
        if (atual->coco.y > 650) {
            spawnAreas[atual->coco.row][atual->coco.col] = 0; // Libera a área de spawn //

            int linha, coluna;
            int tentativas = 0;
            
            do {
                linha = GetRandomValue(0, 1);
                coluna = GetRandomValue(0, 4);
                tentativas++;
            } while (spawnAreas[linha][coluna] == 1 && tentativas < 20);

            spawnAreas[linha][coluna] = 1; // Marca a nova área de spawn como ocupada //

            atual->coco.row = linha;
            atual->coco.col = coluna;
            atual->coco.x = coluna * 200 + 100;
            atual->coco.y = -(linha * 300) - GetRandomValue(50, 300);
            SetCocoType(&atual->coco);
        }
        atual = atual->next;
    }
}

// Limpa a memoria da lista de cocos //
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
            spawnAreas[i][j] = 0; // Reseta as áreas de spawn //
        }
    }
}

// Desenha os cocos na tela //
void DrawCocos(){
    Node *atual = listaCocos;
    while (atual != NULL) {
        Texture2D texturaAtual;

        if (atual->coco.type == 0) {
            texturaAtual = cocoTexture;
        } else if (atual->coco.type == 1) {
            if (atual->coco.sprite == 0) {
                texturaAtual = lataTexture;
            } else {
                texturaAtual = garraTexture;
            }
        } else if (atual->coco.type == 2) {
            texturaAtual = douradoTexture;
        } else {
            texturaAtual = aguaTexture;
        }

        DrawTextureEx(
            texturaAtual,
            (Vector2) {atual->coco.x - 32, atual->coco.y - 32},
            0.0f,
            1.0f,
            WHITE
        );

        atual = atual->next;
    }
}