#include "LivesWidget.h"
#include "raylib.h"

LivesWidget::LivesWidget(int coordXP, int coordYP, int widthP, int heightP, int livesP) : 
	m_coordX(coordXP), 
	m_coordY(coordYP), 
	m_width(widthP), 
	m_height(heightP), 
	m_lives(livesP)
{

}

void LivesWidget::Draw()
{
	// Draw the earth line
	DrawRectangle(0, m_coordY - m_height - 20, 1200, 1, GREEN);

	// Code from: https://www.raylib.com/examples.html
	// Draw the number of lives
	DrawText(TextFormat("%01i", m_lives), 10, m_coordY - m_height - 10, 20, GREEN);

	for(auto i = 0; i < m_lives - 1; ++i)
	{
		// Draw each life as a rectangle
		DrawRectangle(m_coordX + m_livesOffset + i * (m_width + m_livesOffset), m_coordY - m_height - 10, m_width, m_height, GREEN);
	}
}
