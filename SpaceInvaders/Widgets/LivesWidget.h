#pragma once
#include "Widget.h"

class LivesWidget : public Widget
{
public:
	LivesWidget(int coordXP = 25, int coordYP = 890, int widthP = 100, int heightP = 30, int livesP = 3);
	virtual void Draw() override;

	void SetLives(int livesP) { m_lives = livesP; }

private:
	int m_coordX{ 25 };
	int m_coordY{ 850 };
	int m_width{ 100 };
	int m_height{ 30 };
	int m_lives{ 3 };

	int m_livesOffset{ 5 }; // Offset between lives
};

