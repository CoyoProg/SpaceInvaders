#include "HUDWidget.h"
#include "../Core/GameManager.h"

HUDWidget::HUDWidget() : 
	m_scoreWidget(Vector2{25.0f,25.0f}, 20),
	m_livesWidget(Vector2{ 25.0f,895.0f }, Vector2{ 50.0f,20.0f }, 3)
{
}

void HUDWidget::Draw()
{
	// Draw the playground boundaries
	DrawLine(
		PLAYGROUND_OFFSET,
		SCREEN_HEIGHT - 55,
		PLAYGROUND_OFFSET,
		SCREEN_HEIGHT - 45,
		GREEN
	);

	DrawLine(
		SCREEN_WIDTH - PLAYGROUND_OFFSET,
		SCREEN_HEIGHT - 55,
		SCREEN_WIDTH - PLAYGROUND_OFFSET,
		SCREEN_HEIGHT - 45,
		GREEN
	);

	m_scoreWidget.Draw();
	m_livesWidget.Draw();
}

void HUDWidget::OnScoreUpdate(int scoreP)
{
	m_scoreWidget.SetScore(scoreP);
}

void HUDWidget::OnLivesUpdate(int newLivesP)
{
	m_livesWidget.SetLives(newLivesP);
}


