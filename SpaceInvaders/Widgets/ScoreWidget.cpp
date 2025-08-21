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
	// Draw the score
	DrawText(TextFormat(
		"Score: %01i", m_score),
		static_cast<int>(m_position.x),
		static_cast<int>(m_position.y),
		m_fontSize,
		GREEN
	);

	// Draw the high score
	DrawText(TextFormat(
		"High Score: %01i", m_highScore),
		SCREEN_WIDTH - MeasureText("High Score: 0000", m_fontSize) - static_cast<int>(m_position.x),
		static_cast<int>(m_position.y),
		m_fontSize,
		RED
	);
}