#include "GameState.h"
#include "GameManager.h"
#include "UIManager.h"
#include "../Interfaces/IGameStateObserver.h"
#include "../Levels/Level1_SpaceInvaders.h"
#include "../Widgets/StartMenuWidget.h"
#include "../Widgets/HUDWidget.h"
#include "../Actors/Player.h"

// Needed to be able to forward decalre unique ptr
GameState::GameState() = default;
GameState::~GameState() = default;

void GameState::Update(float deltaSecP)
{
	if (m_freezeMovement)
	{
		m_freezeMovementTimer += deltaSecP;
		if (m_freezeMovementTimer < m_freezeMovementDuration)
		{
			return;
		}

		m_freezeMovement = false;
		m_freezeMovementTimer = 0.0f;
		OnPlayerRespawned();
	}
}

void GameState::LoadStartMenu()
{
	GameManager::GetInstance().AddWidget(std::move(std::make_unique<StartMenuWidget>()));
}

void GameState::StartLevel()
{
	GameManager::GetInstance().ResetAllWidgets();

	m_currentLevel = std::make_shared<Level1_SpaceInvaders>();
	m_currentLevel->InitializeLevel(GameManager::GetInstance(), *this);
	m_uiManager = std::make_unique<UIManager>(GameManager::GetInstance(), *this);

	GameManager::GetInstance().SetPauseGame(true);
}

void GameState::OnGameOver()
{
	GameManager::GetInstance().SetPauseGame(true);
}

void GameState::OnCounterFinished()
{
	GameManager::GetInstance().SetPauseGame(false);
}

void GameState::ResetLevel()
{
	GameManager::GetInstance().ResetAllActors();
	m_currentLevel->InitializeLevel(GameManager::GetInstance(), *this);

	GameManager::GetInstance().SetPauseGame(true);
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

		observerPtr->NotifyPlayerDied(m_lives);
		++it;
	}

	if (m_lives <= 5)
	{
		//m_isGameOver = true;

		//ResetLevel();
		//for (const std::shared_ptr<IGameStateObserver>& observer : m_observers)
		//{
		//	observer->NotifyGameOver();
		//}
	}
}

void GameState::OnPlayerRespawned()
{
	GameManager::GetInstance().AddActor(std::make_shared<Player>());

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
	if (observerP.expired()) return;
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
