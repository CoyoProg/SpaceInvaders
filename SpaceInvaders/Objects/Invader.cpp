#include "Invader.h"
#include "../Actors/Alien.h"
#include "../Core/GameManager.h"

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
}

void Invader::CleanupAliens()
{
	for (int i = 0; i < static_cast<int>(m_aliens.size()); )
	{
		std::shared_ptr<Alien> alienPtr = m_aliens[i].lock();

		if (m_aliens[i].expired() || alienPtr->IsMarkedForDeletion())
		{
			m_aliens.erase(m_aliens.begin() + i);

			// If an alien that was removed was before (or at) the moving alien,
			// Decrement the alien moving index to keep moving to the same alien
			if (i <= m_alienToMoveIndex)
			{
				--m_alienToMoveIndex;
			}

			continue;
		}

		++i;
	}

	// If the first alien in the list was removed, reset to the last alien index
	if (m_alienToMoveIndex < 0 && !m_aliens.empty())
	{
		m_alienToMoveIndex = static_cast<int>(m_aliens.size() - 1);
	}
}

void Invader::AddAlien(std::shared_ptr<Alien> alienP)
{
	// Keep track when an alien dies by observing it
	alienP->AddObserver(shared_from_this());
	m_aliens.emplace_back(std::move(alienP));

	m_alienToMoveIndex = static_cast<int>(m_aliens.size() - 1);
}

void Invader::RemoveAllAliens()
{
	CleanupAliens();
}

void Invader::UpdateAlienPosition(float deltaSecP)
{
	m_delayMovementTimer += deltaSecP;

	// Update the position of only one alien at a time after the movement delay
	// This staggers their movement, making the aliens appear to move like a wave
	while (m_delayMovementTimer >= m_movementDelay)
	{
		std::shared_ptr<Alien> alienPtr = m_aliens[m_alienToMoveIndex].lock();
		m_delayMovementTimer -= m_movementDelay;

		Vector2 newPosition = alienPtr->GetPosition();

		// Move the alien horizontally in one direction
		// But if he's changing direction, move down instead
		if (!m_shouldChangeDirection)
		{
			newPosition.x += m_distancePerStep * m_direction;
		}
		else
		{
			newPosition.y += alienPtr->GetSize().y;
		}

		alienPtr->OnAlienMoved();
		alienPtr->SetPosition(newPosition);
		m_alienToMoveIndex--;

		// If all the aliens have moved, reset the index and check if we need to change direction the next step
		if (m_alienToMoveIndex < 0)
		{
			m_alienToMoveIndex = static_cast<int>(m_aliens.size() - 1);

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

	if (m_shootTimer >= m_shootCooldown)
	{
		m_shootTimer = 0.0f;

		// 70% chance of an alien shooting every m_shootCooldown
		// The chance increases when there are fewer aliens left
		int chanceRoll = rand() % 100;
		if (chanceRoll <= 70 + (11 - m_bottomAliensCount))
		{
			std::shared_ptr<Alien> alien = GetRandomBottomAlien();
			if (alien)
			{
				alien->ShootLaser();
			}
		}
	}
}

std::shared_ptr<Alien> Invader::GetRandomBottomAlien() const
{
	if (m_aliens.empty()) return nullptr;

	// An alien is added to the list only if it has a laser available
	if (m_aliens.size() == 1)
	{
		return m_aliens[0].lock()->IsLaserAvailable() ? m_aliens[0].lock() : nullptr;
	}

	std::unordered_map<int, std::shared_ptr<Alien>> bottomsAliens;
	for (const std::weak_ptr<Alien>& alien : m_aliens)
	{
		std::shared_ptr<Alien> alienPtr = alien.lock();
		int col = alienPtr->GetCoordX();

		// Check if a bottommost alien already exists in this column
		auto iterator = bottomsAliens.find(col);
		if (iterator != bottomsAliens.end())
		{
			if (alienPtr->GetCoordY() > iterator->second->GetCoordY())
			{
				iterator->second = alienPtr;
				continue;
			}
		}

		// Register this alien as the bottommost in its column
		bottomsAliens[col] = alienPtr;
	}

	for (auto it = bottomsAliens.begin(); it != bottomsAliens.end(); )
	{
		if (it->second->IsLaserAvailable())
		{
			++it;
			continue;
		}

		// erase() returns an iterator to the next element
		it = bottomsAliens.erase(it);
	}

	if (bottomsAliens.empty()) return nullptr;

	// Update the count of bottom aliens
	m_bottomAliensCount = static_cast<int>(bottomsAliens.size());

	// DEBUG VISUALIZATION
	//for (const auto& alien : bottomsAliens)
	//{
	//	alien.second->SetColor(WWHITE);
	//}

	// ##
	// Code source: https://stackoverflow.com/questions/27024269/select-random-element-in-an-unordered-map
	// Not really optimized, but works for our case because the number of aliens is small
	// ##
	auto randomIterator = bottomsAliens.begin();
	int randomIndex = rand() % m_bottomAliensCount;
	std::advance(randomIterator, randomIndex);

	return randomIterator->second;
}

bool Invader::ShouldChangeDirection() const
{
	for (const std::weak_ptr<Alien>& alien : m_aliens)
	{
		std::shared_ptr<Alien> alienPtr = alien.lock();

		// Check if the next step will go out of bounds
		// Aliens can go slightly out of playground bounds
		float nextStepPosition = alienPtr->GetPosition().x + m_distancePerStep * m_direction;
		if (nextStepPosition > SCREEN_WIDTH - PLAYGROUND_OFFSET / 2 - alienPtr->GetSize().x || nextStepPosition < 0 + PLAYGROUND_OFFSET / 2)
		{
			return true;
		}
	}

	return false;
}