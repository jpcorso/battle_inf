#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600
#define MAX_PROJETEIS 1
#define LINHAS 15
#define COLUNAS 40 + 1 //Esse +1 pq quando chega no final tem um comando ou algo parecido pra indicar nova linha, por isso antes ficava uma escada

// Struct para armazenar informacoes do jogador
typedef struct
{
    Vector2 posicao;
    Vector2 posicao_backup;
    Vector2 size;
    Rectangle player_rec;

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
    Vector2 posicao_backup;
    Rectangle bullet_rec;

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
    Rectangle tij_rec;

    bool ativo;
    bool atingido_projetil;
    char elemento;

}TIJOLOS;

//struct para as celulas de energia
typedef struct
{
    Vector2 posicao;
    Vector2 size;
    Rectangle energia_rec;

    bool alive;

}CELULA;

// Essa funcao calcula se determinado bloco deve ser colocado ou nao
void coloca_tijolo(TIJOLOS[][COLUNAS]);

// essa funcao calcula e atualiza o movimento do jogador

void movimento_jogador(JOGADOR*, TIJOLOS[][COLUNAS], CELULA*);

// Essa funcao desenha o jogo
void draw(JOGADOR*, PROJETIL[], TIJOLOS[][COLUNAS], CELULA*);

// Essa funcao calcula e atualiza o movimento dos projeteis
void movimento_projetil(JOGADOR*, PROJETIL[], TIJOLOS[][COLUNAS]);

//Essa função coloca as células de energia
void celulasEnergia(CELULA*, JOGADOR*);

// Essa funçao verifica colisao entre o player e tijolos
bool verifica_colisao_player(JOGADOR*, TIJOLOS[][COLUNAS]);


//Essa funcao verifica colisao entre os tijolos e os projeteis
bool verifica_colisao_projetil(PROJETIL[], TIJOLOS[][COLUNAS]);

//Essa funcao verifica colisao entre o player e as celulas
bool verifica_colisao_celulas(JOGADOR* , CELULA*);

int main(void)
{
    // Inicializar o array que vai armazenar a string do mapa do jogo
    char tijolosText[600 + 15]; // Esses +15 por causa daquele comando de nova linha

    // Variaveis utilizadas
    TIJOLOS tij[LINHAS][COLUNAS];
    JOGADOR player;
    PROJETIL bullets[MAX_PROJETEIS];
    CELULA energia;

    // Inicializacao do jogador
    player.posicao = (Vector2) {(float)SCREEN_WIDTH / 2 , (float)SCREEN_HEIGHT / 2};
    player.posicao_backup = (Vector2) {(float)SCREEN_WIDTH / 2 , (float)SCREEN_HEIGHT / 2};

    player.size = (Vector2) {50 , 50};
    player.velocidade = 3.5f;
    player.player_rec.x = (float)SCREEN_WIDTH / 2;
    player.player_rec.y = (float)SCREEN_HEIGHT / 2;
    player.player_rec.width = 50;
    player.player_rec.height = 50;

    //Inicialização das celulas
    energia.size = (Vector2) {25, 40};
    energia.alive = false;
    energia.energia_rec.width = 25;
    energia.energia_rec.height = 40;

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
            tij[i][j].atingido_projetil = false;
            //printf("numero %d\n", contador_string_mapa);

            tij[i][j].tij_rec.x = tij[i][j].posicao.x;
            tij[i][j].tij_rec.y = tij[i][j].posicao.y;
            tij[i][j].tij_rec.width = 25;
            tij[i][j].tij_rec.height = 40;

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

        movimento_jogador(&player, tij, &energia);
        movimento_projetil(&player, bullets, tij);
        coloca_tijolo(tij);
        draw(&player, bullets, tij, &energia);
        celulasEnergia(&energia, &player);

    }

    CloseWindow();
    return 0;

}

void celulasEnergia(CELULA *energia, JOGADOR *player){

    srand(time(NULL));

    int celula_spawn = GetRandomValue(0, 5);
    //Precisa achar uma maneira de não trocar com tanta frequencia
    if(celula_spawn == 0 && energia->alive == false)
    {
            energia->posicao.x = GetRandomValue(0, 1000);
            energia->posicao.y = GetRandomValue(0, 600);
            energia->alive = true;
    }

}

// Calcula se o player esta indo para direita, esquerda, cima ou baixo
void movimento_jogador(JOGADOR *player, TIJOLOS tij[][COLUNAS], CELULA *energia)

{

    Texture2D tanquePlayer = LoadTexture("public/tanque_player.png");
    float rotation;

    if(IsKeyDown(KEY_RIGHT) && (player->posicao.x+player->size.x<SCREEN_WIDTH))
    {
        player->posicao_backup.x = player->posicao.x;
        player->posicao_backup.y = player->posicao.y;

        if(verifica_colisao_celulas(player, energia)){
            player->posicao.x += 5.25f;
        }
        else{
            player->posicao.x = player->posicao.x + player->velocidade;
        }

        player->right = true;
        player->left = false;
        player->up = false;
        player->down = false;
      
        rotation = 90;

        player->player_rec.x = player->posicao.x;
        player->player_rec.y = player->posicao.y;

        if(verifica_colisao_player(player, tij))
        {
            player->posicao.x = player->posicao_backup.x;
            player->posicao.y = player->posicao_backup.y;

            player->player_rec.x = player->posicao_backup.x;
            player->player_rec.y = player->posicao_backup.y;
        }

    }
    else if (IsKeyDown(KEY_LEFT) && (player->posicao.x >= 0))
    {
        player->posicao_backup.x = player->posicao.x;
        player->posicao_backup.y = player->posicao.y;


        if(verifica_colisao_celulas(player, energia)){
            player->posicao.x -= 5.25f;
        }
        else{
            player->posicao.x = player->posicao.x - player->velocidade;
        }

        player->right = false;
        player->left = true;
        player->up = false;
        player->down = false;

        rotation = 270;

        player->player_rec.x = player->posicao.x;
        player->player_rec.y = player->posicao.y;

        if(verifica_colisao_player(player, tij))
        {
            player->posicao.x = player->posicao_backup.x;
            player->posicao.y = player->posicao_backup.y;

            player->player_rec.x = player->posicao_backup.x;
            player->player_rec.y = player->posicao_backup.y;
        }
    }
    else if (IsKeyDown(KEY_UP) && (player->posicao.y >= 0))
    {
        player->posicao_backup.x = player->posicao.x;
        player->posicao_backup.y = player->posicao.y;

        if(verifica_colisao_celulas(player, energia)){
            player->posicao.y -= 5.25f;
        }
        else{
            player->posicao.y = player->posicao.y - player->velocidade;
        }

        player->right = false;
        player->left = false;
        player->up = true;
        player->down = false;
      
        rotation = 0;

        player->player_rec.x = player->posicao.x;
        player->player_rec.y = player->posicao.y;

        if(verifica_colisao_player(player, tij))
        {
            player->posicao.x = player->posicao_backup.x;
            player->posicao.y = player->posicao_backup.y;

            player->player_rec.x = player->posicao_backup.x;
            player->player_rec.y = player->posicao_backup.y;
        }

    }
    else if (IsKeyDown(KEY_DOWN) && (player->posicao.y+player->size.y<SCREEN_HEIGHT))
    {
        player->posicao_backup.x = player->posicao.x;
        player->posicao_backup.y = player->posicao.y;

        if(verifica_colisao_celulas(player, energia)){
            player->posicao.y += 5.25f;
        }
        else{
            player->posicao.y = player->posicao.y + player->velocidade;
        }
        player->right = false;
        player->left = false;
        player->up = false;
        player->down = true;

        rotation = 180;

        player->player_rec.x = player->posicao.x;
        player->player_rec.y = player->posicao.y;

        if(verifica_colisao_player(player, tij))
        {
            player->posicao.x = player->posicao_backup.x;
            player->posicao.y = player->posicao_backup.y;

            player->player_rec.x = player->posicao_backup.x;
            player->player_rec.y = player->posicao_backup.y;
        }
    }

    DrawTexturePro(
        tanquePlayer,
        (Rectangle){0, 0, 510, 510},
        player->player_rec,
        (Vector2){25,25}, rotation, RAYWHITE);

}

// essa funcao desenha o jogo
void draw(JOGADOR *player, PROJETIL bullets[], TIJOLOS tij[][COLUNAS], CELULA *energia)
{
    // Comeca desenho
    BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(0, 0, 1000, 60, RED);
        DrawText("Fase 1", SCREEN_WIDTH/2 - 50, 0, 50, BLACK);

        // carregar texturas
        Texture2D tijolo_textura = LoadTexture("public/brick_texture2.png");
        Texture2D celulasText = LoadTexture("public/energy_drop_menor.png");

        //valores para calcular colisões
        Rectangle tijolosRec = (Rectangle) {0,0,25,40};

        // Desenha player (ainda sem textura)
        //DrawRectangleV(player->posicao, player->size, MAROON);

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
                    DrawTextureRec(tijolo_textura, tijolosRec , tij[i][j].posicao, RAYWHITE);
                }
            }
        }

        if(energia->alive)
        {
            DrawTexture(celulasText, energia->posicao.x, energia->posicao.y, RAYWHITE);
        }


    // Finaliza desenho
    EndDrawing();
}


// Atualiza o movimento dos projeteis
void movimento_projetil(JOGADOR *player, PROJETIL bullets[], TIJOLOS tij[][COLUNAS])
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

            bullets[i].bullet_rec.x = bullets[i].posicao.x;
            bullets[i].bullet_rec.y = bullets[i].posicao.y;

            bullets[i].bullet_rec.width = bullets[i].size.x;
            bullets[i].bullet_rec.height = bullets[i].size.y;

        }
        if(bullets[i].ativo && bullets[i].left)
        {
            bullets[i].posicao.x = bullets[i].posicao.x - 20;

            bullets[i].bullet_rec.x = bullets[i].posicao.x;
            bullets[i].bullet_rec.y = bullets[i].posicao.y;

            bullets[i].bullet_rec.width = bullets[i].size.x;
            bullets[i].bullet_rec.height = bullets[i].size.y;
        }
        if(bullets[i].ativo && bullets[i].up)
        {
            bullets[i].posicao.y = bullets[i].posicao.y - 20;

            bullets[i].bullet_rec.x = bullets[i].posicao.x;
            bullets[i].bullet_rec.y = bullets[i].posicao.y;

            bullets[i].bullet_rec.width = bullets[i].size.x;
            bullets[i].bullet_rec.height = bullets[i].size.y;
        }
        if(bullets[i].ativo && bullets[i].down)
        {
            bullets[i].posicao.y = bullets[i].posicao.y + 20;

            bullets[i].bullet_rec.x = bullets[i].posicao.x;
            bullets[i].bullet_rec.y = bullets[i].posicao.y;

            bullets[i].bullet_rec.width = bullets[i].size.x;
            bullets[i].bullet_rec.height = bullets[i].size.y;
        }

        if(bullets[i].ativo)
        {
            if(bullets[i].posicao.x < 0 || bullets[i].posicao.x > SCREEN_WIDTH || bullets[i].posicao.y < 0 || bullets[i].posicao.y > SCREEN_HEIGHT)
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

    verifica_colisao_projetil(bullets, tij);
}

// Verifica se o elemento do arquivo mapa eh um tijolo a ser colocado
void coloca_tijolo(TIJOLOS tij[][COLUNAS])
{
    for(int i = 0; i < LINHAS; i++)
    {
        for(int j = 0; j < COLUNAS; j++)
        {
             if(tij[i][j].elemento == '#' && !tij[i][j].atingido_projetil)
             {
                 tij[i][j].ativo = true;
             }
        }

    }
}


bool verifica_colisao_celulas(JOGADOR *player, CELULA *energia)
{
    bool colisao = false;

    for(int i = 0; i <  LINHAS; i++){

            for(int j = 0; j < COLUNAS; j++){

                if(CheckCollisionRecs(player->player_rec, energia->energia_rec)){
                    colisao = true;
                }

            }

        }    
   
    return colisao;
}

bool verifica_colisao_player(JOGADOR *player, TIJOLOS tij[][COLUNAS])
{
    bool colisao = false;

    for(int i = 0; i <  LINHAS; i++)
    {
        for(int j = 0; j < COLUNAS; j++)
        {

            if(CheckCollisionRecs(player->player_rec, tij[i][j].tij_rec) && tij[i][j].ativo)
            {
                colisao = true;
            }
        }
    }

    return colisao;
}

bool verifica_colisao_projetil(PROJETIL bullets[], TIJOLOS tij[][COLUNAS])
{
    bool colisao = false;

    for(int i = 0; i < LINHAS; i++)
    {
        for(int j = 0; j < COLUNAS; j++)
        {
            for(int k = 0; k < MAX_PROJETEIS; k++)
            {
                if((CheckCollisionRecs(bullets[k].bullet_rec, tij[i][j].tij_rec) && tij[i][j].ativo))
                {
                    tij[i][j].ativo = false;
                    tij[i][j].atingido_projetil = true;

                    bullets[k].posicao = (Vector2) {(float)SCREEN_WIDTH / 2 , (float)SCREEN_HEIGHT / 2};
                    bullets[k].size = (Vector2) {15, 15};
                    bullets[k].ativo = false;

                    bullets[k].right = false;
                    bullets[k].left = false;
                    bullets[k].up = false;
                    bullets[k].down = false;

                }
            }
        }
    }
}

