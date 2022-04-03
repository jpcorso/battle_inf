#include "raylib.h"

#define VELOCITY    20.0f

int main(void){

    // Initialization
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    Vector2 tankPosition = {(float)screenWidth/2, (float)screenHeight/2}; //posição inicial do tanque

    Vector2 size = {(float)20, (float)20}; //tamanho do retangulo

    Texture2D tank = LoadTexture("./public/tank.png");
    Texture2D tijolo = LoadTexture("./public/tijolo.png");
    Texture2D coluna_tijolo = LoadTexture("./public/coluna_tijolo.png");

    SetTargetFPS(60);
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
       

        if (IsKeyDown(KEY_RIGHT)) tankPosition.x += 3.5f;
        if (IsKeyDown(KEY_LEFT)) tankPosition.x -= 3.5f;
        if (IsKeyDown(KEY_UP)) tankPosition.y -= 3.5f;
        if (IsKeyDown(KEY_DOWN)) tankPosition.y += 3.5f;

        // Draw
        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        BeginDrawing();

            ClearBackground(BLACK);

        DrawTexture(tijolo, 200, 100, RAYWHITE);
        DrawTexture(tijolo, 230, 120, RAYWHITE);



        DrawTexture(tijolo, 20, 50, RAYWHITE);
        DrawTexture(coluna_tijolo, 20, 100, RAYWHITE);
        DrawTexture(coluna_tijolo, 90, 100, RAYWHITE);
        DrawTexture(tijolo, 70, 300, RAYWHITE);

        DrawTextureV(tank, tankPosition, GREEN);

        Rectangle limite = {0, 0, 780, 580};

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