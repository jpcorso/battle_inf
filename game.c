#include "raylib.h"

int main(void){

    // Initialization
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    Vector2 tankPosition = {(float)screenWidth/2.0f, (float)screenHeight/2.0f}; //posição inicial do tanque

    Vector2 size = {(float)20, (float)20}; //tamanho do retangulo

    Texture2D tank = LoadTexture("./public/tank.png");
    Texture2D tijolo = LoadTexture("./public/tijolo.png");
    Texture2D coluna_tijolo = LoadTexture("./public/coluna_tijolo.png");
    Texture2D linha_tijolo = LoadTexture("./public/linha_tijolo.png");

    SetTargetFPS(60);
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
       

        
        // Draw
        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        BeginDrawing();

            ClearBackground(BLACK);

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

        DrawTexture(linha_tijolo, 400, 90, RAYWHITE);
        DrawTexture(linha_tijolo, 90, 400, RAYWHITE);
        DrawTexture(coluna_tijolo, 10, 400, RAYWHITE);
        DrawTexture(linha_tijolo, 600, 200, RAYWHITE);
        DrawTexture(coluna_tijolo, 600, 200, RAYWHITE);
        DrawTexture(coluna_tijolo, 400, 250, RAYWHITE);


        //movimento das setas e sentido/direção do tanque
        if(IsKeyDown(KEY_RIGHT)){
            DrawTextureEx(tank, tankPosition,270, 1, GREEN);
            tankPosition.x += 3.5f; 
        }
        else{
            if(IsKeyDown(KEY_LEFT)){
                DrawTextureEx(tank, tankPosition,90, 1, GREEN);
                tankPosition.x -= 3.5f;
            }
            else{
                if(IsKeyDown(KEY_UP)){
                    DrawTextureEx(tank, tankPosition,360, 1, GREEN);
                    tankPosition.y -= 3.5f;
                }
                else{
                    if(IsKeyDown(KEY_DOWN)){
                        DrawTextureEx(tank, tankPosition,180, 1, GREEN);
                        tankPosition.y += 3.5f;
                    }
                    else{
                        DrawTextureEx(tank, tankPosition,270, 1, GREEN);
                    }
                }
            }
        }

        //Retangulo do limite para tanque não sair da tela
        Rectangle limite = {0, 0, 760, 560};

        /*if ((tankPosition.x >= (GetScreenWidth() - 10)) || (tankPosition.x <= 10)) tankPosition.x *= -1.0f;
        if ((tankPosition.y >= (GetScreenHeight() - 10)) || (tankPosition.y <= 10)) tankPosition.y *= -1.0f;*/

        //Checa colisão entre tanque e retangulo do limite
        if (CheckCollisionPointRec(tankPosition, limite))
        {

        }
        else{
            tankPosition.x *= -1;
            tankPosition.y *= -1;
        }

        
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