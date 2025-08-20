#pragma once
#include "Widget.h"

/*
 * @brief Widget that displays the player's score.
 */
class ScoreWidget : public Widget
{
public:
	ScoreWidget(Vector2 positionP = { 25.0f, 25.0f }, int fontSizeP = 20);
	virtual void Draw() override;

	void SetScore(int scoreP) { m_score = scoreP; }

private:
	const int m_fontSize{ 20 };
	int m_score{ 0 };
};

