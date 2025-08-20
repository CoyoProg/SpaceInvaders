#include "ScoreWidget.h"
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
}
