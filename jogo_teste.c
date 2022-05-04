#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 660
#define MAX_PROJETEIS 1
#define MAX_INIMIGOS 5
#define LINHAS 15
#define LAST_LEVEL 2
#define COLUNAS 40 + 1 //Esse +1 pq quando chega no final tem um comando ou algo parecido pra indicar nova linha, por isso antes ficava uma escada
#define MAX_LETRAS 9

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

    int nivel;
    int nivel1_backup;

    int vidas;
    int score;
    int velocidade;
    float boost_velocidade;
    int rotation;
    char username[MAX_LETRAS + 1];

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
    bool tiro;
    bool perseguicao;

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
    int velocidade;

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
void coloca_tijolo(TIJOLOS[][COLUNAS], JOGADOR*);

// essa funcao calcula e atualiza o movimento do jogador
void movimento_jogador(JOGADOR*, TIJOLOS[][COLUNAS]);

// Essa funcao desenha o jogo
void draw(int*, JOGADOR*, PROJETIL[], TIJOLOS[][COLUNAS], CELULA*, INIMIGOS[], PROJETIL[],
        Texture2D, Texture2D, Texture2D, Texture2D, Texture2D, Texture2D, Texture2D, int*);
// Essa funcao desenha o menu do jogo
void desenha_menu(int*, bool*);

// Essa funcao calcula e atualiza o movimento dos projeteis
void movimento_projetil(JOGADOR*, PROJETIL[], TIJOLOS[][COLUNAS]);

//Essa função coloca as células de energia
void celulasEnergia(CELULA*, JOGADOR*, PROJETIL[], int*, int*, TIJOLOS[][COLUNAS]);

// Verifica colisao da celula com parede na hora do spawn
bool colisao_celula_parede(CELULA*, TIJOLOS[][COLUNAS]);

// Verifica colisao entre projeteis dos inimigos e do player
void colisao_projetil_projetil(PROJETIL[], INIMIGOS[], PROJETIL[]);

// ESSE CONJUNTO DE FUNCOES ESTA VINCULADO AOS INIMIGOS
//essa funcao eh tipo uma interface que vai coordenar todas as funcoes relativas aos inimigos
void inimigos(INIMIGOS[], int*, int*, int*, TIJOLOS[][COLUNAS], PROJETIL[], PROJETIL[], JOGADOR*);
//verifica a nova direcao do inimigo
void direcao_inimigo(INIMIGOS[], int);
// verifica direcao do tiro inimigo
void direcao_tiro_inimigo(INIMIGOS[], int, PROJETIL[]);
// atualiza posicao tiro inimigo
void movimento_projetil_inimigo(INIMIGOS[], int, PROJETIL[]);
//atualiza a posicao do inimigo
void movimento_inimigo(INIMIGOS[], int, TIJOLOS[][COLUNAS], PROJETIL[], JOGADOR*);
// verifica colisao tiro inimigo com tijolos
void verifica_colisao_tij_projetil_inimigo(INIMIGOS[], PROJETIL[], TIJOLOS[][COLUNAS], int);
// verifica modo patrulha
void modo_perseguicao(JOGADOR*, INIMIGOS[]);
// verifica colisao entre player e inimigos
void colisao_player_enemy(JOGADOR*, INIMIGOS[]);
// verifica colisao player com projetil inimigo
void colisao_player_projetil_inimigo(JOGADOR*, INIMIGOS[], PROJETIL[]);
// verifica o tempo de spawn dos inimigos
bool spawn_inimigos(int*, int*);
// Essa funcao verifica colisao entre inimigos e tijolos
bool verifica_colisao_inimigo_tij(INIMIGOS[], TIJOLOS[][COLUNAS], int);
// Essa funcao verifica colisao entre inimigos e projeteis
bool verifica_colisao_inimigo_projetil(INIMIGOS[], PROJETIL[], int, JOGADOR*);
// Essa funcao verifica colisao entre inimigos e inimigos
bool verifica_colisao_inimigo_inimigo(INIMIGOS[], int);
//Essa funcao possibilita a insercao de um nome de usuario ao player
void coloca_username(JOGADOR*);
//Essa função salva o jogo
void save_game(JOGADOR*, INIMIGOS[], TIJOLOS[][COLUNAS], CELULA*);



// Essa funçao verifica colisao entre o player e tijolos
bool verifica_colisao_player(JOGADOR*, TIJOLOS[][COLUNAS]);

// Essa funcao verifica colisao entra projetil e tijolos
bool verifica_colisao_projetil(PROJETIL[], TIJOLOS[][COLUNAS]);

// essa funcao eh o contador do bonus da celula de energia (15 seg)
bool timer(int* , int*);

// verifica se os criterios para passar de nivel estao ok(10 segundos de fase e matar todos inimigos)
void passa_nivel(INIMIGOS[], int*, char[], TIJOLOS[][COLUNAS], int*, JOGADOR*, int*);


int main(void)
{

    // Inicializar o array que vai armazenar a string do mapa do jogo
    char tijolosText[600 + 15]; // Esses +15 por causa daquele comando de nova linha


    // Variaveis utilizadas
    TIJOLOS tij[LINHAS][COLUNAS];
    JOGADOR player;
    PROJETIL bullets[MAX_PROJETEIS];
    PROJETIL enemy_bullets[MAX_INIMIGOS];
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

    player.score = 0;
    player.nivel = 0;
    player.vidas = 3;
    player.username[MAX_LETRAS + 1] = '\0';

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
        enemy[i].tiro = false;
        enemy[i].perseguicao = false;

    }

    //Inicialização das celulas
    energia.size = (Vector2) {25, 40};
    energia.alive = false;
    energia.boost_ativo = false;

    // Inicializacao dos projeteis player
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

    // Inicializacao projeteis inimigos
    for(int i = 0; i < MAX_INIMIGOS; i++)
    {
        enemy_bullets[i].posicao = (Vector2) {(float)SCREEN_WIDTH / 2 , (float)SCREEN_HEIGHT / 2};
        enemy_bullets[i].size = (Vector2) {15, 15};
        enemy_bullets[i].ativo = false;

        enemy_bullets[i].right = false;
        enemy_bullets[i].left = false;
        enemy_bullets[i].up = false;
        enemy_bullets[i].down = false;

        enemy_bullets[i].velocidade = 5;
        enemy_bullets[i].boost_velocidade = 0;

    }

    // Inicializacao dos blocos do mapa
    strcpy(tijolosText, LoadFileText("mapas/mapa1.txt"));

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
        Texture2D tanqueEnemy = LoadTexture("public/tanque_inimigo.png");
        Texture2D tanqueEnemyProjetil = LoadTexture("public/PNG/Effects/Sprites/Sprite_Fire_Shots_Flame_007.png");
        Texture2D vidas = LoadTexture("public/shield.png");

    int frame = 0;
    int segundos_frame = 0;

    int frame_enemy = 0;
    int segundos_frame_enemy = 0;

    int tempo_nivel = 0;
    int nivel = 1;

    int TELA = 0;
    int tempo_menu = 0;

    FILE *arqSave;

    bool sair_loop = false;

    coloca_tijolo(tij, &player);



    // Loop do jogo
    while(!WindowShouldClose() && !sair_loop)
    {
        if(TELA < 5)
        {
            desenha_menu(&TELA, &sair_loop);
        }
        else if(TELA = 6)
        {
            coloca_username(&player);
            if(IsKeyDown(KEY_ENTER)){
            TELA = 5;
            }
        }
        else
        {   
            save_game(&player, enemy, tij, &energia);
            movimento_jogador(&player, tij);
            movimento_projetil(&player, bullets, tij);
            inimigos(enemy, &frame_enemy, &segundos_frame_enemy, &numero_inimigos, tij, bullets, enemy_bullets, &player);
            modo_perseguicao(&player, enemy);

            draw(&nivel, &player, bullets, tij, &energia, enemy, enemy_bullets,
                tijolo_textura, celulasText, tanquePlayer, projetilText, tanqueEnemy, tanqueEnemyProjetil, vidas, &TELA);

            celulasEnergia(&energia, &player, bullets, &frame, &segundos_frame, tij);
            colisao_player_enemy(&player, enemy);
            colisao_player_projetil_inimigo(&player, enemy, enemy_bullets);
            colisao_projetil_projetil(bullets, enemy, enemy_bullets);
            passa_nivel(enemy, &tempo_nivel, tijolosText, tij, &nivel, &player, &numero_inimigos);
        }
    }

    CloseWindow();
    return 0;

}
// Essa funcao desenha o menu (cada caso do switch eh uma tela diferente)
void desenha_menu(int *TELA, bool *sair_loop)
{
    //comandos do menu
    switch(*TELA)
    {
        case 0:
            {
                if(IsKeyPressed(KEY_DOWN))
                {
                    *TELA = *TELA + 1;
                }
                if(IsKeyPressed(KEY_UP))
                {
                    *TELA = 4;
                }
                if(IsKeyPressed(KEY_ENTER))
                {
                    *TELA = 6;
                }
            }break;
        case 1:
            {
                if(IsKeyPressed(KEY_DOWN))
                {
                    *TELA = *TELA + 1;
                }
                if(IsKeyPressed(KEY_UP))
                {
                    *TELA = *TELA - 1;
                }
            }break;
        case 2:
            {
                if(IsKeyPressed(KEY_DOWN))
                {
                    *TELA = *TELA + 1;
                }
                if(IsKeyPressed(KEY_UP))
                {
                    *TELA = *TELA - 1;
                }
            }break;
        case 3:
            {
                if(IsKeyPressed(KEY_DOWN))
                {
                    *TELA = *TELA + 1;
                }
                if(IsKeyPressed(KEY_UP))
                {
                    *TELA = *TELA - 1;
                }
            }break;
        case 4:
            {
                if(IsKeyPressed(KEY_DOWN))
                {
                    *TELA = 0;
                }
                if(IsKeyPressed(KEY_UP))
                {
                    *TELA = *TELA - 1;
                }
                if(IsKeyPressed(KEY_ENTER))
                {
                    *sair_loop = true;
                }
            }break;

    }

    //desenha o menu
    BeginDrawing();
        switch(*TELA)
        {
            case 0:
            {


            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
            DrawText("BATTLEINF", ((SCREEN_WIDTH - MeasureText("BATTLEINF", 60)) / 2), 100, 60, DARKGREEN);
            DrawText("NOVO JOGO", ((SCREEN_WIDTH - MeasureText("NOVO JOGO", 20)) / 2), 220, 20, RED);
            DrawText("CONTINUAR", ((SCREEN_WIDTH - MeasureText("CONTINUAR", 20)) / 2), 250, 20, WHITE);
            DrawText("CARREGAR", ((SCREEN_WIDTH - MeasureText("CARREGAR", 20)) / 2), 280, 20, WHITE);
            DrawText("HIGHSCORES", ((SCREEN_WIDTH - MeasureText("HIGHSCORES", 20)) / 2), 310, 20, WHITE);
            DrawText("SAIR", ((SCREEN_WIDTH - MeasureText("SAIR", 20)) / 2), 340, 20, WHITE);
            }break;

            case 1:
            {


            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
            DrawText("BATTLEINF", ((SCREEN_WIDTH - MeasureText("BATTLEINF", 60)) / 2), 100, 60, DARKGREEN);
            DrawText("NOVO JOGO", ((SCREEN_WIDTH - MeasureText("NOVO JOGO", 20)) / 2), 220, 20, WHITE);
            DrawText("CONTINUAR", ((SCREEN_WIDTH - MeasureText("CONTINUAR", 20)) / 2), 250, 20, RED);
            DrawText("CARREGAR", ((SCREEN_WIDTH - MeasureText("CARREGAR", 20)) / 2), 280, 20, WHITE);
            DrawText("HIGHSCORES", ((SCREEN_WIDTH - MeasureText("HIGHSCORES", 20)) / 2), 310, 20, WHITE);
            DrawText("SAIR", ((SCREEN_WIDTH - MeasureText("SAIR", 20)) / 2), 340, 20, WHITE);
            }break;
            case 2:
            {


            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
            DrawText("BATTLEINF", ((SCREEN_WIDTH - MeasureText("BATTLEINF", 60)) / 2), 100, 60, DARKGREEN);
            DrawText("NOVO JOGO", ((SCREEN_WIDTH - MeasureText("NOVO JOGO", 20)) / 2), 220, 20, WHITE);
            DrawText("CONTINUAR", ((SCREEN_WIDTH - MeasureText("CONTINUAR", 20)) / 2), 250, 20, WHITE);
            DrawText("CARREGAR", ((SCREEN_WIDTH - MeasureText("CARREGAR", 20)) / 2), 280, 20, RED);
            DrawText("HIGHSCORES", ((SCREEN_WIDTH - MeasureText("HIGHSCORES", 20)) / 2), 310, 20, WHITE);
            DrawText("SAIR", ((SCREEN_WIDTH - MeasureText("SAIR", 20)) / 2), 340, 20, WHITE);
            }break;
            case 3:
            {


            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
            DrawText("BATTLEINF", ((SCREEN_WIDTH - MeasureText("BATTLEINF", 60)) / 2), 100, 60, DARKGREEN);
            DrawText("NOVO JOGO", ((SCREEN_WIDTH - MeasureText("NOVO JOGO", 20)) / 2), 220, 20, WHITE);
            DrawText("CONTINUAR", ((SCREEN_WIDTH - MeasureText("CONTINUAR", 20)) / 2), 250, 20, WHITE);
            DrawText("CARREGAR", ((SCREEN_WIDTH - MeasureText("CARREGAR", 20)) / 2), 280, 20, WHITE);
            DrawText("HIGHSCORES", ((SCREEN_WIDTH - MeasureText("HIGHSCORES", 20)) / 2), 310, 20, RED);
            DrawText("SAIR", ((SCREEN_WIDTH - MeasureText("SAIR", 20)) / 2), 340, 20, WHITE);
            }break;
            case 4:
            {


            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
            DrawText("BATTLEINF", ((SCREEN_WIDTH - MeasureText("BATTLEINF", 60)) / 2), 100, 60, DARKGREEN);
            DrawText("NOVO JOGO", ((SCREEN_WIDTH - MeasureText("NOVO JOGO", 20)) / 2), 220, 20, WHITE);
            DrawText("CONTINUAR", ((SCREEN_WIDTH - MeasureText("CONTINUAR", 20)) / 2), 250, 20, WHITE);
            DrawText("CARREGAR", ((SCREEN_WIDTH - MeasureText("CARREGAR", 20)) / 2), 280, 20, WHITE);
            DrawText("HIGHSCORES", ((SCREEN_WIDTH - MeasureText("HIGHSCORES", 20)) / 2), 310, 20, WHITE);
            DrawText("SAIR", ((SCREEN_WIDTH - MeasureText("SAIR", 20)) / 2), 340, 20, RED);
            }break;
        }


    EndDrawing();
}

void celulasEnergia(CELULA *energia, JOGADOR *player, PROJETIL bullets[], int *frames, int *segundos_frames, TIJOLOS tij[][COLUNAS]){

    srand(time(NULL));


    int celula_spawn = GetRandomValue(0, 16);
    //Precisa achar uma maneira de não trocar com tanta frequencia
    if(celula_spawn == 0 && energia->alive == false)
    {
           do
           {
                energia->posicao.x = GetRandomValue(0, 1000);
                energia->posicao.y = GetRandomValue(60, 600);

                energia->energia_rec.x = energia->posicao.x;
                energia->energia_rec.y = energia->posicao.y;
                energia->energia_rec.width = 25;
                energia->energia_rec.height = 40;

                energia->alive = true;
           }while(colisao_celula_parede(energia, tij));// verifica colisao com parede
    }

    // verifica se o player pegou a celula
    if(CheckCollisionRecs(energia->energia_rec, player->player_rec))
    {
        int tempo = 0;
        int frames = 0;
        energia->alive = false;
        energia->boost_ativo = true;

    }

    // verifica o tempo do boost
    if(energia->boost_ativo && timer(frames, segundos_frames))
    {
        player->boost_velocidade = 1.5;
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

// colisao com celula e parede
bool colisao_celula_parede(CELULA *energia, TIJOLOS tij[][COLUNAS])
{
    bool colisao = false;

    for(int i = 0; i < LINHAS; i++)
    {
        for(int j = 0; j < COLUNAS; j++)
        {
            if(CheckCollisionRecs(energia->energia_rec, tij[i][j].tij_rec) && tij[i][j].ativo)
            {
                bool colisao = true;
            }
        }
    }
    return colisao;
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

        //colisao com parede
        if(verifica_colisao_player(player, tij))
        {
            player->posicao.x = player->posicao_backup.x;
            player->posicao.y = player->posicao_backup.y;

            player->player_rec.x = player->posicao_backup.x;
            player->player_rec.y = player->posicao_backup.y;
        }
    }
    else if (IsKeyDown(KEY_LEFT) && (player->posicao.x > 0))
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

        // colisao com parede
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

        // colisao com parede
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

        //colisao com parede
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
void draw(int *nivel, JOGADOR *player, PROJETIL bullets[], TIJOLOS tij[][COLUNAS], CELULA *energia, INIMIGOS enemy[], PROJETIL enemy_bullets[],
          Texture2D tijolo_textura, Texture2D celulasText, Texture2D tanquePlayer,
          Texture2D projetilText, Texture2D tanqueEnemy, Texture2D tanqueEnemyProjetil, Texture2D vidas, int *TELA)
{
    // Comeca desenho
    BeginDrawing();
        ClearBackground(BLACK);

    
        // desenha o cabecalho do jogo
        DrawRectangle(0, 0, 1000, 60, GRAY);
        if(*nivel == 1)
        {
            DrawText("Fase 1", ((SCREEN_WIDTH - MeasureText("Fase 1", 50)) / 2), 10, 50, BLACK);
        }
        if(*nivel == 2)
        {
            DrawText("Fase 2", ((SCREEN_WIDTH - MeasureText("Fase 2", 50)) / 2), 10, 50, BLACK);
        }

        // desenha vidas
        if(player->vidas <= 0)
        {
            DrawText("GAME OVER", ((SCREEN_WIDTH - MeasureText("GAME OVER", 70))/2), 330, 70, RED);
            DrawText("Pressione ENTER para voltar ao menu", ((SCREEN_WIDTH - MeasureText("Pressione ENTER para voltar ao menu", 20))/2), 405, 20, WHITE);
            player->velocidade = 0.0f;
            bullets->ativo = false;
            if(IsKeyDown(KEY_ENTER)){
                player->nivel1_backup = player->nivel;
                //Retornar para o menu
            }
        }
        if(player->vidas >= 1)
        {
            DrawTexturePro(vidas, (Rectangle) {0, 0, 700, 700}, (Rectangle) {0, 0, 50, 50}, (Vector2){0, 0}, 0, RAYWHITE);
        }
        if(player->vidas >= 2)
        {
            DrawTexturePro(vidas, (Rectangle) {0, 0, 700, 700}, (Rectangle) {50, 0, 50, 50}, (Vector2){0, 0}, 0, RAYWHITE);
        }
        if(player->vidas >= 3)
        {
            DrawTexturePro(vidas, (Rectangle) {0, 0, 700, 700}, (Rectangle) {100, 0, 50, 50}, (Vector2){0, 0}, 0, RAYWHITE);
        }



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
                //DrawRectangleV(enemy[i].posicao, enemy[i].size, MAROON);
                DrawTexturePro(tanqueEnemy,(Rectangle){0, 0, 510, 510}, enemy[i].enemy_rec, (Vector2) {0 , 30}, enemy[i].rotation, RAYWHITE);
            }
            if(enemy[i].ativo && enemy[i].left)
            {
                //DrawRectangleV(enemy[i].posicao, enemy[i].size, MAROON);
                DrawTexturePro(tanqueEnemy,(Rectangle){0, 0, 510, 510}, enemy[i].enemy_rec, (Vector2) {30 , 0}, enemy[i].rotation, RAYWHITE);
            }
            if(enemy[i].ativo && enemy[i].up)
            {
                //DrawRectangleV(enemy[i].posicao, enemy[i].size, MAROON);
                DrawTexturePro(tanqueEnemy,(Rectangle){0, 0, 510, 510}, enemy[i].enemy_rec, (Vector2) {0 , 0}, enemy[i].rotation, RAYWHITE);
            }
            if(enemy[i].ativo && enemy[i].down)
            {
                //DrawRectangleV(enemy[i].posicao, enemy[i].size, MAROON);
                DrawTexturePro(tanqueEnemy,(Rectangle){0, 0, 510, 510}, enemy[i].enemy_rec, (Vector2) {30 , 30}, enemy[i].rotation, RAYWHITE);
            }
        }

        // Esse loop desenha os projeteis inimigos
        for(int i = 0; i < MAX_INIMIGOS; i++)
        {
            if(enemy_bullets[i].ativo)
            {
                //DrawRectangleV(bullets[i].posicao, bullets[i].size, GREEN);
                if(enemy_bullets[i].right)
                {
                    DrawTexturePro(tanqueEnemyProjetil,(Rectangle){0, 0, 256, 256}, enemy_bullets[i].bullet_rec, (Vector2) {20 , 0}, 270, RAYWHITE);
                }
                if(enemy_bullets[i].left)
                {
                    DrawTexturePro(tanqueEnemyProjetil,(Rectangle){0, 0, 256, 256}, enemy_bullets[i].bullet_rec, (Vector2) {-10 , 10}, 90, RAYWHITE);
                }
                if(enemy_bullets[i].up)
                {
                    DrawTexturePro(tanqueEnemyProjetil,(Rectangle){0, 0, 256, 256}, enemy_bullets[i].bullet_rec, (Vector2) {20 , 30}, 180, RAYWHITE);
                }
                if(enemy_bullets[i].down)
                {
                    DrawTexturePro(tanqueEnemyProjetil,(Rectangle){0, 0, 256, 256}, enemy_bullets[i].bullet_rec, (Vector2) {-10 , 0}, 0, RAYWHITE);
                }
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

// Verifica se o elemento do arquivo mapa eh um tijolo a ser colocado ou o player
void coloca_tijolo(TIJOLOS tij[][COLUNAS], JOGADOR *player)
{
    for(int i = 0; i < LINHAS; i++)
    {
        for(int j = 0; j < COLUNAS; j++)
        {
             if(tij[i][j].elemento == '#' && !tij[i][j].atingido_projetil)
             {
                 tij[i][j].ativo = true;
             }

             if(tij[i][j].elemento == 'T')
             {
                 player->posicao_backup.x = tij[i][j].posicao.x;
                 player->posicao_backup.y = tij[i][j].posicao.y;

                 player->posicao.x = tij[i][j].posicao.x;
                 player->posicao.y = tij[i][j].posicao.y;
                 player->right = true;
                 player->left = false;
                 player->up = false;
                 player->down = false;

                 player->player_rec.x = player->posicao.x;
                 player->player_rec.y = player->posicao.y;

                 player->rotation = 90;

             }
        }

    }
}

// verifica colisao entre player e parede
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

// colisao entre projeteis e paredes
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

// contador de tempo para boost da celula de energia
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

// funcao principal de inimigos
// os inimigos foram salvos em um array e, para acessar um determinado inimigo desse array
// normalmente sera passado como argumento nas funcoes o array dos inimigos e "int i", a qual
// ira possibilitar acessar esse inimigo em especifico
// cada inimigo tambem tera seu proprio projetil no array de projeteis inimigos, o qual tera a mesma posicao do inimigo e podera ser acessado com i tbm
void inimigos (INIMIGOS enemy[], int *frame_enemy, int *segundos_frame_enemy, int *numero_inimigos, TIJOLOS tij[][COLUNAS], PROJETIL bullets[], PROJETIL enemy_bullets[], JOGADOR *player)
{
    srand(time(NULL));
    int index_inimigo = *numero_inimigos;

    // relativo ao spawn aleatorio de inimigos
    if(spawn_inimigos(frame_enemy, segundos_frame_enemy) && *numero_inimigos < MAX_INIMIGOS)
    {
        do
        {
            enemy[index_inimigo].posicao.x = GetRandomValue(0, 999);
            enemy[index_inimigo].posicao.y= GetRandomValue(60, 570);

            enemy[index_inimigo].posicao_backup.x = enemy[index_inimigo].posicao.x;
            enemy[index_inimigo].posicao_backup.y = enemy[index_inimigo].posicao.y;

            enemy[index_inimigo].enemy_rec.x = enemy[index_inimigo].posicao.x;
            enemy[index_inimigo].enemy_rec.y = enemy[index_inimigo].posicao.y;
            enemy[index_inimigo].enemy_rec.width = 30;
            enemy[index_inimigo].enemy_rec.height = 30;

        }while(verifica_colisao_inimigo_tij(enemy, tij, index_inimigo) ||
               verifica_colisao_inimigo_inimigo(enemy, index_inimigo) ||
               (CheckCollisionRecs(enemy[index_inimigo].enemy_rec, player->player_rec) && enemy[index_inimigo].ativo));// evitar colisoes com outras
                                                                                                                        // coisas do jogo


        enemy[index_inimigo].size = (Vector2) {30 , 30};
        enemy[index_inimigo].velocidade = 1;

        enemy[index_inimigo].right = false;
        enemy[index_inimigo].left = false;
        enemy[index_inimigo].up = false;
        enemy[index_inimigo].down = false;

        enemy[index_inimigo].ativo = true;
        *numero_inimigos = *numero_inimigos + 1;

    }
    for(int i = 0; i < MAX_INIMIGOS; i++)
    {
        // responsavel por atualizar posicao do inimigo
        movimento_inimigo(enemy, i, tij, bullets, player);

        // muda direcao do inimigo aleatoriamente
        int muda_direcao = GetRandomValue(0, 1);

        // inimigo atira aleatoriamente
        int atira = GetRandomValue(0, 16);

        // para mudar direcao do inimigo
        if(muda_direcao == 0 && enemy[i].ativo && !enemy[i].perseguicao)
        {
            direcao_inimigo(enemy, i);
        }

        // modo perseguicao do inimigo(atira com mais frequencia tbm e se mantem na mesma linha x ou y do player)
        if(enemy[i].perseguicao)
        {
            atira = GetRandomValue(0 , 5);
        }

        // tiros iniimgos
        if(atira == 0 && enemy[i].ativo && !enemy[i].tiro)
        {
            direcao_tiro_inimigo(enemy, i, enemy_bullets);
        }

        // movimento dos tiros
        if(enemy[i].tiro)
        {
            movimento_projetil_inimigo(enemy, i, enemy_bullets);
            verifica_colisao_tij_projetil_inimigo(enemy, enemy_bullets, tij, i);
        }
    }
}

// direcao dos inimigos (right, left, up, down)
void direcao_inimigo(INIMIGOS enemy[], int i)
{
    // escolhe uma direcao aleatoriamente
    srand(time(NULL) + i);
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

//modo perseguicao inimigo
//quando entra na mesma lina x ou y do jogador, persegue o player
void modo_perseguicao(JOGADOR *player, INIMIGOS enemy[])
{
    for(int i = 0; i < MAX_INIMIGOS; i++)
    {
        if(enemy[i].ativo)
        {
            if(player->posicao.y == enemy[i].posicao.y && player->posicao.x > enemy[i].posicao.x)
            {
                enemy[i].perseguicao = true;

                enemy[i].right = true;
                enemy[i].left = false;
                enemy[i].up = false;
                enemy[i].down = false;
            }
            else if(player->posicao.y == enemy[i].posicao.y && player->posicao.x < enemy[i].posicao.x)
            {
                enemy[i].perseguicao = true;

                enemy[i].right = false;
                enemy[i].left = true;
                enemy[i].up = false;
                enemy[i].down = false;
            }
            else if(player->posicao.x == enemy[i].posicao.x && player->posicao.y < enemy[i].posicao.y)
            {
                enemy[i].perseguicao = true;

                enemy[i].right = false;
                enemy[i].left = false;
                enemy[i].up = true;
                enemy[i].down = false;
            }
            else if(player->posicao.x == enemy[i].posicao.x && player->posicao.y > enemy[i].posicao.y)
            {
                enemy[i].perseguicao = true;

                enemy[i].right = false;
                enemy[i].left = false;
                enemy[i].up = false;
                enemy[i].down = true;
            }
            else
            {
                enemy[i].perseguicao = false;
            }

        }
    }
}

// atualiza posicao dos inimigos
void movimento_inimigo(INIMIGOS enemy[], int i, TIJOLOS tij[][COLUNAS], PROJETIL bullets[], JOGADOR *player)
{
    if(enemy[i].right)
    {
       enemy[i].posicao_backup.x = enemy[i].posicao.x;
       enemy[i].posicao_backup.y = enemy[i].posicao.y;

       enemy[i].posicao.x = enemy[i].posicao.x + enemy[i].velocidade;

       enemy[i].enemy_rec.x = enemy[i].posicao.x;
       enemy[i].enemy_rec.y = enemy[i].posicao.y;

       enemy[i].rotation = 90;

       // colisao com paredes inimigo
       if(verifica_colisao_inimigo_tij(enemy, tij, i))
        {
            enemy[i].posicao.x = enemy[i].posicao_backup.x;
            //enemy[i].posicao.y = enemy[i].posicao_backup.y;
            enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
            //enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;
        }

        // colisao entre inimigos
        if(verifica_colisao_inimigo_inimigo(enemy, i))
        {
            enemy[i].posicao.x = enemy[i].posicao_backup.x;
            //enemy[i].posicao.y = enemy[i].posicao_backup.y;
            enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
            //enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;

        }

        // colisao borda da tela
        if(enemy[i].posicao.x + enemy[i].size.x >= SCREEN_WIDTH)
        {
            enemy[i].posicao.x = enemy[i].posicao_backup.x;
            enemy[i].posicao.y = enemy[i].posicao_backup.y;
            enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
            //enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;
        }

        // colisao inimigo com projetil do player
        verifica_colisao_inimigo_projetil(enemy, bullets, i, player);
    }
    else if(enemy[i].left)
    {
        enemy[i].posicao_backup.x = enemy[i].posicao.x;
        enemy[i].posicao_backup.y = enemy[i].posicao.y;

        enemy[i].posicao.x = enemy[i].posicao.x - enemy[i].velocidade;

        enemy[i].enemy_rec.x = enemy[i].posicao.x;
        enemy[i].enemy_rec.y = enemy[i].posicao.y;

        enemy[i].rotation = 270;

        if(verifica_colisao_inimigo_tij(enemy, tij, i))
        {
            enemy[i].posicao.x = enemy[i].posicao_backup.x;
            //enemy[i].posicao.y = enemy[i].posicao_backup.y;
            enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
            //enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;
        }

        if(verifica_colisao_inimigo_inimigo(enemy, i))
        {
            enemy[i].posicao.x = enemy[i].posicao_backup.x;
            //enemy[i].posicao.y = enemy[i].posicao_backup.y;
            enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
            //enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;

        }

        if(enemy[i].posicao.x <= 0)
        {
            enemy[i].posicao.x = enemy[i].posicao_backup.x;
            //enemy[i].posicao.y = enemy[i].posicao_backup.y;
            enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
            //enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;

        }

        verifica_colisao_inimigo_projetil(enemy, bullets, i, player);

    }
    else if(enemy[i].up)
    {
        enemy[i].posicao_backup.x = enemy[i].posicao.x;
        enemy[i].posicao_backup.y = enemy[i].posicao.y;

        enemy[i].posicao.y = enemy[i].posicao.y - enemy[i].velocidade;

        enemy[i].enemy_rec.x = enemy[i].posicao.x;
        enemy[i].enemy_rec.y = enemy[i].posicao.y;

        enemy[i].rotation = 0;

        if(verifica_colisao_inimigo_tij(enemy, tij, i))
        {
            //enemy[i].posicao.x = enemy[i].posicao_backup.x;
            enemy[i].posicao.y = enemy[i].posicao_backup.y;
            //enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
            enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;

        }

        if(verifica_colisao_inimigo_inimigo(enemy, i))
        {
            //enemy[i].posicao.x = enemy[i].posicao_backup.x;
            enemy[i].posicao.y = enemy[i].posicao_backup.y;
            //enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
            enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;

        }

        if(enemy[i].posicao.y <= 60)
        {
            //enemy[i].posicao.x = enemy[i].posicao_backup.x;
            enemy[i].posicao.y = enemy[i].posicao_backup.y;
            //enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
            enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;

        }

        verifica_colisao_inimigo_projetil(enemy, bullets, i, player);

    }
    else if(enemy[i].down)
    {
        enemy[i].posicao_backup.x = enemy[i].posicao.x;
        enemy[i].posicao_backup.y = enemy[i].posicao.y;

        enemy[i].posicao.y = enemy[i].posicao.y + enemy[i].velocidade;

        enemy[i].enemy_rec.x = enemy[i].posicao.x;
        enemy[i].enemy_rec.y = enemy[i].posicao.y;

        enemy[i].rotation = 180;

       if(verifica_colisao_inimigo_tij(enemy, tij, i))
        {
            //enemy[i].posicao.x = enemy[i].posicao_backup.x;
            enemy[i].posicao.y = enemy[i].posicao_backup.y;
            //enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
            enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;
        }

        if(verifica_colisao_inimigo_inimigo(enemy, i))
        {
            enemy[i].posicao.x = enemy[i].posicao_backup.x;
            enemy[i].posicao.y = enemy[i].posicao_backup.y;
            enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
            enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;

        }

        if(enemy[i].posicao.y + enemy[i].size.y >= SCREEN_HEIGHT)
        {
            //enemy[i].posicao.x = enemy[i].posicao_backup.x;
            enemy[i].posicao.y = enemy[i].posicao_backup.y;
            //enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
            enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;

        }

        verifica_colisao_inimigo_projetil(enemy, bullets, i, player);

    }
}

// grava qual direcao o tiro do inimigo vai ir (right, left, up, down)
void direcao_tiro_inimigo(INIMIGOS enemy[], int i, PROJETIL enemy_bullets[])
{
    if(enemy[i].right)
    {
        enemy[i].tiro = true;
        enemy_bullets[i].ativo = true;

        enemy_bullets[i].right = true;
        enemy_bullets[i].left = false;
        enemy_bullets[i].up = false;
        enemy_bullets[i].down = false;

        enemy_bullets[i].posicao.x = enemy[i].posicao.x;
        enemy_bullets[i].posicao.y = enemy[i].posicao.y;
    }
    else if(enemy[i].left)
    {
        enemy[i].tiro = true;
        enemy_bullets[i].ativo = true;

        enemy_bullets[i].right = false;
        enemy_bullets[i].left = true;
        enemy_bullets[i].up = false;
        enemy_bullets[i].down = false;

        enemy_bullets[i].posicao.x = enemy[i].posicao.x;
        enemy_bullets[i].posicao.y = enemy[i].posicao.y;

    }
    else if(enemy[i].up)
    {
        enemy[i].tiro = true;
        enemy_bullets[i].ativo = true;

        enemy_bullets[i].right = false;
        enemy_bullets[i].left = false;
        enemy_bullets[i].up = true;
        enemy_bullets[i].down = false;

        enemy_bullets[i].posicao.x = enemy[i].posicao.x;
        enemy_bullets[i].posicao.y = enemy[i].posicao.y;

    }
    else if(enemy[i].down)
    {
        enemy[i].tiro = true;
        enemy_bullets[i].ativo = true;

        enemy_bullets[i].right = false;
        enemy_bullets[i].left = false;
        enemy_bullets[i].up = false;
        enemy_bullets[i].down = true;

        enemy_bullets[i].posicao.x = enemy[i].posicao.x;
        enemy_bullets[i].posicao.y = enemy[i].posicao.y;

    }
}

// atualiza valores dos projeteis inimigos
void movimento_projetil_inimigo(INIMIGOS enemy[], int i, PROJETIL enemy_bullets[])
{
    if(enemy_bullets[i].right)
    {
        enemy_bullets[i].posicao.x = enemy_bullets[i].posicao.x + enemy_bullets[i].velocidade;

        enemy_bullets[i].bullet_rec.x = enemy_bullets[i].posicao.x;
        enemy_bullets[i].bullet_rec.y = enemy_bullets[i].posicao.y;

        enemy_bullets[i].bullet_rec.width = enemy_bullets[i].size.x;
        enemy_bullets[i].bullet_rec.height = enemy_bullets[i].size.y;

    }
    else if(enemy_bullets[i].left)
    {
        enemy_bullets[i].posicao.x = enemy_bullets[i].posicao.x - enemy_bullets[i].velocidade;

        enemy_bullets[i].bullet_rec.x = enemy_bullets[i].posicao.x;
        enemy_bullets[i].bullet_rec.y = enemy_bullets[i].posicao.y;

        enemy_bullets[i].bullet_rec.width = enemy_bullets[i].size.x;
        enemy_bullets[i].bullet_rec.height = enemy_bullets[i].size.y;

    }
    else if(enemy_bullets[i].up)
    {
        enemy_bullets[i].posicao.y = enemy_bullets[i].posicao.y - enemy_bullets[i].velocidade;

        enemy_bullets[i].bullet_rec.x = enemy_bullets[i].posicao.x;
        enemy_bullets[i].bullet_rec.y = enemy_bullets[i].posicao.y;

        enemy_bullets[i].bullet_rec.width = enemy_bullets[i].size.x;
        enemy_bullets[i].bullet_rec.height = enemy_bullets[i].size.y;

    }
    else if(enemy_bullets[i].down)
    {
        enemy_bullets[i].posicao.y = enemy_bullets[i].posicao.y + enemy_bullets[i].velocidade;

        enemy_bullets[i].bullet_rec.x = enemy_bullets[i].posicao.x;
        enemy_bullets[i].bullet_rec.y = enemy_bullets[i].posicao.y;

        enemy_bullets[i].bullet_rec.width = enemy_bullets[i].size.x;
        enemy_bullets[i].bullet_rec.height = enemy_bullets[i].size.y;

    }

    if(enemy_bullets[i].ativo)
    {
        // se o projetil sair da tela, ele sera reinicializado
        if(enemy_bullets[i].posicao.x < 0 || enemy_bullets[i].posicao.x > SCREEN_WIDTH || enemy_bullets[i].posicao.y < 0 || enemy_bullets[i].posicao.y > SCREEN_HEIGHT)
            {
                enemy_bullets[i].posicao = (Vector2) {(float)SCREEN_WIDTH / 2 , (float)SCREEN_HEIGHT / 2};
                enemy_bullets[i].size = (Vector2) {15, 15};
                enemy_bullets[i].ativo = false;

                enemy_bullets[i].right = false;
                enemy_bullets[i].left = false;
                enemy_bullets[i].up = false;
                enemy_bullets[i].down = false;

                enemy[i].tiro = false;
            }

    }
}

// colisao entre projeteis do player e dos inimigos
void colisao_projetil_projetil(PROJETIL bullets[], INIMIGOS enemy[], PROJETIL enemy_bullets[])
{
    for(int i = 0; i < MAX_PROJETEIS; i++)
    {
        for(int j = 0; j < MAX_INIMIGOS; j++)
        {
            if(CheckCollisionRecs(bullets[i].bullet_rec, enemy_bullets[j].bullet_rec) && bullets[i].ativo && enemy_bullets[j].ativo)
            {
                enemy_bullets[j].posicao = (Vector2) {(float)SCREEN_WIDTH / 2 , (float)SCREEN_HEIGHT / 2};
                enemy_bullets[j].size = (Vector2) {15, 15};
                enemy_bullets[j].ativo = false;

                enemy_bullets[j].right = false;
                enemy_bullets[j].left = false;
                enemy_bullets[j].up = false;
                enemy_bullets[j].down = false;

                enemy[j].tiro = false;

                bullets[i].ativo = false;

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

// colisao entre player e inimigos
void colisao_player_enemy(JOGADOR *player, INIMIGOS enemy[])
{
    for(int i = 0; i < MAX_INIMIGOS; i++)
    {
        if(CheckCollisionRecs(enemy[i].enemy_rec, player->player_rec) && enemy[i].ativo)
        {
            if(player->right)
            {
                player->posicao.x = player->posicao_backup.x;
                //player->posicao.y = player->posicao_backup.y;

                player->player_rec.x = player->posicao_backup.x;
                //player->player_rec.y = player->posicao_backup.y;
            }
            if(player->left)
            {
                player->posicao.x = player->posicao_backup.x;
                //player->posicao.y = player->posicao_backup.y;

                player->player_rec.x = player->posicao_backup.x;
                //player->player_rec.y = player->posicao_backup.y;
            }
            if(player->up)
            {
                //player->posicao.x = player->posicao_backup.x;
                player->posicao.y = player->posicao_backup.y;

                //player->player_rec.x = player->posicao_backup.x;
                player->player_rec.y = player->posicao_backup.y;
            }
            if(player->down)
            {
                //player->posicao.x = player->posicao_backup.x;
                player->posicao.y = player->posicao_backup.y;

                //player->player_rec.x = player->posicao_backup.x;
                player->player_rec.y = player->posicao_backup.y;
            }

            if(enemy[i].right)
            {
                enemy[i].posicao.x = enemy[i].posicao_backup.x;
                //enemy[i].posicao.y = enemy[i].posicao_backup.y;
                enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
                //enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;

            }
            if(enemy[i].left)
            {
                enemy[i].posicao.x = enemy[i].posicao_backup.x;
                //enemy[i].posicao.y = enemy[i].posicao_backup.y;
                enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
                //enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;

            }
            if(enemy[i].up)
            {
                //enemy[i].posicao.x = enemy[i].posicao_backup.x;
                enemy[i].posicao.y = enemy[i].posicao_backup.y;
                //enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
                enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;

            }
            if(enemy[i].down)
            {
                //enemy[i].posicao.x = enemy[i].posicao_backup.x;
                enemy[i].posicao.y = enemy[i].posicao_backup.y;
                //enemy[i].enemy_rec.x = enemy[i].posicao_backup.x;
                enemy[i].enemy_rec.y = enemy[i].posicao_backup.y;

            }

        }
    }
}

//colisao entre player e projeteis do inimigo(player perde vidas)
void colisao_player_projetil_inimigo(JOGADOR *player, INIMIGOS enemy[], PROJETIL enemy_bullets[])
{
    for(int i = 0; i < MAX_INIMIGOS; i++)
    {
        if(CheckCollisionRecs(enemy_bullets[i].bullet_rec, player->player_rec) && enemy_bullets[i].ativo)
        {
            enemy_bullets[i].ativo = false;
            enemy[i].tiro = false;
            player->vidas = player->vidas - 1;
        }
    }
}

// colisao entre inimigos e tijolos
bool verifica_colisao_inimigo_tij(INIMIGOS enemy[], TIJOLOS tij[][COLUNAS], int i)
{
    bool colisao = false;

    for(int k = 0; k <  LINHAS; k++)
    {
        for(int j = 0; j < COLUNAS; j++)
        {
            if(CheckCollisionRecs(enemy[i].enemy_rec, tij[k][j].tij_rec) && tij[k][j].ativo)
            {
                colisao = true;
            }
        }
    }
    return colisao;
}

// colisao entre projetil do player e inimigos (player ganha pontos ao matar inmigos)
bool verifica_colisao_inimigo_projetil(INIMIGOS enemy[], PROJETIL bullets[], int i, JOGADOR *player)
{
    bool colisao = false;

    for(int k = 0; k < MAX_PROJETEIS; k++)
    {
        if(CheckCollisionRecs(bullets[k].bullet_rec, enemy[i].enemy_rec) && bullets[k].ativo && enemy[i].ativo)
        {
            colisao = true;
            bullets[k].ativo = false;
            enemy[i].ativo = false;
            player->score = player->score + 800;
        }

    }
    return colisao;
}

// colisao projetil inimigo e paredes (tjiolos sao destruidos ao entrar em contato com projeteis)
void verifica_colisao_tij_projetil_inimigo(INIMIGOS enemy[], PROJETIL enemy_bullets[], TIJOLOS tij[][COLUNAS], int i)
{
    for(int j = 0; j < LINHAS; j++)
    {
        for(int k = 0; k < COLUNAS; k++)
        {
            if(CheckCollisionRecs(enemy_bullets[i].bullet_rec, tij[j][k].tij_rec) && tij[j][k].ativo)
            {
                tij[j][k].ativo = false;
                enemy[i].tiro = false;
                enemy_bullets[i].ativo = false;
            }
        }
    }

}


// verifica colisoes entre os proprios inimigos
bool verifica_colisao_inimigo_inimigo(INIMIGOS enemy[], int i)
{
    bool colisao = false;

    for(int k = 0; k < MAX_INIMIGOS; k++)
    {
        if(k != i)
        {
            if(CheckCollisionRecs(enemy[i].enemy_rec, enemy[k].enemy_rec) && enemy[k].ativo && enemy[i].ativo)
            {
                colisao = true;
            }
        }

    }
    return colisao;
}

// spawn dos inimigos
// contador para eles spawnarem de 5 em 5 segundos
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

// verifica se os criterios para passar de nivel foram atingidos(10 seg de fase e todos inimigos mortos)
void passa_nivel(INIMIGOS enemy[], int *tempo_nivel, char tijolosText[], TIJOLOS tij [][COLUNAS], int *nivel, JOGADOR *player, int *enemy_number)
{
    *tempo_nivel = *tempo_nivel + 1;
    bool inimigos = false;

    // verifica se tem algum inimigo vivo
    for(int i = 0; i < MAX_INIMIGOS; i++)
    {
        if(enemy[i].ativo)
        {
            inimigos = true;
        }
    }

    // verifica tbm se ja passaram 10 segundos desde o comeco do nivel
    if((!inimigos) && (*tempo_nivel > 600) && *nivel == 1)
    {
        // mesma logica para colocar tijolos do comeco
        strcpy(tijolosText, LoadFileText("mapas/mapa2.txt"));

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


                tij[i][j].size = (Vector2) {25, 40};
                tij[i][j].ativo = false;
                tij[i][j].atingido_projetil = false;

                tij[i][j].tij_rec.x = tij[i][j].posicao.x;
                tij[i][j].tij_rec.y = tij[i][j].posicao.y;
                tij[i][j].tij_rec.width = 25;
                tij[i][j].tij_rec.height = 40;

                contador_colunas++;
                contador_string_mapa++;
            }
            contador_linhas++;
        }
        // sobe nivel
        *nivel = *nivel + 1;

        // inimigos resetados
        *enemy_number = 0;

        // coloca tijolos do novo nivel
        coloca_tijolo(tij, player);
        // reseta tempo do nivel
        *tempo_nivel = 0;
    }
}

    void coloca_username(JOGADOR *player){
        int letterCount = 0;
        Rectangle textBox = { SCREEN_WIDTH/2.0f - 100, 180, 225, 50 };
        bool mouseOnText = false;
        int framesCounter = 0;
        SetTargetFPS(10);
         // Update
        //----------------------------------------------------------------------------------
        if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;

        if (mouseOnText)
        {
            // Set the window's cursor to the I-Beam
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            // Pega a tecla pressionada
            int key = GetCharPressed();

            // checa se mais caracteres foram colocados no mesmo frame
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_LETRAS))
                {
                    player->username[letterCount] = (char)key;
                    player->username[letterCount+1] = '\0'; // adiciona \0 no final da string
                    letterCount++;
                }

                key = GetCharPressed();  // checa proximo caractere da fila
            }

            if(IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                player->username[letterCount] = '\0';
            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if(mouseOnText){
            framesCounter++;
        }
        else{
            framesCounter = 0;
        }
        //----------------------------------------------------------------------------------

        // desenha
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            DrawText("COLOQUE O MOUSE SOBRE A CAIXA!", 330, 140, 20, GRAY);

            DrawRectangleRec(textBox, LIGHTGRAY);
            if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
            else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

            DrawText(player->username, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

            DrawText(TextFormat("Número de caracteres: %i/%i", letterCount, MAX_LETRAS), 368, 240, 20, DARKGRAY);

            if (mouseOnText)
            {
                if (letterCount < MAX_LETRAS)
                {
                    // desenha underline para identificar onde irá escrever
                    if (((framesCounter/20)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(player->username, 40), (int)textBox.y + 12, 40, MAROON);
                }
                else DrawText("Pressione BACKSPACE para deletar caracteres...", 200, 330, 20, WHITE);
            }

        EndDrawing();
    }

bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126)) keyPressed = true;

    return keyPressed;
}

void save_game(JOGADOR *player, INIMIGOS enemy[], TIJOLOS tij[][COLUNAS], CELULA *energia){
    if(IsKeyDown(KEY_S)){
        FILE *arqSave;
        arqSave = fopen("save.bin", "wb");
        fwrite(&player, sizeof(JOGADOR), 1, arqSave);
        fwrite(&enemy, sizeof(INIMIGOS), 1, arqSave);
        fwrite(&tij, sizeof(TIJOLOS), 1, arqSave);
        fwrite(&energia, sizeof(CELULA), 1, arqSave);
        DrawText("Game saved", ((SCREEN_WIDTH - MeasureText("Game saved", 15))/2), 330, 15, WHITE);
        fclose(arqSave);
    }
}


