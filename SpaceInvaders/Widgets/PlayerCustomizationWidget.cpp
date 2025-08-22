#include "PlayerCustomizationWidget.h"
#include "../Core/GameManager.h"
#include <raymath.h>

PlayerCustomizationWidget::PlayerCustomizationWidget()
{
	m_baseTexture = GameManager::GetInstance().GetTexture("baseD");
	m_canonTexture = GameManager::GetInstance().GetTexture("canonA");

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

void PlayerCustomizationWidget::SetBaseTexture(Texture2D baseTextureP)
{
	m_baseTexture = baseTextureP;

	m_basePosition;
	m_basePosition.x = m_position.x - (m_baseTexture.width * 3) / 2.0f;
	m_basePosition.y = m_position.y + screenOffsetY;
}

void PlayerCustomizationWidget::SetCanonTexture(Texture2D canonTextureP)
{
	m_canonTexture = canonTextureP;

	m_canonPosition;
	m_canonPosition.x = m_position.x - (m_canonTexture.width * 3.0f) / 2.0f;
	m_canonPosition.y = m_position.y + screenOffsetY - (m_canonTexture.height * 3) + scaleFactor * spritePadding * 2;
}

void PlayerCustomizationWidget::Draw()
{
	DrawBase();
	DrawCanon();
}

void PlayerCustomizationWidget::DrawCanon()
{
	Rectangle sourceRec = {
		1.0f,
		1.0f,
		static_cast<float>(m_canonTexture.width),
		static_cast<float>(m_canonTexture.height)
	};

	// Adjust the destination rectangle based on the scale factor
	Rectangle destRec = {
		m_canonPosition.x,
		m_canonPosition.y,
		sourceRec.width * 3,
		sourceRec.height * 3
	};

	DrawTexturePro(m_canonTexture, sourceRec, destRec, Vector2{ 0,0 }, 0.0f, m_playerColor);
}

void PlayerCustomizationWidget::DrawBase()
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
}
