#include "GameState.h"
#include "../Interfaces/IGameStateObserver.h"
#include "GameManager.h"

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

		observerPtr->OnScoreUpdate(m_score);
		++it;
	}
}

void GameState::OnPlayerDied()
{
	m_lives--;

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

		observerPtr->OnLivesUpdate(m_lives);
		++it;
	}

	if (m_lives <= 5)
	{
		m_isGameOver = true;

		//GameManager::GetInstance().ResetLevel();
		//for (const std::shared_ptr<IGameStateObserver>& observer : m_observers)
		//{
		//	observer->OnGameOver();
		//}
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
