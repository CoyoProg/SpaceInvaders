#include "ScoreWidget.h"
#include "raylib.h"

ScoreWidget::ScoreWidget(int coordXP, int coordYP, int fontSizeP) :
	m_coordX(coordXP),
	m_coordY(coordYP),
	m_fontSize(fontSizeP)
{
}

void ScoreWidget::Draw()
{
	// Code from: https://www.raylib.com/examples.html
	// Draw the score
	DrawText(TextFormat("Score: %01i", m_score), m_coordX, m_coordY, m_fontSize, GREEN);
}
