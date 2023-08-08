#include "Functions.hpp"
#include "Vector2.hpp"
#include "raylib.h"
#include "raylib-cpp.hpp"

#include "atlas.h"

const int screenWidth = 640;
const int screenHeight = 480;


int main() {
    /// Initialization ///
    InitWindow(screenWidth, screenHeight, "raylib-cpp-cmake example game");
    SetTargetFPS(60);
    
    /// Main game loop ///
    while (!WindowShouldClose()) {
        /// Update ///
        

        /// Draw ///
        BeginDrawing();
            ClearBackground(RAYWHITE);

            int fontSize = 32;
            raylib::Vector2 textSize = MeasureTextEx(GetFontDefault(), "Raylib Example", fontSize, fontSize/10);
            raylib::DrawText("Raylib Example", (screenWidth - textSize.x)/2, (screenHeight - textSize.y)/2, fontSize, BLACK);

        EndDrawing();
    }

}