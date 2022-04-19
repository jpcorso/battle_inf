#include "raylib.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600

typedef struct {
    Vector2 Posicao;
    Vector2 Tamanho;


    int Municao;
    int Vidas;
    int Pontos;
} Jogador;

void init(Jogador*);
void update(Jogador *jog, Texture2D tanquePlayer, Rectangle sourceRec, Rectangle destRec, Vector2 origin);
void draw(Jogador, Texture2D tanquePlayer,  Rectangle sourceRec, Rectangle destRec, Vector2 origin);

int main(void) {
    Jogador jog;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Meu jogador");

    init(&jog);

    //texturas
    Texture2D tanquePlayer = LoadTexture("./public/tanque_player_menor.png");
   

    int frameWidth = tanquePlayer.width;
    int frameHeight = tanquePlayer.height;

    // Source rectangle (part of the texture to use for drawing)
    Rectangle sourceRec = { 0.0f, 0.0f, (float)frameWidth, (float)frameHeight};

    // Destination rectangle (screen rectangle where drawing part of texture)
    Rectangle destRec = { SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f, frameWidth*2.0f, frameHeight*2.0f};

    // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
    Vector2 origin = { (float)frameWidth, (float)frameHeight };

    Vector2 Posicao = {(float)SCREEN_WIDTH/2.0f, (float)SCREEN_HEIGHT/2.0f};

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------


    // Main game loop
    while (!WindowShouldClose()) {  // Detect window close button or ESC key
        update(&jog, tanquePlayer, sourceRec, destRec, origin);
        draw(jog, tanquePlayer, sourceRec, destRec, origin);

    }

    CloseWindow();

    return 0;
}

void init (Jogador *jog){
    jog->Posicao = (Vector2) {
        (float)SCREEN_WIDTH/2, (float)SCREEN_HEIGHT/2
    };   
    jog->Tamanho = (Vector2){
        50, 50
    };
}

void update (Jogador *jog, Texture2D tanquePlayer, Rectangle sourceRec, Rectangle destRec, Vector2 origin) {
    if (IsKeyDown(KEY_RIGHT) && jog->Posicao.x+jog->Tamanho.x < SCREEN_WIDTH) {
        DrawTexturePro(tanquePlayer, sourceRec, destRec, origin, (float)90, RAYWHITE);
        jog->Posicao.x += 4.0f;
    }
    else{
        if (IsKeyDown(KEY_LEFT) && jog->Posicao.x >= 0){
            //DrawTexturePro(tanquePlayer, sourceRec, destRec, origin, (float)270, RAYWHITE);
            jog->Posicao.x -= 4.0f;
    }
        else{
            if (IsKeyDown(KEY_UP) && jog->Posicao.y >= 0){
                //DrawTexturePro(tanquePlayer, sourceRec, destRec, origin, (float)0, RAYWHITE);
                jog->Posicao.y -= 4.0f;
            }
            else{
                if (IsKeyDown(KEY_DOWN) && jog->Posicao.y+jog->Tamanho.y < SCREEN_HEIGHT){
                    //DrawTexturePro(tanquePlayer, sourceRec, destRec, origin, (float)180, RAYWHITE);
                    jog->Posicao.y += 4.0f;
                }
                else{
                    //DrawTexturePro(tanquePlayer, sourceRec, destRec, origin, (float)90, RAYWHITE);
                }
    
            }
        }
    }
}
   

void draw (Jogador jog, Texture2D tanquePlayer,  Rectangle sourceRec, Rectangle destRec, Vector2 origin) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

        DrawTexturePro(tanquePlayer, sourceRec, destRec, origin, (float)90, RAYWHITE);

    EndDrawing();
}
