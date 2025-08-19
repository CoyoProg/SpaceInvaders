#include "HUDWidget.h"
#include "../Core/GameManager.h"
#include "../Core/GameState.h"
#include "raylib.h"

HUDWidget::HUDWidget() : 
	m_scoreWidget(25, 25, 20),
	m_livesWidget(25, 895, 50, 20, 3)
{
}

void HUDWidget::Draw()
{
	// Draw the earth line
	DrawLine(0, SCREEN_HEIGHT - 45, SCREEN_WIDTH, SCREEN_HEIGHT - 45, GREEN);

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


