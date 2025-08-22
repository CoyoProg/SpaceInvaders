#include "LivesWidget.h"
#include "raylib.h"
#include "../Core/GameState.h"

LivesWidget::LivesWidget(Vector2 positionP, Vector2 sizeP, int livesP) :
	m_size(sizeP),
	m_lives(livesP)
{
	// Apply the player canon textures from the game state to the lives logo
	m_baseTexture = GameState::GetInstance().GetPlayerCanonConfig().baseTexture;
	m_canonTexture = GameState::GetInstance().GetPlayerCanonConfig().canonTexture;

	m_size = Vector2{ static_cast<float>(m_baseTexture.width - 2), static_cast<float>(m_baseTexture.height + m_canonTexture.height - 4)};

	// Position the lives widget centered between the Earth line and the bottom of the screen
	m_position = positionP;
	m_position.y = 856 + (900 - 856 - m_size.y) / 2.0f;

	m_sourceBase = { 1.0f, 1.0f, static_cast<float>(m_baseTexture.width - 2), static_cast<float>(m_baseTexture.height - 2) };
	m_sourceCanon = { 1.0f, 1.0f, static_cast<float>(m_canonTexture.width - 2), static_cast<float>(m_canonTexture.height - 2) };
}

void LivesWidget::Draw()
{
	// Draw the number of lives
	DrawText(TextFormat("%01i", m_lives), 10, static_cast<int>(m_position.y + 5.0f), 20, GREEN);

	for (int i = 0; i < m_lives - 1; ++i)
	{
		Vector2 positionBase{ m_position.x + i * (m_size.x + m_livesOffset) + 5,  m_position.y + m_canonTexture.height - 4 };
		DrawTextureRec(m_baseTexture, m_sourceBase, positionBase, GREEN);
		
		Vector2 positionCanon{ m_position.x + i * (m_size.x + m_livesOffset) + m_baseTexture.width / 2.0f - m_canonTexture.width / 2.0f + 6, m_position.y };
		DrawTextureRec(m_canonTexture, m_sourceCanon, positionCanon, GREEN);
	}
}