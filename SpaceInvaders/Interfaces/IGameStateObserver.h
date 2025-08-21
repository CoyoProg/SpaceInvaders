#pragma once

/*
 * @brief Interface for classes that observe changes in the game state
 */
class IGameStateObserver
{
public:
	virtual ~IGameStateObserver() = default;
	virtual void OnGameOver() {};
	virtual void OnScoreUpdate(int scoreP) {};
	virtual void OnLivesUpdate(int newLivesP) {};
};

