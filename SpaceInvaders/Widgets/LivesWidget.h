#pragma once
#include "Widget.h"

/*
 * @brief Widget that displays the number of lives remaining in the game.
 */
class LivesWidget : public Widget
{
public:
	LivesWidget(Vector2 positionP = { 25.0f, 890.f }, Vector2 sizeP = { 100.0f, 30.0f }, int livesP = 3);
	virtual void Draw() override;

	void SetLives(int livesP) { m_lives = livesP; }

private:
	Texture2D m_baseTexture{};
	Texture2D m_canonTexture{};
	Rectangle m_sourceBase{};
	Rectangle m_sourceCanon{};

	Vector2 m_size{ 100.0f, 30.0f };
	int m_lives{ 3 };
	int m_livesOffset{ 5 };
};

