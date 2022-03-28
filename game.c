#include "raylib.h"

#define PHYSAC_IMPLEMENTATION
#include "extras/physac.h"

#define VELOCITY    2.5f

int main(void){

    // Initialization
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    Vector2 tankPosition = {(float)screenWidth/2, (float)screenHeight/2};
    Vector2 size = {(float)20, (float)20};

    SetTargetFPS(60);
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
       

        if (IsKeyDown(KEY_RIGHT)) tankPosition.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) tankPosition.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) tankPosition.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) tankPosition.y += 2.0f;

        // Draw
        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        BeginDrawing();

            ClearBackground(BLACK);

        DrawRectangleV(tankPosition,size, GREEN);

        bool CheckCollisionRecs(tankPosition, Window);

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