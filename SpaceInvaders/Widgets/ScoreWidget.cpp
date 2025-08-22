#include "ScoreWidget.h"
#include "../Core/GameManager.h"

#include "raylib.h"

ScoreWidget::ScoreWidget(Vector2 positionP, int fontSizeP) :
	m_fontSize(fontSizeP)
{
	m_position = positionP;
}

void ScoreWidget::Draw()
{
	DrawText(TextFormat("Score: %01i", m_score), static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_fontSize, GREEN);
	DrawText(TextFormat("Level %01i", m_level), SCREEN_WIDTH / 2 - MeasureText("Level 0", m_fontSize) / 2, static_cast<int>(m_position.y), m_fontSize, GREEN);
	DrawText(TextFormat("High Score: %01i", m_highScore), SCREEN_WIDTH - MeasureText("High Score: 0000", m_fontSize) - static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_fontSize, RED);
}