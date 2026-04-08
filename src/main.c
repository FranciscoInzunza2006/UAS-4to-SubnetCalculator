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
#include <limits.h>
#include <time.h>

#include "calculator.h"
#include "raygui.h"

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------
static void BtnCalculate();

constexpr int IP_MAX_LENGTH = 16;

int in_value_typeActive = 0;
char in_ipText[IP_MAX_LENGTH] = "192.168.0.1";
int in_ip_classActive = 0;
int in_valueValue = 10;


char out_netsText[64] = "";
char out_hostsText[64] = "";
char out_maskText[64] = "";
char out_range_first_1Text[64] = "";
char out_range_first_2Text[64] = "";
char out_range_first_3Text[64] = "";
char out_range_last_3Text[64] = "";
char out_range_last_2Text[64] = "";
char out_range_last_1Text[64] = "";
char out_time_takenText[64] = "";

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
    bool in_valueEditMode = false;

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
        if (GuiTextBox((Rectangle){72, 48, 120, 24}, in_ipText, IP_MAX_LENGTH, in_ipEditMode))
            in_ipEditMode = !in_ipEditMode;

        GuiLabel((Rectangle){24, 80, 48, 24}, "Clase");
        GuiToggleGroup((Rectangle){72, 80, 40, 24}, "A;B;C", &in_ip_classActive);

        GuiLabel((Rectangle){24, 112, 48, 24}, "Usar");
        GuiToggleGroup((Rectangle){72, 112, 120, 24}, "Subredes;HostsxSubred", &in_value_typeActive);

        if (GuiValueBox((Rectangle){72, 144, 120, 24}, "Valor    ", &in_valueValue, 0, INT_MAX, in_valueEditMode))
            in_valueEditMode = !in_valueEditMode;

        if (GuiButton((Rectangle){24, 176, 120, 24}, "Calcular")) BtnCalculate();

        GuiGroupBox((Rectangle){336, 24, 288, 360}, "Resultados");
        GuiLabel((Rectangle){344, 48, 120, 24}, "# de Redes");
        GuiLabel((Rectangle){464, 48, 120, 24}, out_netsText);

        GuiLabel((Rectangle){344, 72, 120, 24}, "# de HostsxSubred");
        GuiLabel((Rectangle){464, 72, 120, 24}, out_hostsText);

        GuiLabel((Rectangle){344, 96, 120, 24}, "Mascara");
        GuiLabel((Rectangle){464, 96, 120, 24}, out_maskText);

        GuiLabel((Rectangle){344, 128, 120, 24}, "Primeros 3 rangos");
        GuiLabel((Rectangle){352, 152, 232, 24}, out_range_first_1Text);
        GuiLabel((Rectangle){352, 176, 232, 24}, out_range_first_2Text);
        GuiLabel((Rectangle){352, 200, 232, 24}, out_range_first_3Text);

        GuiLabel((Rectangle){344, 232, 120, 24}, "Ultimos 3 rangos");
        GuiLabel((Rectangle){352, 256, 232, 24}, out_range_last_3Text);
        GuiLabel((Rectangle){352, 280, 232, 24}, out_range_last_2Text);
        GuiLabel((Rectangle){352, 304, 232, 24}, out_range_last_1Text);

        GuiLabel((Rectangle){344, 336, 120, 24}, "Tiempo tomado (s)");
        GuiLabel((Rectangle){464, 336, 120, 24}, out_time_takenText);
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
    // Reset outs
    out_netsText[0] = '\0';
    out_hostsText[0] = '\0';
    out_maskText[0] = '\0';
    out_range_first_1Text[0] = '\0';
    out_range_first_2Text[0] = '\0';
    out_range_first_3Text[0] = '\0';
    out_range_last_3Text[0] = '\0';
    out_range_last_2Text[0] = '\0';
    out_range_last_1Text[0] = '\0';
    out_time_takenText[0] = '\0';

    bool result = false;
    const char** ip_parts = parseIp(in_ipText, &result);
    if (!result) {
        printf("Invalid IP");
        return;
    }

    char ip_class;
    switch (in_ip_classActive) {
        case 0: ip_class = 'A';
            break;
        case 1: ip_class = 'B';
            break;
        case 2: ip_class = 'C';
            break;
        default: return;
    }
    const int bits_available = ipClassToBits(ip_class);

    int subnet_amount;
    int hosts_by_subnet;

    int* target_value = in_value_typeActive == 0 ? &subnet_amount : &hosts_by_subnet;
    *target_value = in_valueValue;

    const int bits_needed = BitsNeededToRepresentNumber(*target_value);
    if (bits_needed > bits_available - 2) {
        printf("There isn't any available IP with these settings.");
        return;
    }

    *target_value = 1 << bits_needed; // Round up to next power if needed
    int* value_to_calculate = in_value_typeActive == 1 ? &subnet_amount : &hosts_by_subnet;
    *value_to_calculate = 1 << (bits_available - bits_needed);

    printf(
        "IP Address: %s\n"
        "IP Class: %c\n"
        "Total subnets: %d\n"
        "Hosts by subnet: %d\n",
        in_ipText, ip_class, subnet_amount, hosts_by_subnet);

    const clock_t start = clock();
    writeRangesToFile(ip_parts, ip_class, subnet_amount, hosts_by_subnet);
    const clock_t end = clock();
    const double time_taken = (float)(end - start) / CLOCKS_PER_SEC;

    TextCopy(out_netsText,  TextFormat("%d", subnet_amount));
    TextCopy(out_hostsText,  TextFormat("%d", hosts_by_subnet));
    TextCopy(out_maskText,  getMask(hosts_by_subnet));
    TextCopy(out_time_takenText,  TextFormat("%.4f", time_taken));
}
