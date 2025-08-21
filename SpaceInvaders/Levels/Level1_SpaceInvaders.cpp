#include "Level1_SpaceInvaders.h"
#include "../Core/GameManager.h"
#include "../Actors/Player.h"
#include "../Actors/Alien.h"
#include "../Actors/Shield.h"
#include "../Objects/Invader.h"

Level1_SpaceInvaders::Level1_SpaceInvaders(GameManager& gameManagerP)
{
	gameManagerP.AddActor(std::make_shared<Player>());
	m_invader = std::make_shared<Invader>();

	InitializeAliensGrid(gameManagerP);
	InitializeShields(gameManagerP);

	// Move the Invader ownership to the GameManager when the level is fully initialized
	gameManagerP.AddObject(m_invader);
}

void Level1_SpaceInvaders::OnGameOver()
{
	m_invader->RemoveAllAliens();
}

void Level1_SpaceInvaders::InitializeAliensGrid(GameManager& gameManagerP)
{
	// The grid is centered horizontally based on the total m_width of the aliens and the spaces between columns
	int totalGridWidth = AlienGridConfig::COLUMN_NUMBER * static_cast<int>(AlienGridConfig::ALIEN_SIZE.x) + (AlienGridConfig::COLUMN_NUMBER - 1) * AlienGridConfig::SPACE_BETWEEN_COLS;
	int horizontalMargin = (SCREEN_WIDTH - totalGridWidth) / 2;

	// Create the aliens in a grid pattern
	for (int row = 0; row < AlienGridConfig::ROW_NUMBER; ++row)
	{
		// Reverse the order of columns so that the bottom left alien is the last one created
		for (int col = AlienGridConfig::COLUMN_NUMBER - 1; col >= 0; --col)
		{
			std::shared_ptr<Alien> alien = CreateAlien(row, col, horizontalMargin);
			gameManagerP.AddActor(alien);
			m_invader->AddAlien(alien);
		}
	}
}

void Level1_SpaceInvaders::InitializeShields(GameManager& gameManagerP)
{
	int shieldNumber = 4;
	Vector2 shieldSize = { 120, 80 };
	int bottomOffset = 175;

	// Create the Shields
	for (int i = 0; i < shieldNumber; ++i)
	{
		float posX = static_cast<float>((i + 1) * (SCREEN_WIDTH / (shieldNumber + 1)) - shieldSize.x / 2);
		float posY = static_cast<float>(SCREEN_HEIGHT - bottomOffset);

		std::shared_ptr<Shield> shield = std::make_shared<Shield>(Vector2{ posX ,posY }, shieldSize);
		shield->SetColor(GREEN);
		gameManagerP.AddActor(shield);
	}
}

std::shared_ptr<Alien> Level1_SpaceInvaders::CreateAlien(int rowP, int colP, int horizontalMarginP)
{
	// Choose the alien type and score based on the row
	AlienInfo alienInfo;
	AssignAlienType(alienInfo, rowP);

	Vector2 position = {
		horizontalMarginP + colP * (AlienGridConfig::ALIEN_SIZE.x + AlienGridConfig::SPACE_BETWEEN_COLS),
		AlienGridConfig::TOP_OFFSET + rowP * (AlienGridConfig::ALIEN_SIZE.y + AlienGridConfig::SPACE_BETWEEN_ROWS)
	};

	Vector2 size = {
		SPRITE_PROPERTIES.at(alienInfo.type).width / 2.7f,
		SPRITE_PROPERTIES.at(alienInfo.type).height / 2.7f
	};

	if (alienInfo.type == SpriteID::AlienSmall)
	{
		// Slightly adjust the x position because there is some pixel missing in the sprite sheet
		position.x += 3.0f;
	}

	Color alienColor = ComputeAlienRadialColor(static_cast<float>(rowP), static_cast<float>(colP));
	std::shared_ptr<Alien> alien = std::make_shared<Alien>(position, size, colP, rowP, alienInfo.type, alienColor, alienInfo.score);

	return alien;
}

Color Level1_SpaceInvaders::ComputeAlienRadialColor(float rowP, float colP)
{
	// ##
	// Note: The following gradient logic comes from this website:
	// https://happycoding.io/tutorials/processing/for-loops/radial-gradient
	// Calculate the distance from the center of the grid to create a radial gradient effect.
	// ##

	// Clamp the column to ensure it is within the grid bounds
	if (colP < 0) colP = 0;
	if (colP >= AlienGridConfig::COLUMN_NUMBER - 1) colP = AlienGridConfig::COLUMN_NUMBER - 1;

	// Normalize the column and row indices to a range of 0 to 1
	float normalizeX = colP / (AlienGridConfig::COLUMN_NUMBER - 1);
	float normalizeY = rowP / (AlienGridConfig::ROW_NUMBER - 1);

	// The longest distance in the grid is from one corner to the center, which is sqrt(0.5) in a 0.5x0.5 square.
	float maxDistanceFromCenter = static_cast<float>(sqrt(0.5f));

	// Calculate the distance from center (0.5, 0.5)
	// sqrt((x-0.5)^2 + (y-0.5)^2)
	float dx = normalizeX - 0.5f;
	float dy = normalizeY - 0.5f;
	float distFromCenter = static_cast<float>(sqrt(dx * dx + dy * dy));
	distFromCenter /= maxDistanceFromCenter;

	Color result;
	result.r = static_cast<unsigned char>(255);
	result.g = static_cast<unsigned char>(192 * (1.0f - distFromCenter));
	result.b = static_cast<unsigned char>(64 * (1.0f - distFromCenter));
	result.a = 255;

	return result;
}

void Level1_SpaceInvaders::AssignAlienType(AlienInfo& alienInfoP, int row)
{
	switch (row)
	{
	case 0:
		alienInfoP = { SpriteID::AlienSmall,10 };
		break;
	case 1:
	case 2:
		alienInfoP = { SpriteID::AlienMedium, 20 };
		break;
	case 3:
	case 4:
		alienInfoP = { SpriteID::AlienLarge, 10 };
		break;
	}
}
