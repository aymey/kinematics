#include "raylib.h"

int main(void) {
    InitWindow(800, 600, "kinematics");

    while(!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
