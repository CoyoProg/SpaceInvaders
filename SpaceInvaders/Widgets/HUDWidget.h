#pragma once
#include "Widget.h"
#include "ScoreWidget.h"
#include "LivesWidget.h"
#include "../Interfaces/IGameStateObserver.h"

/*
 * @brief HUDWidget displays all UI elements related to the game state,
 */
class HUDWidget : public Widget, public IGameStateObserver
{
public:
	HUDWidget();

	virtual void Draw() override;
	virtual void OnScoreUpdate(int scoreP) override;
	virtual void OnLivesUpdate(int newLivesP) override;

private:
	ScoreWidget m_scoreWidget;
	LivesWidget m_livesWidget;
};

