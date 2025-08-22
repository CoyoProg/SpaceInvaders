#include "HUDWidget.h"
#include "../Core/GameManager.h"

HUDWidget::HUDWidget() :
	m_scoreWidget(Vector2{ 25.0f,25.0f }, 20),
	m_livesWidget(Vector2{ 25.0f,895.0f }, Vector2{ 50.0f,20.0f }, 3)
{
}

void HUDWidget::Draw()
{
	// Draw the playground boundaries
	DrawLine(PLAYGROUND_OFFSET, SCREEN_HEIGHT - 55, PLAYGROUND_OFFSET, SCREEN_HEIGHT - 45, GREEN);
	DrawLine(SCREEN_WIDTH - PLAYGROUND_OFFSET, SCREEN_HEIGHT - 55, SCREEN_WIDTH - PLAYGROUND_OFFSET, SCREEN_HEIGHT - 45, GREEN);

	// DEBUG: Display the number of actors currently in the game
	//DrawText(
	//	TextFormat("Actor Count: %01i",GameManager::GetInstance().GetActorCount()),
	//	SCREEN_WIDTH - MeasureText("Actor Count: 00", 20) - 25,
	//	870,
	//	20,
	//	GREEN);

	m_scoreWidget.Draw();
	m_livesWidget.Draw();
}

void HUDWidget::NotifyScoreUpdate(int scoreP)
{
	m_scoreWidget.SetScore(scoreP);
}

void HUDWidget::NotifyHighScoreUpdate(int highScoreP)
{
	m_scoreWidget.SetHighScore(highScoreP);
}

void HUDWidget::NotifyPlayerLifeUpdate(int newLivesP)
{
	m_livesWidget.SetLives(newLivesP);
}

void HUDWidget::NotifyLevelStart(int levelIndexP)
{
	m_scoreWidget.SetLevel(levelIndexP);
}