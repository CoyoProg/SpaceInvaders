#pragma once
#include "Component.h"
#include "raylib.h"

/*
 * @brief Component that handles sprite animations.
 */
class SpriteAnimationComponent : public Component
{
public:
	void SetupSpriteAnimation(Texture2D spriteSheetP,
		float spriteWidthP,
		float spriteHeightP = 84.0f,
		float spriteOffsetP = 0.0f,
		float spritePaddingP = 10.0f,
		int maxFramesP = 1,
		float scaleFactorP = 1);
	void Draw(Vector2 positionP, Color colorP);

	// Set the animation to the next frame
	void NextAnimationFrame();

	// Set the animation to a specific frame
	void SetFrame(int frameIndexP);

private:
	Texture2D m_spriteSheet{};
	float m_scaleFactor{ 1.0f };
	float m_spriteOffset{ 0.0f };
	float m_spritePadding{ 10.0f };
	float m_spriteWidth{ 112.0f };
	float m_spriteHeight{ 84.0f };
	int m_currentFrame{ 0 };
	int m_maxFrames{ 1 };
};

