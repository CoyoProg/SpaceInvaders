#include "Level1_SpaceInvaders.h"
#include "../Core/GameManager.h"
#include "../Actors/LaserCanon.h"
#include "../Actors/Alien.h"
#include "raymath.h"

Level1_SpaceInvaders::Level1_SpaceInvaders(GameManager& gameManagerP)
{
	gameManagerP.AddActor(std::make_shared<LaserCanon>());

	InitializeAliensGrid(gameManagerP);
}

void Level1_SpaceInvaders::InitializeAliensGrid(GameManager& gameManagerP)
{
	// The grid is centered horizontally based on the total width of the aliens and the spaces between
	int totalGridWidth = AlienGridConfig::columnNumber * AlienGridConfig::alienSize.x + (AlienGridConfig::columnNumber - 1) * AlienGridConfig::spaceBetweenCols;
	int horizontalMargin = (SCREEN_WIDTH - totalGridWidth) / 2;

	for (int row = 0; row < AlienGridConfig::rowNumber; ++row)
	{
		// We reverse the order of columns so that the bottom left alien is the last one created
		for (int col = AlienGridConfig::columnNumber - 1; col >= 0; --col)
		{
			auto alien = CreateAlien(row, col, horizontalMargin);
			gameManagerP.AddActor(alien);
			m_aliens.emplace_back(alien);
		}
	}

	m_currentAlienIndex = m_aliens.size() - 1;
}

std::shared_ptr<Alien> Level1_SpaceInvaders::CreateAlien(int rowP, int colP, int horizontalMarginP)
{
	auto alien = std::make_shared<Alien>();

	alien->SetPosition(
		horizontalMarginP + colP * (AlienGridConfig::alienSize.x + AlienGridConfig::spaceBetweenCols),
		AlienGridConfig::topOffset + rowP * (AlienGridConfig::alienSize.y + AlienGridConfig::spaceBetweenRows)
	);
	alien->SetSize(AlienGridConfig::alienSize);

	Color previousColor = CalculateGradientColor(rowP, (colP + colP - 1) / 2);
	Color nextColor = CalculateGradientColor(rowP, (colP + colP + 1) / 2);

	alien->SetColor(previousColor, nextColor);

	return alien;
}

Color Level1_SpaceInvaders::CalculateGradientColor(int rowP, float colP)
{
	////
	//// Note: The following gradient logic comes from this website: https://happycoding.io/tutorials/processing/for-loops/corner-gradient
	//// We calculate the distance from each corner of the grid to create a gradient effect.
	////

	float normalizeX = (float)(colP) / (AlienGridConfig::columnNumber - 1);
	float normalizeY = (float)(rowP) / (AlienGridConfig::rowNumber - 1);

	float maxDist = sqrt(2.0f);

	float distanceFromTopLeft = sqrt(normalizeX * normalizeX + normalizeY * normalizeY);
	distanceFromTopLeft /= maxDist;

	float dx = normalizeX - 1.0f;
	float dy = normalizeY - 0.0f;
	float distanceFromTopRight = sqrt(dx * dx + dy * dy);
	distanceFromTopRight /= maxDist;

	dx = normalizeX - 0.0f;
	dy = normalizeY - 1.0f;
	float distanceFromBottomLeft = sqrt(dx * dx + dy * dy);
	distanceFromBottomLeft /= maxDist;

	Color result;
	result.r = (unsigned char)(255) * distanceFromBottomLeft;
	result.g = (unsigned char)(255) * distanceFromTopLeft;
	result.b = (unsigned char)(255) * distanceFromTopRight;
	result.a = 255;

	return result;
}

void Level1_SpaceInvaders::Update(float deltaSecP)
{
	m_delayMovementTimer += deltaSecP;

	// We use a delay timer so the aliens move in a staggered manner
	while (m_delayMovementTimer >= m_movementDelay)
	{
		m_delayMovementTimer -= m_movementDelay;

		UpdateCurrentAlienPosition();
		m_currentAlienIndex--;

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

			m_currentAlienIndex = m_aliens.size() - 1;

			if (m_shouldChangeDirection = ShouldChangeDirection())
			{
				m_direction = -m_direction;
			}
		}
	}
}

void Level1_SpaceInvaders::UpdateCurrentAlienPosition()
{
	Vector2 newPosition = m_aliens[m_currentAlienIndex]->GetPosition();
	newPosition.x += m_distancePerStep * m_direction;

	if (m_shouldChangeDirection)
	{
		// Move down
		newPosition.y += m_aliens[m_currentAlienIndex]->GetSize().y + AlienGridConfig::spaceBetweenRows;
	}

	m_aliens[m_currentAlienIndex]->SetPosition(newPosition.x, newPosition.y);
}

bool Level1_SpaceInvaders::ShouldChangeDirection() const
{
	for (auto& alien : m_aliens)
	{
		int nextStepPosition = alien->GetPosition().x + m_distancePerStep * m_direction;
		if (nextStepPosition > SCREEN_WIDTH - alien->GetSize().x || nextStepPosition < 0)
		{
			return true;
		}
	}

	return false;
}
