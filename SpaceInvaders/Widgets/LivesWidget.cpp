#include "LivesWidget.h"
#include "raylib.h"

LivesWidget::LivesWidget(Vector2 positionP, Vector2 sizeP, int livesP) :
	m_size(sizeP),
	m_lives(livesP)
{
	m_position = positionP;
}

void LivesWidget::Draw()
{
	// Draw the number of lives
	DrawText(TextFormat("%01i", m_lives), 10, static_cast<int>(m_position.y - m_size.y - 10), 20, GREEN);

	for (int i = 0; i < m_lives - 1; ++i)
	{
		// ##
		// TO DO: Replace with the actual player sprite
		// Draw each life as a rectangle
		DrawRectangleV(
			Vector2{ m_position.x + m_livesOffset + i * (m_size.x + m_livesOffset), m_position.y - m_size.y - 10 },
			m_size,
			GREEN
		);
		// ##
	}
}
