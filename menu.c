/*******************************************************************************************
*
*   raylib [textures] example - sprite button
*
*   This example has been created using raylib 2.5 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2019 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define NUM_FRAMES  3       // Number of frames (rectangles) for the button sprite texture

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "raylib [textures] example - sprite button");

    InitAudioDevice();      // Initialize audio device

    Sound fxButton = LoadSound("resources/buttonfx.wav");   // Load button sound

    Texture2D novo_jogo = LoadTexture("resources/novo_jogo.png"); // Load NOVO JOGO button texture
    Texture2D continuar = LoadTexture("resources/continuar.png"); // Load CONTINUAR button texture
    Texture2D carregar_mapa = LoadTexture("resources/carregar_mapa.png"); // Load CARREGAR MAPA button texture
    Texture2D ranking = LoadTexture("resources/ranking.png"); // Load RANKING button texture
    Texture2D sair = LoadTexture("resources/sair.png"); // Load SAIR button texture

    // Define frame rectangle for drawing
    float frameHeight = (float)novo_jogo.height/NUM_FRAMES;
    Rectangle sourceRec = { 0, 0, (float)novo_jogo.width, frameHeight };

    float frameHeight1 = (float)continuar.height/NUM_FRAMES;
    Rectangle sourceRec1 = { 1, 1, (float)continuar.width, frameHeight1 };

    float frameHeight2 = (float)carregar_mapa.height/NUM_FRAMES;
    Rectangle sourceRec2 = { 0, 0, (float)continuar.width, frameHeight2 };

    float frameHeight3 = (float)ranking.height/NUM_FRAMES;
    Rectangle sourceRec3 = { 0, 0, (float)continuar.width, frameHeight3 };

    float frameHeight4 = (float)sair.height/NUM_FRAMES;
    Rectangle sourceRec4 = { 0, 0, (float)continuar.width, frameHeight4 };

    

    // Define button bounds on screen
    Rectangle btnBounds = { screenWidth/2.0f - novo_jogo.width/2.0f, screenHeight/3.0f - novo_jogo.height/NUM_FRAMES/3.0f, (float)novo_jogo.width, frameHeight };
    Rectangle btnBounds1 = { screenWidth/2.0f - continuar.width/2.0f, screenHeight/2.0f - continuar.height/NUM_FRAMES/2.0f, (float)continuar.width, frameHeight1 };
    Rectangle btnBounds2 = { screenWidth/2.0f - carregar_mapa.width/2.0f, screenHeight/1.5f - carregar_mapa.height/NUM_FRAMES/1.5f, (float)carregar_mapa.width, frameHeight2 };
    Rectangle btnBounds3 = { screenWidth/2.0f - ranking.width/2.0f, screenHeight/1.2f - ranking.height/NUM_FRAMES/1.2f, (float)ranking.width, frameHeight3 };
    Rectangle btnBounds4 = { screenWidth/2.0f - sair.width/2.0f, screenHeight/1.0f - sair.height/NUM_FRAMES/1.0f, (float)sair.width, frameHeight4 };



    int btnState = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    int btnState1 = 0;
    int btnState2 = 0;
    int btnState3 = 0;
    int btnState4 = 0;

    bool btnAction = false;         // Button action should be activated

    Vector2 mousePoint = { 0.0f, 0.0f };

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        mousePoint = GetMousePosition();
        btnAction = false;

        // Check button state
        if (CheckCollisionPointRec(mousePoint, btnBounds))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){ 
                btnState = 2;
                btnState1 = 0;
                btnState2 = 0;
                btnState3 = 0;
                btnState4 =0;
            }
            else{
                 btnState = 1;
            }     

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
        }
        else btnState = 0;

        if (btnAction)
        {
            PlaySound(fxButton);
        }
        if (CheckCollisionPointRec(mousePoint, btnBounds1))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                btnState = 0;
                btnState1 = 2;
                btnState2 = 0;
                btnState3 = 0;
                btnState4 =0;
            }    
            else{
                btnState1 = 1;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
        }
        else btnState1 = 0;

        if (btnAction)
        {
            PlaySound(fxButton);
        }

        if (CheckCollisionPointRec(mousePoint, btnBounds2))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                btnState = 0;
                btnState1 = 0;
                btnState2 = 2;
                btnState3 = 0;
                btnState4 =0;
            }    
            else{
                btnState2 = 1;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
        }
        else btnState2 = 0;

        if (btnAction)
        {
            PlaySound(fxButton);         
        }


        if (CheckCollisionPointRec(mousePoint, btnBounds3))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                btnState = 0;
                btnState1 = 0;
                btnState2 = 0;
                btnState3 = 2;
                btnState4 =0;
            }    
            else{
                btnState3 = 1;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
        }
        else btnState3 = 0;

        if (btnAction)
        {
            PlaySound(fxButton);         
        }


        if (CheckCollisionPointRec(mousePoint, btnBounds4))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                btnState = 0;
                btnState1 = 0;
                btnState2 = 0;
                btnState3 = 0;
                btnState4 =2;
            }    
            else{
                btnState4 = 1;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
        }
        else btnState4 = 0;

        if (btnAction)
        {
            PlaySound(fxButton);         
        }

         // TODO: Any desired action
        // Calculate button frame rectangle to draw depending on button state
        sourceRec.y = btnState*frameHeight;
        sourceRec1.y = btnState*frameHeight1;
        sourceRec2.y = btnState*frameHeight2;
        sourceRec3.y = btnState*frameHeight3;
        sourceRec4.y = btnState*frameHeight4;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GRAY);

            DrawTextureRec(novo_jogo, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE); // Draw button frame
            DrawTextureRec(continuar, sourceRec1, (Vector2){ btnBounds1.x, btnBounds1.y }, WHITE); // Draw button frame
            DrawTextureRec(carregar_mapa, sourceRec2, (Vector2){ btnBounds2.x, btnBounds2.y }, WHITE); // Draw button frame
            DrawTextureRec(ranking, sourceRec3, (Vector2){ btnBounds3.x, btnBounds3.y }, WHITE); // Draw button frame
            DrawTextureRec(sair, sourceRec4, (Vector2){ btnBounds4.x, btnBounds4.y }, WHITE); // Draw button frame





        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(sair);  // Unload button texture
    UnloadTexture(ranking);  // Unload button texture
    UnloadTexture(carregar_mapa);  // Unload button texture
    UnloadTexture(continuar);  // Unload button texture
    UnloadTexture(novo_jogo);  // Unload button texture
    UnloadSound(fxButton);  // Unload sound

    CloseAudioDevice();     // Close audio device

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
