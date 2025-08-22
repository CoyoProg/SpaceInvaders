#include "GameState.h"
#include "GameManager.h"
#include "UIManager.h"
#include "../Interfaces/IGameStateObserver.h"
#include "../Levels/Level1_SpaceInvaders.h"
#include "../Widgets/StartMenuWidget.h"
#include "../Widgets/HUDWidget.h"
#include "../Actors/Player.h"

// Retrospection Note: Having so many time the same block of code to notify observers, for different events, is annoying,
// If I had more time I would have created a NotifyObservers method that takes a function as parameter

// Needed to be able to forward decalre unique ptr
GameState::GameState() = default;
GameState::~GameState() = default;

void GameState::Update(float deltaSecP)
{
	if (m_freezeMovement)
	{
		HandleTransitionTimer(deltaSecP);
	}
}

void GameState::HandleTransitionTimer(float deltaSecP)
{
	m_freezeMovementTimer += deltaSecP;
	if (m_freezeMovementTimer < m_freezeMovementDuration)
	{
		return;
	}

	if (m_isGameOver)
	{
		GameManager::GetInstance().ClearLevel();

		if (IsKeyPressed(KEY_SPACE))
		{
			ResetLevel();
			m_isGameOver = false;
			m_freezeMovement = false;
			m_freezeMovementTimer = 0.0f;
		}

		if (IsKeyPressed(KEY_X))
		{
			ReturnToMainMenu();
		}
	}
	else
	{
		m_freezeMovement = false;
		m_freezeMovementTimer = 0.0f;
		OnPlayerRespawned();
	}
}

void GameState::ReturnToMainMenu()
{
	GameManager::GetInstance().ClearLevel();
	GameManager::GetInstance().ClearAllWidgets();
	m_isGameOver = false;
	m_freezeMovement = false;
	m_freezeMovementTimer = 0.0f;
	LoadStartMenu();
}

void GameState::LoadStartMenu()
{
	std::shared_ptr<StartMenuWidget> startMenuWidget = std::make_shared<StartMenuWidget>();
	startMenuWidget->SetupWidgetBindings();
	GameManager::GetInstance().AddWidget(startMenuWidget);
}

void GameState::StartLevel(PlayerData playerDataP)
{

	// Clear the Start menu widgets
	GameManager::GetInstance().ClearAllWidgets();

	// Set the player data
	m_maxLives = playerDataP.lives;
	m_lives = m_maxLives;
	m_playerCanonConfig = PlayerCanonConfig{ playerDataP.baseTexture, playerDataP.canonTexture };

	// Initialize the first level
	m_currentLevel = std::make_shared<Level1_SpaceInvaders>();
	m_currentLevel->SetPlayerData(playerDataP);
	m_currentLevel->InitializeLevel(GameManager::GetInstance(), *this);

	// Initialize the HUD
	m_uiManager = std::make_unique<UIManager>(GameManager::GetInstance(), *this);

	// Pause the game until the countdown is finished
	GameManager::GetInstance().SetPauseGame(true);

	// Notify all observers that the level has started
	for (auto it = m_observers.begin(); it != m_observers.end(); )
	{
		// Check if the observer is still valid and erase it if not
		std::shared_ptr<IGameStateObserver>  observerPtr = it->lock();
		if (!observerPtr)
		{
			it = m_observers.erase(it);
			continue;
		}

		observerPtr->NotifyLevelStart(m_currentLevelIndex);
		observerPtr->NotifyPlayerLifeUpdate(m_lives);
		++it;
	}
}

void GameState::NextLevel()
{
	// Clear all projectiles from the previous level
	GameManager::GetInstance().ClearAllProjectiles();
	GameManager::GetInstance().SetPauseGame(true);

	// Initialize the next level with increased difficulty
	m_currentLevelIndex++;
	m_currentLevel->InitializeLevel(GameManager::GetInstance(), *this, m_currentLevelIndex);

	// Notify all observers that the level has started
	for (auto it = m_observers.begin(); it != m_observers.end(); )
	{
		// Check if the observer is still valid and erase it if not
		std::shared_ptr<IGameStateObserver>  observerPtr = it->lock();
		if (!observerPtr)
		{
			it = m_observers.erase(it);
			continue;
		}

		observerPtr->NotifyLevelStart(m_currentLevelIndex);
		++it;
	}
}

void GameState::OnGameOver()
{
	m_isGameOver = true;

	PlaySound(GameManager::GetInstance().GetSound("gameOver"));
	GameManager::GetInstance().SetPauseGame(true);

	// Update the highest score
	m_highestScore = m_score > m_highestScore ? m_score : m_highestScore;

	// Notify all observers about the game over and high score update
	for (auto it = m_observers.begin(); it != m_observers.end(); )
	{
		// Check if the observer is still valid and erase it if not
		std::shared_ptr<IGameStateObserver>  observerPtr = it->lock();
		if (!observerPtr)
		{
			it = m_observers.erase(it);
			continue;
		}

		observerPtr->NotifyGameOver();
		observerPtr->NotifyHighScoreUpdate(m_highestScore);
		++it;
	}
}

void GameState::OnCountdownFinished()
{
	GameManager::GetInstance().SetPauseGame(false);
}

void GameState::ResetLevel()
{
	// Reset to level 1
	m_currentLevel->InitializeLevel(GameManager::GetInstance(), *this);

	GameManager::GetInstance().SetPauseGame(true);

	// Reset player stats
	m_lives = m_maxLives;
	m_score = 0;
	m_currentLevelIndex = 1;

	for (auto it = m_observers.begin(); it != m_observers.end(); )
	{
		std::shared_ptr<IGameStateObserver>  observerPtr = it->lock();
		if (!observerPtr)
		{
			it = m_observers.erase(it);
			continue;
		}

		observerPtr->NotifyLevelStart(m_currentLevelIndex);
		observerPtr->NotifyPlayerLifeUpdate(m_lives);
		observerPtr->NotifyScoreUpdate(m_score);
		++it;
	}
}

void GameState::AddScore(int scoreP)
{
	m_score += scoreP;

	// Notify all observers about the score update
	for (auto it = m_observers.begin(); it != m_observers.end(); )
	{
		// Check if the observer is still valid and erase it if not
		std::shared_ptr<IGameStateObserver>  observerPtr = it->lock();
		if (!observerPtr)
		{
			it = m_observers.erase(it);
			continue;
		}

		observerPtr->NotifyScoreUpdate(m_score);
		++it;
	}
}

void GameState::OnPlayerDied()
{
	m_lives--;
	m_freezeMovement = true;

	// Notify all observers about the lives update
	for (auto it = m_observers.begin(); it != m_observers.end(); )
	{
		// Check if the observer is still valid and erase it if not
		std::shared_ptr<IGameStateObserver>  observerPtr = it->lock();
		if (!observerPtr)
		{
			it = m_observers.erase(it);
			continue;
		}

		observerPtr->NotifyPlayerLifeUpdate(m_lives);
		++it;
	}

	if (m_lives <= 0)
	{
		OnGameOver();
		return;
	}

	// Notify all observers that the player has died
	for (auto it = m_observers.begin(); it != m_observers.end(); )
	{
		// Check if the observer is still valid and erase it if not
		std::shared_ptr<IGameStateObserver>  observerPtr = it->lock();
		if (!observerPtr)
		{
			it = m_observers.erase(it);
			continue;
		}

		observerPtr->NotifyPlayerDied();
		++it;
	}
}

void GameState::OnPlayerRespawned()
{
	m_currentLevel->SpawnPlayer(GameManager::GetInstance());

	// Notify all observers that the player has respawned
	for (auto it = m_observers.begin(); it != m_observers.end(); )
	{
		// Check if the observer is still valid and erase it if not
		std::shared_ptr<IGameStateObserver>  observerPtr = it->lock();
		if (!observerPtr)
		{
			it = m_observers.erase(it);
			continue;
		}

		observerPtr->NotifyPlayerRespawn();
		++it;
	}
}

void GameState::AddObserver(const std::weak_ptr<IGameStateObserver> observerP)
{
	m_observers.push_back(observerP);
}

void GameState::RemoveObserver(const std::weak_ptr<IGameStateObserver> observerP)
{
	if (observerP.expired())
	{
		return;
	}

	std::shared_ptr<IGameStateObserver> observerPtr = observerP.lock();

	m_observers.erase(
		std::remove_if(
			m_observers.begin(),
			m_observers.end(),
			[&observerPtr](const std::weak_ptr<IGameStateObserver>& wptr) {
				auto sp = wptr.lock();
				return sp == observerPtr; // compare shared_ptrs
			}),
		m_observers.end()
	);
}
