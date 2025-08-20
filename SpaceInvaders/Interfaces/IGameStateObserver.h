#pragma once

/*
 * @brief Interface for classes that observe changes in the game state
 */
class IGameStateObserver
{
public:
	virtual ~IGameStateObserver() = default;
	virtual void OnScoreUpdate(int scoreP) = 0;
	virtual void OnLivesUpdate(int newLivesP) = 0;
};

