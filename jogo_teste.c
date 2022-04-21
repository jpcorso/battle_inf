#include "raylib.h"
#include <stdio.h>
#include <string.h>


#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600
#define MAX_PROJETEIS 1

typedef struct
{
    Vector2 posicao;
    Vector2 size;
    bool left;
    bool right;
    bool up;
    bool down;

    float rotation;

}JOGADOR;

typedef struct
{
    Vector2 posicao;
    Vector2 size;

    bool ativo;

    bool left;
    bool right;
    bool up;
    bool down;

}PROJETIL;

typedef struct
{
    Vector2 posicao;

}TIJOLOS;

void desenha_texturas(TIJOLOS*);
void movimento_jogador(JOGADOR*, float rotation);
void draw(JOGADOR*, PROJETIL[], float rotation);

void movimento_projetil(JOGADOR*, PROJETIL[]);

int main(void)
{
    float rotation;
    TIJOLOS tij;
    JOGADOR player;
    PROJETIL bullets[MAX_PROJETEIS];

    player.posicao = (Vector2) {(float)SCREEN_WIDTH / 2 , (float)SCREEN_HEIGHT / 2};
    player.size = (Vector2) {30 , 30};

    tij.posicao = (Vector2) {(float)0, (float)0};

    for(int i = 0; i < MAX_PROJETEIS; i++)
    {
        bullets[i].posicao = (Vector2) {(float)SCREEN_WIDTH / 2 , (float)SCREEN_HEIGHT / 2};
        bullets[i].size = (Vector2) {15, 15};
        bullets[i].ativo = false;

        bullets[i].right = false;
        bullets[i].left = false;
        bullets[i].up = false;
        bullets[i].down = false;
    }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BattleINF");
    SetTargetFPS(60);

    while(!WindowShouldClose())
    {
        movimento_jogador(&player, rotation);
        movimento_projetil(&player, bullets);
        draw(&player, bullets, rotation);
        //desenha_texturas(&tij);

    }

    CloseWindow();
    return 0;

}

void movimento_jogador(JOGADOR *player, float rotation)
{

    if(IsKeyDown(KEY_RIGHT) && player->posicao.x+player->size.x<SCREEN_WIDTH)
    {
        player->posicao.x = player->posicao.x + 3.5;
        player->right = true;
        player->left = false;
        player->up = false;
        player->down = false;
        rotation = 90;
    }
    else if (IsKeyDown(KEY_LEFT) && player->posicao.x >= 0)
    {
        player->posicao.x = player->posicao.x - 3.5;
        player->right = false;
        player->left = true;
        player->up = false;
        player->down = false;
        rotation = 270;
    }
    else if (IsKeyDown(KEY_UP) && player->posicao.y >= 0)
    {
        player->posicao.y = player->posicao.y - 3.5;
        player->right = false;
        player->left = false;
        player->up = true;
        player->down = false;
        rotation = 0;
    }
    else if (IsKeyDown(KEY_DOWN) && player->posicao.y+player->size.y<SCREEN_HEIGHT)
    {
        player->posicao.y = player->posicao.y + 3.5;
        player->right = false;
        player->left = false;
        player->up = false;
        player->down = true;
        rotation = 180;
    }
}

void desenha_texturas(TIJOLOS *tij){

    BeginDrawing();

    Texture2D tijolos = LoadTexture("public/brick_texture2.png");

    Rectangle tijolosRec = (Rectangle){
        0,0,tijolos.width,tijolos.height
    };

    //FILE *pontText;

    //tijolosText[600]

    //pontText = fopen("/home/jpcorso/projetos/programas/battle_inf/mapas/mapa1", "r");

    char tijolosText[600];
    strcpy(tijolosText, LoadFileText("mapas/mapa1"));
    int i;
    int j;

        for(i=0;i<15;i++){
            for(j=0;j<40;j++){
                tij->posicao.x += tijolos.width;
                if(tijolosText[i] == '#'){
                    DrawTextureRec(tijolos, tijolosRec, tij->posicao, RAYWHITE);
                }
            }
                    tij->posicao.x = 0;
                tij->posicao.y += tijolos.height; 
        }

    EndDrawing();

}

void draw(JOGADOR *player, PROJETIL bullets[], float rotation)
{

    /*//Load tank texture
    Texture2D tanquePlayer = LoadTexture("public/tanque_player_menor.png");
     // Source rectangle (part of the texture to use for drawing)

    int frameHeight = tanquePlayer.width;
    int frameWidth = tanquePlayer.height;

    Rectangle sourceRec = { 0.0f, 0.0f, frameWidth, frameHeight};
    // Destination rectangle (screen rectangle where drawing part of texture)
    Rectangle destRec = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, frameWidth*2.0f, frameHeight*2.0f};
    // Origin of the texture (rotation/scale point), it's relative to destination rectangle size
    Vector2 origin = { (float)frameWidth, (float)frameHeight };*/

    BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleV(player->posicao, player->size, MAROON);
        //DrawTexturePro(tanquePlayer, sourceRec, destRec, origin, rotation, RAYWHITE);
        for(int i = 0; i < MAX_PROJETEIS; i++)
        {
            if(bullets[i].ativo)
            {
                DrawRectangleV(bullets[i].posicao, bullets[i].size, GREEN);
            }
        }

    EndDrawing();
}

void movimento_projetil(JOGADOR *player, PROJETIL bullets[])
{
    if(IsKeyPressed(KEY_SPACE))
    {
        for (int i = 0; i < MAX_PROJETEIS; i++)
        {
            if(!bullets[i].ativo)
            {

                if(player[i].right == true)
                {
                    bullets[i].ativo = true;

                    bullets[i].right = true;
                    bullets[i].left = false;
                    bullets[i].up = false;
                    bullets[i].down = false;

                    bullets[i].posicao.x = player->posicao.x;
                    bullets[i].posicao.y = player->posicao.y;

                    break;

                }
                if (player[i].left == true)
                {
                    bullets[i].ativo = true;

                    bullets[i].right = false;
                    bullets[i].left = true;
                    bullets[i].up = false;
                    bullets[i].down = false;

                    bullets[i].posicao.x = player->posicao.x;
                    bullets[i].posicao.y = player->posicao.y;

                    break;
                }
                if (player[i].up == true)
                {
                    bullets[i].ativo = true;

                    bullets[i].right = false;
                    bullets[i].left = false;
                    bullets[i].up = true;
                    bullets[i].down = false;

                    bullets[i].posicao.x = player->posicao.x;
                    bullets[i].posicao.y = player->posicao.y;

                    break;
                }
                if (player[i].down == true)
                {
                    bullets[i].ativo = true;

                    bullets[i].right = false;
                    bullets[i].left = false;
                    bullets[i].up = false;
                    bullets[i].down = true;

                    bullets[i].posicao.x = player->posicao.x;
                    bullets[i].posicao.y = player->posicao.y;

                    break;
                }
            }
        }
    }

    for (int i = 0; i < MAX_PROJETEIS; i++)
    {

        if(bullets[i].ativo && bullets[i].right)
        {
            bullets[i].posicao.x = bullets[i].posicao.x + 20;
        }
        if(bullets[i].ativo && bullets[i].left)
        {
            bullets[i].posicao.x = bullets[i].posicao.x - 20;
        }
        if(bullets[i].ativo && bullets[i].up)
        {
            bullets[i].posicao.y = bullets[i].posicao.y - 20;
        }
        if(bullets[i].ativo && bullets[i].down)
        {
            bullets[i].posicao.y = bullets[i].posicao.y + 20;
        }

        if(bullets[i].ativo)
        {
            if (bullets[i].posicao.x < 0 || bullets[i].posicao.x > SCREEN_WIDTH || bullets[i].posicao.y < 0 || bullets[i].posicao.y > SCREEN_HEIGHT)
            {
                bullets[i].posicao = (Vector2) {(float)SCREEN_WIDTH / 2 , (float)SCREEN_HEIGHT / 2};
                bullets[i].size = (Vector2) {15, 15};
                bullets[i].ativo = false;

                bullets[i].right = false;
                bullets[i].left = false;
                bullets[i].up = false;
                bullets[i].down = false;
            }
        }
    }
}
