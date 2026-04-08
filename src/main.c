/*******************************************************************************************
*
*   Calculator v1.0.0 - Subnet calculator
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 Paco. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------
static void BtnCalculate();

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 700;
    int screenHeight = 400;

    InitWindow(screenWidth, screenHeight, "Calculator");

    // Calculator: controls initialization
    //----------------------------------------------------------------------------------
    bool in_ipEditMode = false;
    char in_ipText[128] = "SAMPLE TEXT";
    int in_ip_classActive = 0;
    int in_value_typeActive = 0;
    bool in_valueEditMode = false;
    int in_valueValue = 0;
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Implement required update logic
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        // raygui: controls drawing
        //----------------------------------------------------------------------------------
        GuiLabel((Rectangle){24, 16, 120, 24}, "Calculadora de Redes");
        GuiLabel((Rectangle){24, 48, 48, 24}, "IP");
        if (GuiTextBox((Rectangle){72, 48, 120, 24}, in_ipText, 128, in_ipEditMode)) in_ipEditMode = !in_ipEditMode;

        GuiLabel((Rectangle){24, 80, 48, 24}, "Clase");
        GuiToggleGroup((Rectangle){72, 80, 40, 24}, "A;B;C", &in_ip_classActive);

        GuiLabel((Rectangle){24, 112, 48, 24}, "Usar");
        GuiToggleGroup((Rectangle){72, 112, 120, 24}, "Redes;HostsxSubred", &in_value_typeActive);

        if (GuiValueBox((Rectangle){72, 144, 120, 24}, "Valor    ", &in_valueValue, 0, 100, in_valueEditMode))
            in_valueEditMode = !in_valueEditMode;

        if (GuiButton((Rectangle){24, 176, 120, 24}, "Calcular")) BtnCalculate();

        GuiGroupBox((Rectangle){336, 24, 288, 360}, "Resultados");
        GuiLabel((Rectangle){344, 48, 120, 24}, "# de Redes");
        GuiLabel((Rectangle){464, 48, 120, 24}, "SAMPLE TEXT");

        GuiLabel((Rectangle){344, 72, 120, 24}, "# de HostsxSubred");
        GuiLabel((Rectangle){464, 72, 120, 24}, "SAMPLE TEXT");

        GuiLabel((Rectangle){344, 96, 120, 24}, "Mascara");
        GuiLabel((Rectangle){464, 96, 120, 24}, "SAMPLE TEXT");

        GuiLabel((Rectangle){344, 128, 120, 24}, "Primeros 3 rangos");
        GuiLabel((Rectangle){352, 152, 232, 24}, "SAMPLE TEXT");
        GuiLabel((Rectangle){352, 176, 232, 24}, "SAMPLE TEXT");
        GuiLabel((Rectangle){352, 200, 232, 24}, "SAMPLE TEXT");

        GuiLabel((Rectangle){344, 232, 120, 24}, "Ultimos 3 rangos");
        GuiLabel((Rectangle){352, 256, 232, 24}, "SAMPLE TEXT");
        GuiLabel((Rectangle){352, 280, 232, 24}, "SAMPLE TEXT");
        GuiLabel((Rectangle){352, 304, 232, 24}, "SAMPLE TEXT");

        GuiLabel((Rectangle){344, 336, 120, 24}, "Tiempo tomado (s)");
        GuiLabel((Rectangle){464, 336, 120, 24}, "SAMPLE TEXT");
        //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------
static void BtnCalculate() {
    // TODO: Implement control logic
}
