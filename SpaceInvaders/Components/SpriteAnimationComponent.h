#pragma once
#include "Component.h"
#include "raylib.h"

class SpriteAnimationComponent : public Component
{
public:
	void Draw(Vector2 positionP, Color colorP);

	void SetupSpriteAnimation(
		Texture2D spriteSheetP,
		float spriteWidthP,
		float spriteHeightP = 84.0f,
		float spriteOffsetP = 0.0f,
		float spritePaddingP = 10.0f,
		int maxFramesP = 1,
		float scaleFactorP = 1
	);

	void NextAnimationFrame();
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

