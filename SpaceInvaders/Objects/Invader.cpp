#include "Invader.h"
#include "../Actors/Alien.h"
#include "../Core/GameManager.h"
#include "raymath.h"

Invader::Invader(int spaceBetweenRowsP)
{
	m_spaceBetweenRows = spaceBetweenRowsP;
}

void Invader::Update(float deltaSecP)
{
	m_delayMovementTimer += deltaSecP;

	// We use a delay timer so the aliens move in a staggered manner 
	while (m_delayMovementTimer >= m_movementDelay)
	{
		m_delayMovementTimer -= m_movementDelay;

		// Move one alien at a time
		UpdateAlienPosition();
		m_currentAlienIndex--;

		// If we have moved all the aliens, reset the index and check if we need to change direction
		if (m_currentAlienIndex < 0)
		{
			// TO BE REMOVED
			// ####
			//m_movementDelay = m_movementDelay / 1.1; // Speed up the movement
			//if (m_movementDelay < 0.000001f) // Prevent it from going too fast
			//{
			//	m_movementDelay = 0.0001f;
			//}
			// ####

			m_currentAlienIndex = static_cast<int>(m_aliens.size() - 1);

			if (m_shouldChangeDirection = ShouldChangeDirection())
			{
				m_direction = -m_direction;
			}
		}
	}
}

void Invader::AddAlien(std::shared_ptr<Alien> alienP)
{
	m_aliens.emplace_back(std::move(alienP));

	m_currentAlienIndex = static_cast<int>(m_aliens.size() - 1);
}

void Invader::UpdateAlienPosition()
{
	Vector2 newPosition = m_aliens[m_currentAlienIndex]->GetPosition();
	newPosition.x += m_distancePerStep * m_direction;

	if (m_shouldChangeDirection)
	{
		// Move down
		newPosition.y += m_aliens[m_currentAlienIndex]->GetSize().y + m_spaceBetweenRows;
	}

	m_aliens[m_currentAlienIndex]->SetPosition(newPosition);
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
