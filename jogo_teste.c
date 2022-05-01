#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 660
#define MAX_PROJETEIS 1
#define MAX_INIMIGOS 1
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

typedef struct
{
    Vector2 posicao;
    Vector2 posicao_backup;
    Vector2 size;
    Rectangle enemy_rec;

    bool left;
    bool right;
    bool up;
    bool down;

    bool ativo;

    int velocidade;
    int rotation;

}INIMIGOS;

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
void draw(JOGADOR*, PROJETIL[], TIJOLOS[][COLUNAS], CELULA*, INIMIGOS[], Texture2D, Texture2D, Texture2D, Texture2D, Texture2D);
// TODO (nao consegui fazer)
void desenha_menu(int TELA);

// Essa funcao calcula e atualiza o movimento dos projeteis
void movimento_projetil(JOGADOR*, PROJETIL[], TIJOLOS[][COLUNAS]);

//Essa função coloca as células de energia
void celulasEnergia(CELULA*, JOGADOR*, PROJETIL[], int*, int*);

//essa funcao eh tipo uma interface que vai coordenar todas as funcoes relativas aos inimigos
void inimigos(INIMIGOS[], int*, int*, int*, TIJOLOS[][COLUNAS]);

//verifica a nova direcao do inimigo
void direcao_inimigo(INIMIGOS[], int);

//atualiza a posicao do inimigo
void movimento_inimigo(INIMIGOS[], int, TIJOLOS[][COLUNAS]);

// verifica o tempo de spawn dos inimigos
bool spawn_inimigos(int*, int*);

// Essa funcao verifica colisao entre inimigos e tijolos
bool verifica_colisao_inimigo(INIMIGOS[], TIJOLOS[][COLUNAS]);

// Essa funçao verifica colisao entre o player e tijolos
bool verifica_colisao_player(JOGADOR*, TIJOLOS[][COLUNAS]);

// Essa funcao verifica colisao entra projetil e tijolos
bool verifica_colisao_projetil(PROJETIL[], TIJOLOS[][COLUNAS]);

// essa funcao eh o contador do bonus da celula de energia (15 seg)
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
    INIMIGOS enemy[MAX_INIMIGOS];

    // Inicializacao do jogador
    player.posicao = (Vector2) {(float)SCREEN_WIDTH / 2 , (float)SCREEN_HEIGHT / 2};
    player.posicao_backup = (Vector2) {(float)SCREEN_WIDTH / 2 , (float)SCREEN_HEIGHT / 2};
    player.size = (Vector2) {30 , 30};
    player.velocidade = 3;
    player.boost_velocidade = 0;
    player.player_rec.x = (float)SCREEN_WIDTH / 2;
    player.player_rec.y = (float)SCREEN_HEIGHT / 2;
    player.player_rec.width = 30;
    player.player_rec.height = 30;

    player.right = true;
    player.left = false;
    player.up = false;
    player.down = false;

    // Inicializacao dos inimigos
    for(int i = 0; i < MAX_INIMIGOS; i++)
    {
        enemy[i].posicao = (Vector2) {(float)SCREEN_WIDTH / 2 , (float)SCREEN_HEIGHT / 2};
        enemy[i].posicao_backup = (Vector2) {(float)SCREEN_WIDTH / 2 , (float)SCREEN_HEIGHT / 2};
        enemy[i].size = (Vector2) {30 , 30};
        enemy[i].velocidade = 3;
        enemy[i].enemy_rec.x = (float)SCREEN_WIDTH / 2;
        enemy[i].enemy_rec.y = (float)SCREEN_HEIGHT / 2;
        enemy[i].enemy_rec.width = 30;
        enemy[i].enemy_rec.height = 30;

        enemy[i].right = true;
        enemy[i].left = false;
        enemy[i].up = false;
        enemy[i].down = false;

        enemy[i].ativo = false;

    }

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
    int numero_inimigos = 0;

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

    // carregar texturas
        Texture2D tijolo_textura = LoadTexture("public/brick_texture2.png");
        Texture2D celulasText = LoadTexture("public/energy_drop_menor.png");
        Texture2D tanquePlayer = LoadTexture("public/tanque_player.png");
        Texture2D projetilText = LoadTexture("public/PNG/Effects/Sprites/Sprite_Fire_Shots_Flame_007.png");

        // TODO (eu tava colocando textura nos tanques inimigos mas nao consegui)
        Texture2D tanqueEnemy = LoadTexture("public/tanque_inimigo.png");
        /*
        Texture2D tanqueEnemy1 = LoadTexture("public/tanque_inimigo1.png");
        Texture2D tanqueEnemy2 = LoadTexture("public/tanque_inimigo2.png");
        Texture2D tanqueEnemy3 = LoadTexture("public/tanque_inimigo3.png");
        Texture2D tanqueEnemy4 = LoadTexture("public/tanque_inimigo4.png");
        */

    int frame = 0;
    int segundos_frame = 0;

    int frame_enemy = 0;
    int segundos_frame_enemy = 0;

    coloca_tijolo(tij);



    // Loop do jogo
    while(!WindowShouldClose())
    {
        movimento_jogador(&player, tij);
        movimento_projetil(&player, bullets, tij);
        //coloca_tijolo(tij);
        inimigos(enemy, &frame_enemy, &segundos_frame_enemy, &numero_inimigos, tij);

        draw(&player, bullets, tij, &energia, enemy,tijolo_textura, celulasText, tanquePlayer, projetilText,
             tanqueEnemy);

        celulasEnergia(&energia, &player, bullets, &frame, &segundos_frame);
    }

    CloseWindow();
    return 0;

}

void celulasEnergia(CELULA *energia, JOGADOR *player, PROJETIL bullets[], int *frames, int *segundos_frames){

    srand(time(NULL));


    int celula_spawn = GetRandomValue(0, 5);
    //Precisa achar uma maneira de não trocar com tanta frequencia
    if(celula_spawn == 0 && energia->alive == false)
    {
            energia->posicao.x = GetRandomValue(0, 1000);
            energia->posicao.y = GetRandomValue(60, 600);

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

    if(energia->boost_ativo && timer(frames, segundos_frames))
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
void draw(JOGADOR *player, PROJETIL bullets[], TIJOLOS tij[][COLUNAS], CELULA *energia, INIMIGOS enemy[],
          Texture2D tijolo_textura, Texture2D celulasText, Texture2D tanquePlayer, Texture2D projetilText,
          Texture2D tanqueEnemy)
{
    // Comeca desenho
    BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangle(0, 0, 1000, 60, RED);
        DrawText("Fase 1", ((SCREEN_WIDTH - MeasureText("Fase 1", 50)) / 2), 10, 50, BLACK);

        // carregar texturas
        //Texture2D tijolo_textura = LoadTexture("public/brick_texture2.png");
        //Texture2D celulasText = LoadTexture("public/energy_drop_menor.png");
        //Texture2D tanquePlayer = LoadTexture("public/tanque_player.png");
        //Texture2D projetilText = LoadTexture("public/PNG/Effects/Sprites/Sprite_Fire_Shots_Flame_007.png");


        //valores para calcular colisões
        Rectangle tijolosRec = (Rectangle) {0,0,25,40};

        // Desenha player
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

        // Esse loop desenha os projeteis
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
        // Desenha celulas
        if(energia->alive)
        {
            DrawTexture(celulasText, energia->posicao.x, energia->posicao.y, RAYWHITE);
        }

        // Inimigos
        for(int i = 0; i < MAX_INIMIGOS; i++)
        {
            if(enemy[i].ativo && enemy[i].right)
            {
                DrawRectangleV(enemy[i].posicao, enemy[i].size, MAROON);
                DrawTexturePro(tanqueEnemy,(Rectangle){0, 0, 510, 510}, enemy[0].enemy_rec, (Vector2) {0 , 30}, enemy[i].rotation, RAYWHITE);
            }
            if(enemy[i].ativo && enemy[i].left)
            {
                DrawRectangleV(enemy[i].posicao, enemy[i].size, MAROON);
                DrawTexturePro(tanqueEnemy,(Rectangle){0, 0, 510, 510}, enemy[0].enemy_rec, (Vector2) {30 , 0}, enemy[i].rotation, RAYWHITE);
            }
            if(enemy[i].ativo && enemy[i].up)
            {
                DrawRectangleV(enemy[i].posicao, enemy[i].size, MAROON);
                DrawTexturePro(tanqueEnemy,(Rectangle){0, 0, 510, 510}, enemy[0].enemy_rec, (Vector2) {0 , 0}, enemy[i].rotation, RAYWHITE);
            }
            if(enemy[i].ativo && enemy[i].down)
            {
                DrawRectangleV(enemy[i].posicao, enemy[i].size, MAROON);
                DrawTexturePro(tanqueEnemy,(Rectangle){0, 0, 510, 510}, enemy[0].enemy_rec, (Vector2) {30 , 30}, enemy[i].rotation, RAYWHITE);
            }
        }


    // Finaliza desenho
    EndDrawing();
}


// Atualiza o movimento dos projeteis
void movimento_projetil(JOGADOR *player, PROJETIL bullets[], TIJOLOS tij[][COLUNAS])
{
    if(IsKeyPressed(KEY_SPACE))
    {
        for (int k = 0; k < MAX_PROJETEIS; k++)
        {
            if(!bullets[k].ativo)
            {

                if(player[k].right == true)
                {
                    bullets[k].ativo = true;

                    bullets[k].right = true;
                    bullets[k].left = false;
                    bullets[k].up = false;
                    bullets[k].down = false;

                    bullets[k].posicao.x = player->posicao.x;
                    bullets[k].posicao.y = player->posicao.y;
                    break;


                }
                else if (player[k].left == true)
                {
                    bullets[k].ativo = true;

                    bullets[k].right = false;
                    bullets[k].left = true;
                    bullets[k].up = false;
                    bullets[k].down = false;

                    bullets[k].posicao.x = player->posicao.x;
                    bullets[k].posicao.y = player->posicao.y;
                    break;


                }
                else if (player[k].up == true)
                {
                    bullets[k].ativo = true;

                    bullets[k].right = false;
                    bullets[k].left = false;
                    bullets[k].up = true;
                    bullets[k].down = false;

                    bullets[k].posicao.x = player->posicao.x;
                    bullets[k].posicao.y = player->posicao.y;
                    break;

                }
                else if (player[k].down == true)
                {
                    bullets[k].ativo = true;

                    bullets[k].right = false;
                    bullets[k].left = false;
                    bullets[k].up = false;
                    bullets[k].down = true;

                    bullets[k].posicao.x = player->posicao.x;
                    bullets[k].posicao.y = player->posicao.y;
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

bool timer(int *frames, int *segundos_frames)
{
    bool tempo = true;

    *frames = *frames + 1;

    if(*frames >= 60)
    {
        *frames = 0;
        *segundos_frames = *segundos_frames + 1;
    }
    if (*segundos_frames >= 15)
    {
        *frames = 0;
        *segundos_frames = 0;
        tempo = false;
    }

    return tempo;
}

void inimigos (INIMIGOS enemy[], int *frame_enemy, int *segundos_frame_enemy, int *numero_inimigos, TIJOLOS tij[][COLUNAS])
{
    srand(time(NULL));
    int index_inimigo = *numero_inimigos;

    if(spawn_inimigos(frame_enemy, segundos_frame_enemy) && *numero_inimigos < MAX_INIMIGOS)
    {
        enemy[index_inimigo].posicao = (Vector2) {260, 250};
        enemy[index_inimigo].posicao_backup = (Vector2) {120 , 120};
        enemy[index_inimigo].size = (Vector2) {30 , 30};
        enemy[index_inimigo].velocidade = 3;
        enemy[index_inimigo].enemy_rec.x = (float)SCREEN_WIDTH / 2;
        enemy[index_inimigo].enemy_rec.y = (float)SCREEN_HEIGHT / 2;
        enemy[index_inimigo].enemy_rec.width = 30;
        enemy[index_inimigo].enemy_rec.height = 30;

        enemy[index_inimigo].right = false;
        enemy[index_inimigo].left = false;
        enemy[index_inimigo].up = false;
        enemy[index_inimigo].down = false;

        enemy[index_inimigo].ativo = true;
        *numero_inimigos = *numero_inimigos + 1;

    }
    for(int i = 0; i < MAX_INIMIGOS; i++)
    {
        movimento_inimigo(enemy, i, tij);

        int muda_direcao = GetRandomValue(0, 0);

        if(muda_direcao == 0 && enemy[i].ativo)
        {
            direcao_inimigo(enemy, i);
        }
    }
}

void direcao_inimigo(INIMIGOS enemy[], int i)
{
    srand(time(NULL));
    int direcao = GetRandomValue(0, 3);

    if(direcao == 0)
    {
       enemy[i].right = true;
       enemy[i].left = false;
       enemy[i].up = false;
       enemy[i].down = false;
    }
    if(direcao == 1)
    {
        enemy[i].right = false;
        enemy[i].left = true;
        enemy[i].up = false;
        enemy[i].down = false;

    }
    if(direcao == 2)
    {
        enemy[i].right = false;
        enemy[i].left = false;
        enemy[i].up = true;
        enemy[i].down = false;

    }
    if(direcao == 3)
    {
        enemy[i].right = false;
        enemy[i].left = false;
        enemy[i].up = false;
        enemy[i].down = true;

    }
}

void movimento_inimigo(INIMIGOS enemy[], int i, TIJOLOS tij[][COLUNAS])
{
    if(enemy[i].right && (enemy[i].posicao.x + enemy[i].size.x) < SCREEN_WIDTH)
    {
       enemy[i].posicao_backup.x = enemy[i].posicao.x;
       enemy[i].posicao_backup.y = enemy[i].posicao.y;

       enemy[i].posicao.x = enemy[i].posicao.x + 3;

       enemy[i].enemy_rec.x = enemy[i].posicao.x;
       enemy[i].enemy_rec.y = enemy[i].posicao.y;

       enemy[i].rotation = 90;

       if(verifica_colisao_inimigo(enemy, tij))
       {
           enemy[i].posicao.x = enemy[i].posicao_backup.x;
           enemy[i].posicao.y = enemy[i].posicao_backup.y;
       }
    }
    else if(enemy[i].left && enemy[i].posicao.x > 0)
    {
        enemy[i].posicao_backup.x = enemy[i].posicao.x;
        enemy[i].posicao_backup.y = enemy[i].posicao.y;

        enemy[i].posicao.x = enemy[i].posicao.x - 3;

        enemy[i].enemy_rec.x = enemy[i].posicao.x;
        enemy[i].enemy_rec.y = enemy[i].posicao.y;

        enemy[i].rotation = 270;

        if(verifica_colisao_inimigo(enemy, tij))
       {
           enemy[i].posicao.x = enemy[i].posicao_backup.x;
           enemy[i].posicao.y = enemy[i].posicao_backup.y;
       }

    }
    else if(enemy[i].up && enemy[i].posicao.y > 60)
    {
        enemy[i].posicao_backup.x = enemy[i].posicao.x;
        enemy[i].posicao_backup.y = enemy[i].posicao.y;

        enemy[i].posicao.y = enemy[i].posicao.y - 3;

        enemy[i].enemy_rec.x = enemy[i].posicao.x;
        enemy[i].enemy_rec.y = enemy[i].posicao.y;

        enemy[i].rotation = 0;

        if(verifica_colisao_inimigo(enemy, tij))
       {
           enemy[i].posicao.x = enemy[i].posicao_backup.x;
           enemy[i].posicao.y = enemy[i].posicao_backup.y;
       }

    }
    else if(enemy[i].down && enemy[i].posicao.y + enemy[i].size.y < SCREEN_HEIGHT)
    {
        enemy[i].posicao_backup.x = enemy[i].posicao.x;
        enemy[i].posicao_backup.y = enemy[i].posicao.y;

        enemy[i].posicao.y = enemy[i].posicao.y + 3;

        enemy[i].enemy_rec.x = enemy[i].posicao.x;
        enemy[i].enemy_rec.y = enemy[i].posicao.y;

        enemy[i].rotation = 180;

        if(verifica_colisao_inimigo(enemy, tij))
       {
           enemy[i].posicao.x = enemy[i].posicao_backup.x;
           enemy[i].posicao.y = enemy[i].posicao_backup.y;
       }

    }
}
bool verifica_colisao_inimigo(INIMIGOS enemy[], TIJOLOS tij[][COLUNAS])
{
    bool colisao = false;

    for(int k = 0; k < MAX_INIMIGOS; k++)
    {
    for(int i = 0; i <  LINHAS; i++)
    {
        for(int j = 0; j < COLUNAS; j++)
        {
            if(CheckCollisionRecs(enemy[k].enemy_rec, tij[i][j].tij_rec) && tij[i][j].ativo)
            {
                colisao = true;
            }
        }
    }
    }

    return colisao;
}

bool spawn_inimigos(int *frame_enemy, int *segundos_frame_enemy)
{
    bool spawn_ok = false;

    *frame_enemy = *frame_enemy + 1;
    if(*frame_enemy >= 60)
    {
        *frame_enemy = 0;
        *segundos_frame_enemy = *segundos_frame_enemy + 1;
    }
    if(*segundos_frame_enemy >= 5)
    {
        *frame_enemy = 0;
        *segundos_frame_enemy = 0;
        spawn_ok = true;
    }

    return spawn_ok;
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

