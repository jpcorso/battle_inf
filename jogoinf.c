#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600

typedef struct {
    Vector2 Posicao;
    Vector2 Tamanho;

    int Municao;
    int Vidas;
    int Pontos;
} Jogador;

typedef struct{

}

void init(Jogador*);
void update(Jogador*);
void draw(Jogador);

int main(void) {
    Jogador jog;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Meu jogador");

    init(&jog);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

   

    // Main game loop
    while (!WindowShouldClose()) {  // Detect window close button or ESC key

    Vector2 Posicao;

        update(&jog);
        draw(jog);

    }

    CloseWindow();

    return 0;
}

void init (Jogador *jog) {
    jog->Posicao = (Vector2) {
        (float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2
    };
    jog->Tamanho = (Vector2) {
        40, 40
    };
}

void update (Jogador *jog) {
    if (IsKeyDown(KEY_RIGHT) && jog->Posicao.x+jog->Tamanho.x < SCREEN_WIDTH){
        jog->Posicao.x += 3.5f;
    }
    else{
        if (IsKeyDown(KEY_LEFT) && jog->Posicao.x >= 0) {
            jog->Posicao.x -= 3.5f;
        }
        else{
            if (IsKeyDown(KEY_UP) && jog->Posicao.y >= 0) {
                jog->Posicao.y -= 3.5f;
            }
            else{
                if (IsKeyDown(KEY_DOWN) && jog->Posicao.y+jog->Tamanho.y < SCREEN_HEIGHT) {
                    jog->Posicao.y += 3.5f;
                }
                else{
                    if (IsKeyDown(KEY_SPACE)){
                        DrawRectangleV(jog->Posicao, )
                    }
                }
            }
        }
    }
}

void draw (Jogador jog) {
    BeginDrawing();

    FILE *pontText;
    char tijolosText[100];

    pontText = fopen("/home/jpcorso/projetos/programas/battle_inf/mapas/mapa1", tijolosText);


    ClearBackground(RAYWHITE);

    DrawText("Mova o jogador com as setinhas do teclado", 10, 10, 20, DARKGRAY);

    if(pontText == '#'){
        DrawRectangle(30, 60, 20, 20, ORANGE);
    }

    DrawRectangleV(jog.Posicao, jog.Tamanho, MAROON);

    fclose(pontText);

    EndDrawing();
}
