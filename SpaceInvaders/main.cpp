#include "Core/GameManager.h"
#include "Widgets/BackgroundWidget.h"

int main(void)
{
	GameManager& gameManager = GameManager::GetInstance();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");

    gameManager.LoadRessources();
    gameManager.LoadStartMenu();

    // The background widget is persistent across all levels
    // And needs to be draw first
    BackgroundWidget m_backgroundWidget(GameManager::GetInstance().GetTexture("starSheet"));

    SetTargetFPS(120);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Get the time since the last frame
        float deltaTime = GetFrameTime();

        m_backgroundWidget.Update(deltaTime);
        gameManager.Update(deltaTime);
        gameManager.CollisionCheck();
        gameManager.CleanupActors();
        gameManager.FlushPendingLists();


        BeginDrawing();

        ClearBackground(BLACK);
        m_backgroundWidget.Draw();
        gameManager.Draw();

        EndDrawing();
    }

    gameManager.UnloadTextures();
    CloseWindow();
    return 0;
}