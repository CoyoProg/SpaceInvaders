#pragma once
#include "Widget.h"
#include "../Interfaces/IGameStateObserver.h"

#include <string>

class HUDWidget : public Widget, public IGameStateObserver
{
public:
	HUDWidget();

	virtual void Draw() override;
	virtual void OnScoreUpdate(int scoreP) override;
	virtual void OnLifeLost() override;

private:
	std::string scoreText{ "Score: 0" };
	int m_lives{ 3 };
	int m_highScore{ 0 };
};

