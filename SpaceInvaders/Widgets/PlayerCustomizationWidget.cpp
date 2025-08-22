#include "PlayerCustomizationWidget.h"
#include "../Core/GameManager.h"
#include <raymath.h>

PlayerCustomizationWidget::PlayerCustomizationWidget()
{
	m_baseTexture = GameManager::GetInstance().GetTexture("playerBase");
	m_canonTexture = GameManager::GetInstance().GetTexture("playerCanon");

	float screenOffsetY = 100.0f;
	int spritePadding = 1;
	int scaleFactor = 3;

	m_position = {
		SCREEN_WIDTH / 2.0f,
		SCREEN_HEIGHT / 2.0f
	};

	m_basePosition;
	m_basePosition.x = m_position.x - (m_baseTexture.width * 3) / 2.0f;
	m_basePosition.y = m_position.y + screenOffsetY;

	m_canonPosition;
	m_canonPosition.x = m_position.x - (m_canonTexture.width * 3.0f) / 2.0f;
	m_canonPosition.y = m_position.y + screenOffsetY - (m_canonTexture.height * 3) + scaleFactor * spritePadding * 2;
}

void PlayerCustomizationWidget::Draw()
{
	Rectangle sourceRec = {
		1.0f,
		1.0f,
		static_cast<float>(m_baseTexture.width),
		static_cast<float>(m_baseTexture.height)
	};

	// Adjust the destination rectangle based on the scale factor
	Rectangle destRec = {
		m_basePosition.x,
		m_basePosition.y,
		sourceRec.width * 3,
		sourceRec.height * 3
	};
	DrawTexturePro(m_baseTexture, sourceRec, destRec, Vector2{ 0,0 }, 0.0f, m_playerColor);



	sourceRec = {
		1.0f,
		1.0f,
		static_cast<float>(m_canonTexture.width),
		static_cast<float>(m_canonTexture.height)
	};

	// Adjust the destination rectangle based on the scale factor
	destRec = {
		m_canonPosition.x,
		m_canonPosition.y,
		sourceRec.width * 3,
		sourceRec.height * 3
	};

	DrawTexturePro(m_canonTexture, sourceRec, destRec, Vector2{ 0,0 }, 0.0f, m_playerColor);
}

void PlayerCustomizationWidget::SelectNextColor()
{
	switch (m_enum)
	{
		case PlayerColor::PCW_GREEN:
			m_enum = PlayerColor::PCW_BLUE;
			m_playerColor = BLUE;
			break;
		case PlayerColor::PCW_BLUE:
			m_enum = PlayerColor::PCW_RED;
			m_playerColor = RED;
			break;
		case PlayerColor::PCW_RED:
			m_enum = PlayerColor::PCW_YELLOW;
			m_playerColor = YELLOW;
			break;
		case PlayerColor::PCW_YELLOW:
			m_enum = PlayerColor::PCW_WHITE;
			m_playerColor = WHITE;
			break;
		case PlayerColor::PCW_WHITE:
			m_enum = PlayerColor::PCW_PURPLE;
			m_playerColor = PURPLE;
			break;
		case PlayerColor::PCW_PURPLE:
			m_enum = PlayerColor::PCW_ORANGE;
			m_playerColor = ORANGE;
			break;
		case PlayerColor::PCW_ORANGE:
			m_enum = PlayerColor::PCW_CYAN;
			m_playerColor = DARKBLUE;
			break;
		case PlayerColor::PCW_CYAN:
			m_enum = PlayerColor::PCW_GREEN;
			m_playerColor = GREEN;
			break;
	}
}

void PlayerCustomizationWidget::SelectPreviousColor()
{
	switch (m_enum)
	{
		case PlayerColor::PCW_GREEN:
			m_enum = PlayerColor::PCW_CYAN;
			m_playerColor = DARKBLUE;
			break;
		case PlayerColor::PCW_BLUE:
			m_enum = PlayerColor::PCW_GREEN;
			m_playerColor = GREEN;
			break;
		case PlayerColor::PCW_RED:
			m_enum = PlayerColor::PCW_BLUE;
			m_playerColor = BLUE;
			break;
		case PlayerColor::PCW_YELLOW:
			m_enum = PlayerColor::PCW_RED;
			m_playerColor = RED;
			break;
		case PlayerColor::PCW_WHITE:
			m_enum = PlayerColor::PCW_YELLOW;
			m_playerColor = YELLOW;
			break;
		case PlayerColor::PCW_PURPLE:
			m_enum = PlayerColor::PCW_WHITE;
			m_playerColor = WHITE;
			break;
		case PlayerColor::PCW_ORANGE:
			m_enum = PlayerColor::PCW_PURPLE;
			m_playerColor = PURPLE;
			break;
		case PlayerColor::PCW_CYAN:
			m_enum = PlayerColor::PCW_ORANGE;
			m_playerColor = ORANGE;
			break;
	}
}
