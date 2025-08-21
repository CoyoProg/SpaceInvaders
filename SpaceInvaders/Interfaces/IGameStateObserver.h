#pragma once

/*
 * @brief Interface for classes that observe changes in the game state
 */
class IGameStateObserver
{
public:
	virtual ~IGameStateObserver() = default;
	virtual void NotifyGameOver() {};
	virtual void NotifyScoreUpdate(int scoreP) {};
	virtual void NotifyPlayerDied(int newLivesP) {};
	virtual void NotifyPlayerRespawn() {};
};

