#include "Invader.h"
#include "../Actors/Alien.h"
#include "../Core/GameManager.h"

#include "raymath.h"
#include <unordered_map>
#include <iostream>

Invader::Invader(int spaceBetweenRowsP)
{
	m_spaceBetweenRows = spaceBetweenRowsP;
}

void Invader::Update(float deltaSecP)
{
	if (m_aliens.empty()) return;

	UpdateAlienPosition(deltaSecP);

	UpdateShootProbability(deltaSecP);
}

void Invader::OnAlienDied(Alien& alienP)
{
	CleanupAliens();

	// Increment score
	// Increment speed ?
}

void Invader::CleanupAliens()
{
	for (int i = 0; i < static_cast<int>(m_aliens.size()); )
	{
		if (m_aliens[i]->IsMarkedForDeletion())
		{
			m_aliens.erase(m_aliens.begin() + i);

			// If the removed alien is before or at the current index,
			// decrement the current index to keep it pointing to the same "next to move" alien
			if (i <= m_currentAlienIndex)
			{
				--m_currentAlienIndex;
			}

			// Don't increment i because elements shifted left
		}
		else
		{
			++i;
		}
	}

	// Wrap around if currentAlienIndex goes negative
	if (m_currentAlienIndex < 0 && !m_aliens.empty())
	{
		m_currentAlienIndex = static_cast<int>(m_aliens.size() - 1);
	}
}

void Invader::AddAlien(std::shared_ptr<Alien> alienP)
{
	alienP->AddObserver(shared_from_this());
	m_aliens.emplace_back(std::move(alienP));

	m_currentAlienIndex = static_cast<int>(m_aliens.size() - 1);
}

void Invader::UpdateAlienPosition(float deltaSecP)
{
	m_delayMovementTimer += deltaSecP;

	// We only update the position of the aliens when the timer exceeds the movement delay
	// This is to stagger the movement of the aliens so they don't all move at the same time
	while (m_delayMovementTimer >= m_movementDelay)
	{
		m_delayMovementTimer -= m_movementDelay;

		Vector2 newPosition = m_aliens[m_currentAlienIndex]->GetPosition();

		if (!m_shouldChangeDirection)
		{
			// Move the current alien horizontally
			newPosition.x += m_distancePerStep * m_direction;
		}
		else
		{
			// Move down if we are changing direction during this step
			newPosition.y += m_aliens[m_currentAlienIndex]->GetSize().y;
		}

		m_aliens[m_currentAlienIndex]->SetPosition(newPosition);
		m_currentAlienIndex--;

		// If we have moved all the aliens, reset the index and check if we need to change direction
		if (m_currentAlienIndex < 0)
		{
			m_currentAlienIndex = static_cast<int>(m_aliens.size() - 1);

			if (m_shouldChangeDirection = ShouldChangeDirection())
			{
				m_direction = -m_direction;
			}
		}
	}
}

void Invader::UpdateShootProbability(float deltaSecP)
{
	m_shootTimer += deltaSecP;
	if (m_shootTimer >= 0.2f)
	{
		m_shootTimer = 0.0f;
		int chanceRoll = GetRandomValue(0, 1);

		if (chanceRoll)
		{
			GetRandomBottomAlien().ShootLaser();
		}
	}
}

Alien& Invader::GetRandomBottomAlien() const
{
	if (m_aliens.empty())
	{
		throw std::runtime_error("No aliens available");
	}

	if (m_aliens.size() == 1) return *m_aliens[0];

	std::unordered_map<int, Alien*> bottomsAliens;

	for (const auto& alien : m_aliens)
	{
		int col = alien->GetCoordX();

		// Check if we already have a bottom alien for this column
		auto iterator = bottomsAliens.find(col);
		if (iterator == bottomsAliens.end())
		{
			bottomsAliens[col] = alien.get();
		}
		else
		{
			// If we already have a bottom alien for this column, check if the current one is lower
			if (alien->GetCoordY() > iterator->second->GetCoordY())
			{
				iterator->second = alien.get();
			}
		}
	}

	// DEBUG VISUALIZATION
	//for (const auto& alien : bottomsAliens)
	//{
	//	alien.second->SetColor(WHITE, WHITE);
	//}

	// ##
	// Code source: https://stackoverflow.com/questions/27024269/select-random-element-in-an-unordered-map
	// Not really optimized, but works for our case because the number of aliens is small
	// ##
	auto randomIterator = bottomsAliens.begin();
	int randomIndex = rand() % bottomsAliens.size();
	std::advance(randomIterator, randomIndex);
	
	return *randomIterator->second;
}

bool Invader::ShouldChangeDirection() const
{
	for (auto& alien : m_aliens)
	{
		// Check if the next step will go out of bounds
		float nextStepPosition = alien->GetPosition().x + m_distancePerStep * m_direction;
		if (nextStepPosition > SCREEN_WIDTH - alien->GetSize().x || nextStepPosition < 0)
		{
			return true;
		}
	}

	return false;
}