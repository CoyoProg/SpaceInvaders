#pragma once

/*
 * @brief Interface for classes that observe changes in the game state
 */
class IGameStateObserver
{
public:
	virtual ~IGameStateObserver() = default;

	virtual void NotifyLevelStart(int levelIndexP) {};
	virtual void NotifyGameOver() {};
	virtual void NotifyScoreUpdate(int scoreP) {};
	virtual void NotifyHighScoreUpdate(int highScoreP) {};
	virtual void NotifyPlayerLifeUpdate(int newLivesP) {};
	virtual void NotifyPlayerDied() {};
	virtual void NotifyPlayerRespawn() {};
};

