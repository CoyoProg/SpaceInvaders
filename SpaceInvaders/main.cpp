#include "raylib.h"
#include <memory>

#include "Core/GameManager.h"
#include "Actors/Actor.h"

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Space Invaders");
    SetTargetFPS(120);

	GameManager& gameManager = GameManager::GetInstance();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        gameManager.Update();
        gameManager.CollisionCheck();
        gameManager.CleanupActors();
        gameManager.FlushPendingLists();

        BeginDrawing();
        ClearBackground(BLACK);

        gameManager.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}