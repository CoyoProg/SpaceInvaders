#include "raylib.h"
#include <memory>

#include "Core/GameManager.h"
#include "Actors/Actor.h"

int main(void)
{
	GameManager& gameManager = GameManager::GetInstance();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");

    gameManager.LoadRessources();
    gameManager.InitializeGame();

    SetTargetFPS(120);
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

    gameManager.UnloadTextures();
    CloseWindow();
    return 0;
}