#include "SpriteAnimationComponent.h"

void SpriteAnimationComponent::SetupSpriteAnimation(Texture2D spriteSheetP, float spriteWidthP, float spriteHeightP, float spriteOffsetP, float spritePaddingP, int maxFramesP, float scaleFactorP)
{
	m_spriteSheet = spriteSheetP;
	m_spriteWidth = spriteWidthP;
	m_spriteHeight = spriteHeightP;
	m_spriteOffset = spriteOffsetP;
	m_spritePadding = spritePaddingP;
	m_maxFrames = maxFramesP;
	m_scaleFactor = scaleFactorP;
}

void SpriteAnimationComponent::Draw(Vector2 positionP, Color colorP)
{
	// Draw the texture if it is loaded
	if (m_spriteSheet.id != 0)
	{
		Rectangle sourceRec = {
			m_spriteOffset + m_spritePadding + m_currentFrame * (m_spritePadding + m_spriteWidth),
			m_spritePadding,
			m_spriteWidth,
			m_spriteHeight
		};

		// Adjust the destination rectangle based on the scale factor
		Rectangle destRec = {
			positionP.x,
			positionP.y,
			sourceRec.width / m_scaleFactor,
			sourceRec.height / m_scaleFactor
		};

		DrawTexturePro(m_spriteSheet, sourceRec, destRec, Vector2{ 0,0 }, 0.0f, colorP);
	}
}

void SpriteAnimationComponent::NextAnimationFrame()
{
	// Set the animation to the next frame
	// Or reset it if it has reached the maximum frame
	m_currentFrame = m_currentFrame == m_maxFrames ? 0 : m_currentFrame + 1;
}

void SpriteAnimationComponent::SetFrame(int frameIndexP)
{
	m_currentFrame = frameIndexP;
}
