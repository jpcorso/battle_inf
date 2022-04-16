#include "raylib.h"
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TANKSPEED 3.5f

int main(void){

    // Initialization
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    Vector2 tankPosition = {(float)screenWidth/2.0f, (float)screenHeight/2.0f}; //posição inicial do tanque
    Vector2 tankPositionBackup = tankPosition;

    Vector2 tankRedPosition = {(float)screenWidth/1.7f, (float)screenHeight/1.7f};

    Vector2 size = {(float)20, (float)20}; //tamanho do retangulo

    Texture2D tank = LoadTexture("./public/tank.png");

    Texture2D tijolo = LoadTexture("./public/tijolo.png");
    Texture2D coluna_tijolo = LoadTexture("./public/coluna_tijolo.png");
    Texture2D linha_tijolo = LoadTexture("./public/linha_tijolo.png");

    Texture2D wallpaper = LoadTexture("./public/wppfase1.png");

    SetTargetFPS(60);
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
       

        
        // Draw
        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        BeginDrawing();

            ClearBackground(BLACK);

        //textura do wallpaper
        DrawTexture(wallpaper, 0, 0, RAYWHITE);

        //texturas dos tijolos

        //escada de tijolos
        DrawTexture(tijolo, 200, 100, RAYWHITE);
        DrawTexture(tijolo, 230, 120, RAYWHITE);
        DrawTexture(tijolo, 260, 140, RAYWHITE);
        DrawTexture(tijolo, 290, 160, RAYWHITE);

        DrawTexture(tijolo, 100, 270, RAYWHITE);
        DrawTexture(tijolo, 20, 50, RAYWHITE);

        DrawTexture(coluna_tijolo, 20, 100, RAYWHITE);
        DrawTexture(coluna_tijolo, 90, 100, RAYWHITE);
        DrawTexture(tijolo, 70, 300, RAYWHITE);

        //"U" de tijolos
        DrawTexture(linha_tijolo, 500, 400, RAYWHITE);
        DrawTexture(coluna_tijolo, 500, 400, RAYWHITE);
        DrawTexture(coluna_tijolo, 600, 400, RAYWHITE);

        DrawTexture(linha_tijolo, 90, 400, RAYWHITE);
        DrawTexture(coluna_tijolo, 10, 400, RAYWHITE);
        DrawTexture(linha_tijolo, 600, 200, RAYWHITE);
        DrawTexture(coluna_tijolo, 600, 200, RAYWHITE);
        DrawTexture(coluna_tijolo, 200, 250, RAYWHITE);

        DrawTextureEx(tank, tankRedPosition, 90, 1, RED);


        //movimento das setas e sentido/direção do tanque
        tankPositionBackup = tankPosition;
        if(IsKeyDown(KEY_RIGHT)){
            DrawTexturePro(tank, tankPosition,270, 1, GREEN);
            tankPosition.x += TANKSPEED; 
        }
        else{
            if(IsKeyDown(KEY_LEFT)){
                DrawTexturePro(tank, tankPosition,90, 1, GREEN);
                tankPosition.x -= TANKSPEED;
            }
            else{
                if(IsKeyDown(KEY_UP)){
                    DrawTexturePro(tank, tankPosition,180, 1, GREEN);
                    tankPosition.y -= TANKSPEED;
                }
                else{
                    if(IsKeyDown(KEY_DOWN)){
                        DrawTexturePro(tank, tankPosition,360, 1, GREEN);
                        tankPosition.y += TANKSPEED;
                    }
                    else{
                        DrawTexturePro(tank, tankPosition,270, 1, GREEN);
                         
                    }
                }
            }
        }


        //Retangulo do limite para tanque não sair da tela
        Rectangle limite = {0, 0, 800, 600};

        Rectangle player = {tankPosition.x, tankPosition.y, tank.width, tank.height};
        Rectangle inter = GetCollisionRec(player, limite);

        Rectangle retangulo_tijolo = {70, 300, 40, 25};

        /*if ((tankPosition.x >= (GetScreenWidth() - 10)) || (tankPosition.x <= 10)) tankPosition.x *= -1.0f;
        if ((tankPosition.y >= (GetScreenHeight() - 10)) || (tankPosition.y <= 10)) tankPosition.y *= -1.0f;*/

        //Checa colisão entre tanque e retangulo do limite
        /*if (CheckCollisionPointRec(tankPosition, limite)){
        
           tankPosition.x = tankPosition.x;
           tankPosition.y = tankPosition.y;

        }

        else{

        }*/

        if(inter.width != player.width || inter.height != player.height || inter.x != player.x || inter.y != player.y )
            tankPosition = tankPositionBackup;     
          
        EndDrawing();
        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   }

    //De-Initialization
   //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

   //TODO: Unload all loaded resources at this point

   CloseWindow();        // Close window and OpenGL context
   //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    return 0;
}