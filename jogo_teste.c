#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 660
#define MAX_PROJETEIS 1
#define LINHAS 15
#define COLUNAS 40 + 1 //Esse +1 pq quando chega no final tem um comando ou algo parecido pra indicar nova linha, por isso antes ficava uma escada

// Struct para armazenar informacoes do jogador
typedef struct
{
    Vector2 posicao;
    Vector2 size;
    bool left;
    bool right;
    bool up;
    bool down;

    int vidas;
    int velocidade;

}JOGADOR;

// Struct para armazenar informacoes de projeteis
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

// Struct para armazenar informacoes de tijolos
typedef struct
{
    Vector2 posicao;
    Vector2 size;

    bool ativo;
    char elemento;

}TIJOLOS;

//struct para as celulas de energia
typedef struct
{
    Vector2 posicao;
    Vector2 size;

    bool alive;

}CELULA;

// Essa funcao calcula se determinado bloco deve ser colocado ou nao
void coloca_tijolo(TIJOLOS[][COLUNAS]);
// essa funcao calcula e atualiza o movimento do jogador
void movimento_jogador(JOGADOR*, Rectangle);
// Essa funcao desenha o jogo
void draw(JOGADOR*, PROJETIL[], TIJOLOS[][COLUNAS],Vector2 posicao, Vector2 size);
// Essa funcao calcula e atualiza o movimento dos projeteis
void movimento_projetil(JOGADOR*, PROJETIL[]);
//Essa função coloca as células de energia
void celulasEnergia(CELULA*, JOGADOR*);

int main(void)
{
    // Inicializar o array que vai armazenar a string do mapa do jogo
    char tijolosText[600 + 15]; // Esses +15 por causa daquele comando de nova linha

    Rectangle tijolosRec = (Rectangle) {0,0,25,40};

    // Variaveis utilizadas
    TIJOLOS tij[LINHAS][COLUNAS];
    JOGADOR player;
    PROJETIL bullets[MAX_PROJETEIS];
    CELULA energia;

    // Inicializacao do jogador
    player.posicao = (Vector2) {(float)SCREEN_WIDTH / 2 , (float)SCREEN_HEIGHT / 2};
    player.size = (Vector2) {30 , 30};
    player.velocidade = 3.5f;

    //Inicialização das celulas
    energia.size = (Vector2) {25, 40};

    // Inicializacao dos projeteis
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

    // Inicializacao dos blocos do mapa
    strcpy(tijolosText, LoadFileText("mapas/mapa1"));
    int contador_string_mapa = 0;
    int contador_linhas = 0;
    int contador_colunas = 0;

    for(int i = 0; i < LINHAS; i++)
    {
        contador_colunas = 0;

        for(int j = 0; j < COLUNAS; j++)
        {
            tij[i][j].posicao.y = 0 + (contador_linhas * 40);
            tij[i][j].posicao.x = 0 + (contador_colunas * 25);
            tij[i][j].elemento = tijolosText[contador_string_mapa];
            // Esses prints sao para debug
            //printf("char : %c\n", tij[i][j].elemento);
            //printf("contador linhas: %d\n", contador_linhas);
            //printf("contador colunas: %d\n", contador_colunas);
            //printf("pos x: %f\n", tij[i][j].posicao.x);
            //printf("pos y: %f\n", tij[i][j].posicao.y);

            tij[i][j].size = (Vector2) {25, 40};
            tij[i][j].ativo = false;
            //printf("numero %d\n", contador_string_mapa);

            contador_colunas++;
            contador_string_mapa++;
        }
        contador_linhas++;
    }

    // Faz a janela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BattleINF");
    SetTargetFPS(60);

    // Loop do jogo
    while(!WindowShouldClose())
    {
        movimento_jogador(&player, tijolosRec);
        movimento_projetil(&player, bullets);
        coloca_tijolo(tij);
        draw(&player, bullets, tij, player.posicao, player.size);
        celulasEnergia(&energia, &player);
    
    }

    CloseWindow();
    return 0;

}

void celulasEnergia(CELULA *energia, JOGADOR *player){


    Texture2D celulasText = LoadTexture("public/energy_drop_menor.png");

    int randLin, randCol;
    int i, j;

    srand(time(NULL));

        //Precisa achar uma maneira de não trocar com tanta frequencia
        for(i=0;i<2;i++){
            int randLin = GetRandomValue(0, 1000);
            int randCol = GetRandomValue(0, 600);      
            //int randLin = rand()%1000;
            //int randCol = rand()%600;
        
            DrawTexture(celulasText, randLin, randCol, RAYWHITE);
        
        }

    if(&energia->posicao == &player->posicao){

        //Como manter essa velocidade?
        player->velocidade = 5.25f;
        //projeteis aq

    }



}

// Calcula se o player esta indo para direita, esquerda, cima ou baixo
void movimento_jogador(JOGADOR *player, Rectangle tijolosRec)
{
    if(IsKeyDown(KEY_RIGHT) && (player->posicao.x+player->size.x<SCREEN_WIDTH))
    {
        player->posicao.x = player->posicao.x + player->velocidade;
        player->right = true;
        player->left = false;
        player->up = false;
        player->down = false;
    }
    else if (IsKeyDown(KEY_LEFT) && (player->posicao.x >= 0))
    {
        player->posicao.x = player->posicao.x - player->velocidade;
        player->right = false;
        player->left = true;
        player->up = false;
        player->down = false;
    }
    else if (IsKeyDown(KEY_UP) && (player->posicao.y >= 60))
    {
        player->posicao.y = player->posicao.y - player->velocidade;
        player->right = false;
        player->left = false;
        player->up = true;
        player->down = false;
    }
    else if (IsKeyDown(KEY_DOWN) && (player->posicao.y+player->size.y<SCREEN_HEIGHT))
    {
        player->posicao.y = player->posicao.y + player->velocidade;
        player->right = false;
        player->left = false;
        player->up = false;
        player->down = true;
    }
}

// essa funcao desenha o jogo
void draw(JOGADOR *player, PROJETIL bullets[], TIJOLOS tij[][COLUNAS], Vector2 posicao, Vector2 size)
{
    // Comeca desenho
    BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(0, 0, 1000, 60, RED);
        DrawText("Fase 1", SCREEN_WIDTH/2 - 50, 0, 50, BLACK);
        

        Texture2D tijolo_textura = LoadTexture("public/brick_texture2.png");

        //valores para calcular colisões
        Rectangle tijolosRec = (Rectangle) {0,0,25,40};
        Vector2 posicaoBackup = posicao;
        Rectangle playerRec = {posicao.x, posicao.y, size.x, size.y};
        Rectangle tijInter = GetCollisionRec(playerRec, tijolosRec);

        // Desenha player (ainda sem textura)
        DrawRectangleV(player->posicao, player->size, MAROON);

        // Esse loop desenha os projeteis (ainda sem textura)
        for(int i = 0; i < MAX_PROJETEIS; i++)
        {
            if(bullets[i].ativo)
            {
                DrawRectangleV(bullets[i].posicao, bullets[i].size, GREEN);
            }
        }

        // Esse loop desenha os tijolos ativos do mapa
        for(int i = 0; i < LINHAS; i++)
        {
            for(int j = 0; j < COLUNAS; j++)
            {
                if(tij[i][j].ativo)
                {
                    //printf("%c\n" , tij[i][j].elemento);
                    DrawTextureRec(tijolo_textura, tijolosRec , tij[i][j].posicao, RAYWHITE);

                    
                }
                    //DrawRectangleV(tij[i][j].posicao, tij[i][j].size, BLACK);
            }
        }

        //posicao = posicaoBackup;
        //checa colisão do vetor com os tijolos
        

    // Finaliza desenho
    EndDrawing();
}


// Atualiza o movimento dos projeteis
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

// Verifica se o elemento do arquivo mapa eh um tijolo a ser colocado
void coloca_tijolo(TIJOLOS tij[][COLUNAS])
{
    for(int i = 0; i < LINHAS; i++)
    {
        for(int j = 0; j < COLUNAS; j++)
        {
             if(tij[i][j].elemento == '#')
             {
                 tij[i][j].ativo = true;
             }
        }

    }
}

