#include "SpriteAnimationComponent.h"

void SpriteAnimationComponent::Draw(Vector2 positionP, Color colorP)
{
	if (m_spriteSheet.id != 0)
	{
		// If the texture is set, draw it
		Rectangle sourceRec = {
			m_spriteOffset + m_spritePadding + m_currentFrame * (m_spritePadding + m_spriteWidth),
			m_spritePadding,
			m_spriteWidth,
			m_spriteHeight
		};

		Rectangle destRec = {
			positionP.x,
			positionP.y,
			sourceRec.width / 2.7f,
			sourceRec.height / 2.7f
		}; // Scale downed by 2.7

		DrawTexturePro(m_spriteSheet, sourceRec, destRec, Vector2{ 0,0 }, 0.0f, colorP);
	}
}

void SpriteAnimationComponent::SetupSpriteAnimation(Texture2D spriteSheetP, float spriteWidthP, float spriteHeightP, float spriteOffsetP, float spritePaddingP, int maxFramesP)
{
	m_spriteSheet = spriteSheetP;
	m_spriteOffset = spriteOffsetP;
	m_spritePadding = spritePaddingP;
	m_spriteWidth = spriteWidthP;
	m_spriteHeight = spriteHeightP;
	m_maxFrames = maxFramesP;
}

void SpriteAnimationComponent::NextAnimationFrame()
{
	m_currentFrame = m_currentFrame == m_maxFrames ? 0 : m_currentFrame + 1;
}
