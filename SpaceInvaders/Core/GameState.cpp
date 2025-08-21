#include "GameState.h"
#include "../Interfaces/IGameStateObserver.h"
#include "GameManager.h"

void GameState::AddScore(int scoreP)
{
	m_score += scoreP;

	for (const std::shared_ptr<IGameStateObserver>& observer : m_observers)
	{
		observer->OnScoreUpdate(m_score);
	}
}

void GameState::OnPlayerDied()
{
	m_lives--;

	for (const std::shared_ptr<IGameStateObserver>& observer : m_observers)
	{
		observer->OnLivesUpdate(m_lives);
	}

	if (m_lives <= 5)
	{
		m_isGameOver = true;
		GameManager::GetInstance().ResetLevel();
		for (const std::shared_ptr<IGameStateObserver>& observer : m_observers)
		{
			observer->OnGameOver();
		}
	}
}

void GameState::AddObserver(const std::shared_ptr<IGameStateObserver> observerP)
{
	m_observers.push_back(observerP);
}

void GameState::RemoveObserver(const std::shared_ptr<IGameStateObserver> observerP)
{
	m_observers.erase(
		std::remove(
			m_observers.begin(),
			m_observers.end(),
			observerP),
		m_observers.end()
	);
}
