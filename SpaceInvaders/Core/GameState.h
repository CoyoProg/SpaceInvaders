#pragma once
#include "../Interfaces/IUpdatable.h"
#include "../Data/PlayerData.h"
#include <memory>
#include <vector>
#include <functional>

class IGameStateObserver;
class UIManager;
class Level1_SpaceInvaders;

struct PlayerCanonConfig
{
	Texture2D baseTexture{};
	Texture2D canonTexture{};
};

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
	void HandleTransitionTimer(float deltaSecP);

	void ReturnToMainMenu();

	// Load the start menu
	void LoadStartMenu();
	// Load the first level
	void StartLevel(PlayerData playerDataP);
	void NextLevel();
	void ResetLevel();
	void OnGameOver();
	void OnCountdownFinished();

	// The score is updated everytime the player destroys an enemy
	void AddScore(int scoreP);

	void AddObserver(const std::weak_ptr<IGameStateObserver> observerP);
	void RemoveObserver(const std::weak_ptr<IGameStateObserver> observerP);

	void OnPlayerDied();
	void OnPlayerRespawned();

	PlayerCanonConfig GetPlayerCanonConfig() const { return m_playerCanonConfig; }

private:
	GameState();

private:
	std::vector<std::weak_ptr<IGameStateObserver>> m_observers;
	std::shared_ptr<Level1_SpaceInvaders> m_currentLevel;
	std::unique_ptr<UIManager> m_uiManager;
	PlayerCanonConfig m_playerCanonConfig;

	int m_currentLevelIndex{ 1 };
	int m_score{ 0 };
	int m_highestScore{ 0 };
	int m_maxLives{ 3 };
	int m_lives{ 3 };
	bool m_isGameOver{ false };
	bool m_freezeMovement{ false };
	float m_freezeMovementDuration{ 1.75f };
	float m_freezeMovementTimer{ 0.0f };
};

