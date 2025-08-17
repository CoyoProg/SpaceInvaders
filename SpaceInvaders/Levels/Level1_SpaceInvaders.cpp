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
	Vector2 position = {
		horizontalMarginP + colP * (AlienGridConfig::alienSize.x + AlienGridConfig::spaceBetweenCols),
		AlienGridConfig::topOffset + rowP * (AlienGridConfig::alienSize.y + AlienGridConfig::spaceBetweenRows)
	};

	Vector2 size = AlienGridConfig::alienSize;
	auto alien = std::make_shared<Alien>(position, size);

	Color previousColor = CalculateGradientColor(rowP, (colP + colP - 1) / 2);
	Color nextColor = CalculateGradientColor(rowP, (colP + colP + 1) / 2);

	alien->SetColor(previousColor, nextColor);

	return alien;
}

Color Level1_SpaceInvaders::CalculateGradientColor(int rowP, float colP)
{
	////
	//// Note: The following gradient logic comes from this website:
	//// https://happycoding.io/tutorials/processing/for-loops/corner-gradient
	//// https://happycoding.io/tutorials/processing/for-loops/radial-gradient
	//// We calculate the distance from each corner (or center) of the grid to create a gradient effect.
	////

	// TO DO: CHOOSE WHICH GRADIENT TO USE AND REMOVE THE OTHER ONE

	// Normalize the column and row indices to a range of 0 to 1
	float normalizeX = (float)(colP) / (AlienGridConfig::columnNumber - 1);
	float normalizeY = (float)(rowP) / (AlienGridConfig::rowNumber - 1);

	// The longest distance in the grid is from one corner to the opposite corner, which is sqrt(2) in a 1x1 square.
	float maxDist = sqrt(2.0f);

	// Calculate the distance from top left (0,0)
	// sqrt((x-0)^2 + (y-0)^2)
	float distanceFromTopLeft = sqrt(normalizeX * normalizeX + normalizeY * normalizeY);
	distanceFromTopLeft /= maxDist;

	// Calculate the distance from top right (1,0)
	// sqrt((x-1)^2 + (y-0)^2)
	float dx = normalizeX - 1.0f;
	float dy = normalizeY - 0.0f;
	float distanceFromTopRight = sqrt(dx * dx + dy * dy);
	distanceFromTopRight /= maxDist;

	// Calculate the distance from bottom left (0,1)
	// sqrt((x-0)^2 + (y-1)^2)
	dx = normalizeX - 0.0f;
	dy = normalizeY - 1.0f;
	float distanceFromBottomLeft = sqrt(dx * dx + dy * dy);
	distanceFromBottomLeft /= maxDist;

	// Calculate the distance from center (0.5, 0.5)
	// sqrt((x-0.5)^2 + (y-0.5)^2)
	float maxDistanceFromCenter = sqrt(0.5f); // The center is at (0.5, 0.5)
	dx = normalizeX - 0.5f;
	dy = normalizeY - 0.5f;
	float distFromCenter = sqrt(dx * dx + dy * dy);
	distFromCenter /= maxDistanceFromCenter;

	Color result;
	//result.r = (unsigned char)(255) * distanceFromBottomLeft;
	//result.g = (unsigned char)(255) * distanceFromTopLeft;
	//result.b = (unsigned char)(255) * distanceFromTopRight;

	result.r = static_cast<unsigned char>(255);									// strong red
	result.g = static_cast<unsigned char>(192 * (1.0f - distFromCenter));		// some green
	result.b = static_cast<unsigned char>(64 * (1.0f - distFromCenter));		// light up with some blue

	result.a = 255;

	return result;
}

// TO DO: MOVE TO INVADER CLASS THE LOGIC FOR ALIENS MOVEMENT
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
