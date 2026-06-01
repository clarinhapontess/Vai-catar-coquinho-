# 🌴 Vai Catar Coquinho! 🌴

## 🦀 Descrição:

**Vai Catar Coquinho!** é um jogo em linha de comando onde o jogador controla um personagem que deve se mover horizontalmente para capturar o maior número possível de cocos que caem do topo da tela.
**Objetivo:** Fazer a maior pontuação possível antes de perder todas as vidas.

## 🤔 Como funciona o jogo:

* O personagem se move para a esquerda e direita
* Cocos caem do topo da tela
* O jogador deve capturar os cocos se esquivando do lixo .
* cada lixo capturado tira uma vida do personagem .
* são 3 vidas 
### 🥥 Tipos de cocos:

* **Coco normal:** +1 ponto
* **Lixo na praia:** -1 vida
* **Coco dourado:** dobra os pontos por 20 seg
* **Água de coco:** +1 vida

💀 **Fim de jogo:** O jogador perde quando suas vidas chegam a 0.

## ✨ Equipe:

* Maria Clara Pontes — [mcpr2@cesar.school](mailto:mcpr2@cesar.school)
* Dandara Resende — [dmsrb@cesar.school](mailto:dmsrb@cesar.school)
* Leticia Soares — [lsncb@cesar.school](mailto:lsncb@cesar.school)

## ⚙️ Requisitos (conceitos da linguagem C:):

* Estruturas (`struct`)
* Ponteiros
* Alocação dinâmica de memória
* Listas encadeadas
* Matrizes
* Leitura e escrita em arquivos

## 🛠️ Ferramentas utilizadas

* Linguagem: C
* Ambiente: Linux / WSL / MacOS

### 📚 Biblioteca utilizada

> 🔗 (https://github.com/raysan5/raylib)

## 🎥 Vídeo do jogo funcionando

> 🔗 *(https://youtu.be/ZhzCPaU3L_8)*

## ▶️ Como compilar e executar:

No terminal WSL, compile o jogo com:

gcc src/*.c -o build/jogo -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

Depois execute com:

./build/jogo

## 📺 Slides de apresentação

### Entrega 1:

> 🔗 (https://canva.link/20psjj7ozorcdjp)

### Entrega 2:
canva
> 🔗 *(https://canva.link/j1c6jrjn37qzcms)*

## 📌 Status do projeto

* [x] Mecânica básica implementada
* [x] Sistema de vidas
* [x] Tipos de cocos
* [x] Sistema de pontuação
* [x] Mecânica avançada
* [x] Salvamento de top scores
* [x] Interface no terminal
* [x] Música e efeitos sonoros
* [x] tela de ínicio + game over
* [x] impementação de skins colecionáveis
* [x] Vídeo demonstrativo
* [x] Slides

