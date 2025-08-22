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

	// Calculate the canon and base positions based on the initial texture sizes
	m_basePosition;
	m_basePosition.x = m_position.x - (m_baseTexture.width * m_scaleFactor) / 2.0f;
	m_basePosition.y = m_position.y + m_screenOffsetY;

	m_canonPosition;
	m_canonPosition.x = m_position.x - (m_canonTexture.width * m_scaleFactor) / 2.0f;
	m_canonPosition.y = m_position.y + m_screenOffsetY - (m_canonTexture.height * 3) + m_scaleFactor * m_spritePadding * 2;
}

void PlayerCustomizationWidget::SetBaseTexture(Texture2D baseTextureP)
{
	m_baseTexture = baseTextureP;

	// Recalculate base position with the new texture size
	m_basePosition;
	m_basePosition.x = m_position.x - (m_baseTexture.width * m_scaleFactor) / 2.0f;
	m_basePosition.y = m_position.y + m_screenOffsetY;
}

void PlayerCustomizationWidget::SetCanonTexture(Texture2D canonTextureP)
{
	m_canonTexture = canonTextureP;

	// Recalculate canon position with the new texture size
	m_canonPosition;
	m_canonPosition.x = m_position.x - (m_canonTexture.width * m_scaleFactor) / 2.0f;
	m_canonPosition.y = m_position.y + m_screenOffsetY - (m_canonTexture.height * m_scaleFactor) + m_scaleFactor * m_spritePadding * 2;
}

void PlayerCustomizationWidget::Draw()
{
	DrawBase();
	DrawCanon();
}

void PlayerCustomizationWidget::DrawCanon()
{
	// Scale up the canon texture
	Rectangle sourceRec = { 1.0f, 1.0f, static_cast<float>(m_canonTexture.width), static_cast<float>(m_canonTexture.height) };
	Rectangle destRec = { m_canonPosition.x, m_canonPosition.y, sourceRec.width * 3, sourceRec.height * 3 };

	DrawTexturePro(m_canonTexture, sourceRec, destRec, Vector2{ 0,0 }, 0.0f, m_playerColor);
}

void PlayerCustomizationWidget::DrawBase()
{
	// Scale up the base texture
	Rectangle sourceRec = { 1.0f, 1.0f, static_cast<float>(m_baseTexture.width), static_cast<float>(m_baseTexture.height) };
	Rectangle destRec = { m_basePosition.x, m_basePosition.y, sourceRec.width * m_scaleFactor, sourceRec.height * m_scaleFactor };

	DrawTexturePro(m_baseTexture, sourceRec, destRec, Vector2{ 0,0 }, 0.0f, m_playerColor);
}
