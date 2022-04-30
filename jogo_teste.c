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
    Vector2 posicao_backup;
    Vector2 size;
    Rectangle player_rec;

    bool left;
    bool right;
    bool up;
    bool down;

    int vidas;
    int velocidade;
    int boost_velocidade;
    int rotation;

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

    int boost_velocidade;

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
    bool boost_ativo;

}CELULA;

// Essa funcao calcula se determinado bloco deve ser colocado ou nao
void coloca_tijolo(TIJOLOS[][COLUNAS]);

// essa funcao calcula e atualiza o movimento do jogador
void movimento_jogador(JOGADOR*, TIJOLOS[][COLUNAS]);

// Essa funcao desenha o jogo
void draw(JOGADOR*, PROJETIL[], TIJOLOS[][COLUNAS], CELULA*);

void desenha_menu(int TELA);

// Essa funcao calcula e atualiza o movimento dos projeteis
void movimento_projetil(JOGADOR*, PROJETIL[], TIJOLOS[][COLUNAS]);

//Essa função coloca as células de energia
void celulasEnergia(CELULA*, JOGADOR*, PROJETIL[]);

// Essa funçao verifica colisao entre o player e tijolos
bool verifica_colisao_player(JOGADOR*, TIJOLOS[][COLUNAS]);

// Essa funcao verifica colisao entra projetil e tijolos
bool verifica_colisao_projetil(PROJETIL[], TIJOLOS[][COLUNAS]);

bool timer(int* , int*);


int main(void)
{
    int TELA = 0;
    int marcador_tela = 0;
    int menu[5] = {1 , 2 , 3 , 4 , 5};
    int GAMEPLAY = 5;
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
    player.size = (Vector2) {30 , 30};
    player.velocidade = 4;
    player.boost_velocidade = 0;
    player.player_rec.x = (float)SCREEN_WIDTH / 2;
    player.player_rec.y = (float)SCREEN_HEIGHT / 2;
    player.player_rec.width = 30;
    player.player_rec.height = 30;

    player.right = true;
    player.left = false;
    player.up = false;
    player.down = false;

    //Inicialização das celulas
    energia.size = (Vector2) {25, 40};
    energia.alive = false;
    energia.boost_ativo = false;

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

        bullets[i].boost_velocidade = 0;
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
            tij[i][j].posicao.y = 60 + (contador_linhas * 40);
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
        movimento_jogador(&player, tij);
        movimento_projetil(&player, bullets, tij);
        coloca_tijolo(tij);
        draw(&player, bullets, tij, &energia);
        celulasEnergia(&energia, &player, bullets);
    }

    CloseWindow();
    return 0;

}

void celulasEnergia(CELULA *energia, JOGADOR *player, PROJETIL bullets[]){

    srand(time(NULL));

    int celula_spawn = GetRandomValue(0, 5);
    //Precisa achar uma maneira de não trocar com tanta frequencia
    if(celula_spawn == 0 && energia->alive == false)
    {
            energia->posicao.x = GetRandomValue(0, 1000);
            energia->posicao.y = GetRandomValue(0, 600);

            energia->energia_rec.x = energia->posicao.x;
            energia->energia_rec.y = energia->posicao.y;
            energia->energia_rec.width = 25;
            energia->energia_rec.height = 40;

            energia->alive = true;
    }


    if(CheckCollisionRecs(energia->energia_rec, player->player_rec))
    {
        int tempo = 0;
        int frames = 0;
        energia->alive = false;
        energia->boost_ativo = true;

    }

    if(energia->boost_ativo)
    {
        player->boost_velocidade = 2;
        for(int i = 0; i < MAX_PROJETEIS; i++)
        {
            bullets[i].boost_velocidade = 10;
        }

    }
    else
    {
        player->boost_velocidade = 0;
        for(int i = 0; i < MAX_PROJETEIS; i++)
        {
            bullets[i].boost_velocidade = 0;
        }
        energia->boost_ativo = false;

    }
}

// Calcula se o player esta indo para direita, esquerda, cima ou baixo
void movimento_jogador(JOGADOR *player, TIJOLOS tij[][COLUNAS])
{
    if(IsKeyDown(KEY_RIGHT) && (player->posicao.x+player->size.x<SCREEN_WIDTH))
    {
        player->posicao_backup.x = player->posicao.x;
        player->posicao_backup.y = player->posicao.y;

        player->posicao.x = player->posicao.x + player->velocidade + player->boost_velocidade;
        player->right = true;
        player->left = false;
        player->up = false;
        player->down = false;

        player->player_rec.x = player->posicao.x;
        player->player_rec.y = player->posicao.y;

        player->rotation = 90;

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

        player->posicao.x = player->posicao.x - player->velocidade - player->boost_velocidade;
        player->right = false;
        player->left = true;
        player->up = false;
        player->down = false;

        player->rotation = 270;

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
    else if (IsKeyDown(KEY_UP) && (player->posicao.y >= 60))
    {
        player->posicao_backup.x = player->posicao.x;
        player->posicao_backup.y = player->posicao.y;

        player->posicao.y = player->posicao.y - player->velocidade - player->boost_velocidade;
        player->right = false;
        player->left = false;
        player->up = true;
        player->down = false;

        player->rotation = 0;

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

        player->posicao.y = player->posicao.y + player->velocidade + player->boost_velocidade;
        player->right = false;
        player->left = false;
        player->up = false;
        player->down = true;

        player->rotation = 180;

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
}

// essa funcao desenha o jogo
void draw(JOGADOR *player, PROJETIL bullets[], TIJOLOS tij[][COLUNAS], CELULA *energia)
{
    // Comeca desenho
    BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(0, 0, 1000, 60, RED);
        DrawText("Fase 1", ((SCREEN_WIDTH - MeasureText("Fase 1", 50)) / 2), 10, 50, BLACK);

        // carregar texturas
        Texture2D tijolo_textura = LoadTexture("public/brick_texture2.png");
        Texture2D celulasText = LoadTexture("public/energy_drop_menor.png");
        Texture2D tanquePlayer = LoadTexture("public/tanque_player.png");
        Texture2D projetilText = LoadTexture("public/PNG/Effects/Sprites/Sprite_Fire_Shots_Flame_007.png");


        //valores para calcular colisões
        Rectangle tijolosRec = (Rectangle) {0,0,25,40};

        // Desenha player (ainda sem textura)
        //DrawRectangleV(player->posicao, player->size, MAROON);
        if(player->right)
        {
            DrawTexturePro(tanquePlayer, (Rectangle){0, 0, 510, 510}, player->player_rec,(Vector2) {0 , 30}, player->rotation, RAYWHITE);
        }
        if(player->left)
        {
            DrawTexturePro(tanquePlayer, (Rectangle){0, 0, 510, 510}, player->player_rec,(Vector2){30 , 0}, player->rotation, RAYWHITE);
        }
        if(player->up)
        {
            DrawTexturePro(tanquePlayer, (Rectangle){0, 0, 510, 510}, player->player_rec,(Vector2){0 , 0}, player->rotation, RAYWHITE);
        }
        if(player->down)
        {
            DrawTexturePro(tanquePlayer, (Rectangle){0, 0, 510, 510}, player->player_rec,(Vector2){30 , 30}, player->rotation, RAYWHITE);
        }

        // Esse loop desenha os projeteis (ainda sem textura)
        for(int i = 0; i < MAX_PROJETEIS; i++)
        {
            if(bullets[i].ativo)
            {
                //DrawRectangleV(bullets[i].posicao, bullets[i].size, GREEN);
                if(bullets[i].right)
                {
                    DrawTexturePro(projetilText,(Rectangle){0, 0, 256, 256}, bullets[i].bullet_rec, (Vector2) {20 , 0}, 270, RAYWHITE);
                }
                if(bullets[i].left)
                {
                    DrawTexturePro(projetilText,(Rectangle){0, 0, 256, 256}, bullets[i].bullet_rec, (Vector2) {-10 , 10}, 90, RAYWHITE);
                }
                if(bullets[i].up)
                {
                    DrawTexturePro(projetilText,(Rectangle){0, 0, 256, 256}, bullets[i].bullet_rec, (Vector2) {20 , 30}, 180, RAYWHITE);
                }
                if(bullets[i].down)
                {
                    DrawTexturePro(projetilText,(Rectangle){0, 0, 256, 256}, bullets[i].bullet_rec, (Vector2) {-10 , 0}, 0, RAYWHITE);
                }
            }
        }

        // Esse loop desenha os tijolos ativos do mapa
        for(int i = 0; i < LINHAS; i++)
        {
            for(int j = 0; j < COLUNAS; j++)
            {
                if(tij[i][j].ativo)
                {
                    //DrawTextureRec(tijolo_textura, tijolosRec , tij[i][j].posicao, RAYWHITE);
                    DrawTexturePro(tijolo_textura, (Rectangle){0, 0, 125, 125}, tij[i][j].tij_rec, (Vector2) {0,0}, 0, RAYWHITE);
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
            bullets[i].posicao.x = bullets[i].posicao.x + 20 + bullets[i].boost_velocidade;

            bullets[i].bullet_rec.x = bullets[i].posicao.x;
            bullets[i].bullet_rec.y = bullets[i].posicao.y;

            bullets[i].bullet_rec.width = bullets[i].size.x;
            bullets[i].bullet_rec.height = bullets[i].size.y;

        }
        if(bullets[i].ativo && bullets[i].left)
        {
            bullets[i].posicao.x = bullets[i].posicao.x - 20 - bullets[i].boost_velocidade;

            bullets[i].bullet_rec.x = bullets[i].posicao.x;
            bullets[i].bullet_rec.y = bullets[i].posicao.y;

            bullets[i].bullet_rec.width = bullets[i].size.x;
            bullets[i].bullet_rec.height = bullets[i].size.y;
        }
        if(bullets[i].ativo && bullets[i].up)
        {
            bullets[i].posicao.y = bullets[i].posicao.y - 20 - bullets[i].boost_velocidade;

            bullets[i].bullet_rec.x = bullets[i].posicao.x;
            bullets[i].bullet_rec.y = bullets[i].posicao.y;

            bullets[i].bullet_rec.width = bullets[i].size.x;
            bullets[i].bullet_rec.height = bullets[i].size.y;
        }
        if(bullets[i].ativo && bullets[i].down)
        {
            bullets[i].posicao.y = bullets[i].posicao.y + 20 + bullets[i].boost_velocidade;

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

bool timer(int *tempo, int *frames)
{
    bool time = true;
    *frames++;
    if (*frames >= 60)
    {
        *frames = 0;
        *tempo++;
    }
    if (*tempo >= 15)
    {
        *tempo = 0;
        bool time = false;
    }

    return time;
}
// tentei fazer um negocio aqui e nao deu
/*void desenha_menu(int TELA)
{
    BeginDrawing();
    ClearBackground(BLACK);
    switch(TELA)
            {
                case 0:
                {
                    // TODO: Draw TITLE screen here!
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
                    DrawText("BATTLEINF", ((SCREEN_WIDTH - MeasureText("BATTLEINF", 60)) / 2), 100, 60, DARKGREEN);
                    DrawText("NOVO JOGO", ((SCREEN_WIDTH - MeasureText("NOVO JOGO", 20)) / 2), 220, 20, RED);
                    DrawText("CONTINUAR", ((SCREEN_WIDTH - MeasureText("CONTINUAR", 20)) / 2), 250, 20, WHITE);
                    DrawText("CARREGAR", ((SCREEN_WIDTH - MeasureText("CARREGAR", 20)) / 2), 280, 20, WHITE);
                    DrawText("HIGHSCORES", ((SCREEN_WIDTH - MeasureText("HIGHSCORES", 20)) / 2), 310, 20, WHITE);
                    DrawText("SAIR", ((SCREEN_WIDTH - MeasureText("SAIR", 20)) / 2), 340, 20, WHITE);

                } break;
                case 1:
                {
                    // TODO: Draw TITLE screen here!
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
                    DrawText("BATTLEINF", ((SCREEN_WIDTH - MeasureText("BATTLEINF", 60)) / 2), 100, 60, DARKGREEN);
                    DrawText("NOVO JOGO", ((SCREEN_WIDTH - MeasureText("NOVO JOGO", 20)) / 2), 220, 20, RED);
                    DrawText("CONTINUAR", ((SCREEN_WIDTH - MeasureText("CONTINUAR", 20)) / 2), 250, 20, WHITE);
                    DrawText("CARREGAR", ((SCREEN_WIDTH - MeasureText("CARREGAR", 20)) / 2), 280, 20, WHITE);
                    DrawText("HIGHSCORES", ((SCREEN_WIDTH - MeasureText("HIGHSCORES", 20)) / 2), 310, 20, WHITE);
                    DrawText("SAIR", ((SCREEN_WIDTH - MeasureText("SAIR", 20)) / 2), 340, 20, WHITE);

                } break;
                case 2:
                {
                    // TODO: Draw TITLE screen here!
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
                    DrawText("BATTLEINF", ((SCREEN_WIDTH - MeasureText("BATTLEINF", 60)) / 2), 100, 60, DARKGREEN);
                    DrawText("NOVO JOGO", ((SCREEN_WIDTH - MeasureText("NOVO JOGO", 20)) / 2), 220, 20, RED);
                    DrawText("CONTINUAR", ((SCREEN_WIDTH - MeasureText("CONTINUAR", 20)) / 2), 250, 20, WHITE);
                    DrawText("CARREGAR", ((SCREEN_WIDTH - MeasureText("CARREGAR", 20)) / 2), 280, 20, WHITE);
                    DrawText("HIGHSCORES", ((SCREEN_WIDTH - MeasureText("HIGHSCORES", 20)) / 2), 310, 20, WHITE);
                    DrawText("SAIR", ((SCREEN_WIDTH - MeasureText("SAIR", 20)) / 2), 340, 20, WHITE);

                } break;
                case 3:
                {
                    // TODO: Draw TITLE screen here!
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
                    DrawText("BATTLEINF", ((SCREEN_WIDTH - MeasureText("BATTLEINF", 60)) / 2), 100, 60, DARKGREEN);
                    DrawText("NOVO JOGO", ((SCREEN_WIDTH - MeasureText("NOVO JOGO", 20)) / 2), 220, 20, RED);
                    DrawText("CONTINUAR", ((SCREEN_WIDTH - MeasureText("CONTINUAR", 20)) / 2), 250, 20, WHITE);
                    DrawText("CARREGAR", ((SCREEN_WIDTH - MeasureText("CARREGAR", 20)) / 2), 280, 20, WHITE);
                    DrawText("HIGHSCORES", ((SCREEN_WIDTH - MeasureText("HIGHSCORES", 20)) / 2), 310, 20, WHITE);
                    DrawText("SAIR", ((SCREEN_WIDTH - MeasureText("SAIR", 20)) / 2), 340, 20, WHITE);

                } break;
                case 4:
                {
                    // TODO: Draw TITLE screen here!
                    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
                    DrawText("BATTLEINF", ((SCREEN_WIDTH - MeasureText("BATTLEINF", 60)) / 2), 100, 60, DARKGREEN);
                    DrawText("NOVO JOGO", ((SCREEN_WIDTH - MeasureText("NOVO JOGO", 20)) / 2), 220, 20, RED);
                    DrawText("CONTINUAR", ((SCREEN_WIDTH - MeasureText("CONTINUAR", 20)) / 2), 250, 20, WHITE);
                    DrawText("CARREGAR", ((SCREEN_WIDTH - MeasureText("CARREGAR", 20)) / 2), 280, 20, WHITE);
                    DrawText("HIGHSCORES", ((SCREEN_WIDTH - MeasureText("HIGHSCORES", 20)) / 2), 310, 20, WHITE);
                    DrawText("SAIR", ((SCREEN_WIDTH - MeasureText("SAIR", 20)) / 2), 340, 20, WHITE);

                } break;
            }
            EndDrawing();
}*/

