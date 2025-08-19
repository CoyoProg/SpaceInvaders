#pragma once
#include "Widget.h"
#include <string>

class ScoreWidget : public Widget
{
public:
	ScoreWidget(int coordXP = 25, int coordYP = 25, int fontSizeP = 20);
	virtual void Draw() override;

	void SetScore(int scoreP) { m_score = scoreP; }

private:
	int m_coordX{ 25 };
	int m_coordY{ 25 };
	int m_fontSize{ 20 };
	int m_score{ 0 };
};

