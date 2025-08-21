#pragma once
#include "../Interfaces/IUpdatable.h"
#include <memory>
#include <vector>

class IGameStateObserver;
class UIManager;
class Level1_SpaceInvaders;

/*
 * @brief GameState is a singleton class that manages the game state, including score, lives, and game over status
 */
class GameState : public IUpdatable
{
public:
	// Singleton pattern to ensure only one instance of GameManager exists
	static GameState& GetInstance() {
		static GameState instance;
		return instance;
	}
	~GameState();

	// Delete copy and move constructors and assignment operators to prevent copying
	GameState(const GameState& other) = delete;
	GameState& operator=(const GameState& other) = delete;
	GameState(GameState&&) = delete;
	GameState& operator=(GameState&&) = delete;

	// Allow us to update the game state indenpendently of the game manager
	virtual void Update(float deltaSecP) override;

	// Load the start menu
	void LoadStartMenu();
	// Load the first level
	void StartLevel();
	void ResetLevel();
	void OnGameOver();
	void OnCounterFinished();

	// The score is updated everytime the player destroys an enemy
	void AddScore(int scoreP);
	void AddObserver(const std::weak_ptr<IGameStateObserver> observerP);
	void RemoveObserver(const std::weak_ptr<IGameStateObserver> observerP);

	void OnPlayerDied();
	void OnPlayerRespawned();

private:
	GameState();

private:
	std::vector<std::weak_ptr<IGameStateObserver>> m_observers;
	std::shared_ptr<Level1_SpaceInvaders> m_currentLevel;
	std::unique_ptr<UIManager> m_uiManager;

	int m_score{ 0 };
	int m_lives{ 3 };
	bool m_isGameOver{ false };
	bool m_freezeMovement{ false };
	float m_freezeMovementDuration{ 3.0f };
	float m_freezeMovementTimer{ 0.0f };
};

