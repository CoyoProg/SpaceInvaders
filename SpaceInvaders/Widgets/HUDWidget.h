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
	virtual void NotifyScoreUpdate(int scoreP) override;
	virtual void NotifyHighScoreUpdate(int highScoreP) override;
	virtual void NotifyPlayerLifeUpdate(int newLivesP) override;
	virtual void NotifyLevelStart(int levelIndexP) override;

private:
	ScoreWidget m_scoreWidget;
	LivesWidget m_livesWidget;
};

