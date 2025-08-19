#pragma once
#include <memory>
#include <vector>

class IGameStateObserver;

class GameState
{
public:
	// Singleton pattern to ensure only one instance of GameManager exists
	static GameState& GetInstance() {
		static GameState instance;
		return instance;
	}

	// Delete copy and move constructors and assignment operators to prevent copying
	GameState(const GameState& other) = delete;
	GameState& operator=(const GameState& other) = delete;
	GameState(GameState&&) = delete;
	GameState& operator=(GameState&&) = delete;

	void AddScore(int scoreP);
	void AddObserver(const std::shared_ptr<IGameStateObserver> observerP);
	void RemoveObserver(const std::shared_ptr<IGameStateObserver> observerP);

	void OnPlayerDied();

private:
	GameState();

private:
	std::vector<std::shared_ptr<IGameStateObserver>> m_observers;

	int m_score = 0;
	int m_lives = 3;
	bool m_isGameOver = false;
	bool m_isGamePaused = false;
};

