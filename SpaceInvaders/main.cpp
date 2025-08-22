#include "Core/GameManager.h"
#include "Core/GameState.h"
#include "Widgets/BackgroundWidget.h"

int main(void)
{
	GameManager& gameManager = GameManager::GetInstance();
	GameState& gameState = GameState::GetInstance();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Space Invaders");
    InitAudioDevice();

    gameManager.LoadRessources();
    gameState.LoadStartMenu();
    Music music = LoadMusicStream("../Resources/Sounds/musicInvaders.wav");
    music.looping = true;

    PlayMusicStream(music);

    // The background widget is persistent across all levels
    // And needs to be draw first
    BackgroundWidget m_backgroundWidget(GameManager::GetInstance().GetTexture("starSheet"));

    SetTargetFPS(120);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateMusicStream(music);

        // Get the time since the last frame
        float deltaTime = GetFrameTime();
        m_backgroundWidget.Update(deltaTime);
        gameManager.Update(deltaTime);
        gameState.Update(deltaTime);

        gameManager.CollisionCheck();
        gameManager.CleanupActors();
        gameManager.FlushPendingLists();

        BeginDrawing();
        ClearBackground(BLACK);
        m_backgroundWidget.Draw();
        gameManager.Draw();
        EndDrawing();
    }

    StopMusicStream(music);
    UnloadMusicStream(music);
	gameManager.UnloadSounds();
    gameManager.UnloadTextures();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}