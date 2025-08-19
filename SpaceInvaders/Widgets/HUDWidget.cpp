#include "HUDWidget.h"
#include "../Core/GameManager.h"
#include "../Core/GameState.h"
#include "raylib.h"

HUDWidget::HUDWidget()
{
	// Initialize score text
	int score = 0;
	scoreText = (std::string("Score: ") + std::to_string(score));
	
	// Initialize lives and high score
	m_lives = 3; // Default value, can be updated later
	m_highScore = 0; // Default value, can be updated later
}

void HUDWidget::Draw()
{
	char const* score = scoreText.c_str();
	DrawText(score, 0 + 25,0 + 25, 20, GREEN);
}

void HUDWidget::OnScoreUpdate(int scoreP)
{
	scoreText = (std::string("Score: ") + std::to_string(scoreP));
}

void HUDWidget::OnLifeLost()
{
}


