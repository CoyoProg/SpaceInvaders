#include "Level1_SpaceInvaders.h"
#include "../Core/GameManager.h"
#include "../Actors/Player.h"
#include "../Actors/Alien.h"
#include "../Actors/Shield.h"
#include "../Objects/Invader.h"
#include "../Components/SpriteAnimationComponent.h"

#include "raymath.h"

Level1_SpaceInvaders::Level1_SpaceInvaders(GameManager& gameManagerP)
{
	gameManagerP.AddActor(std::make_shared<Player>());
	m_invader = std::make_shared<Invader>();
	m_alienSheet = gameManagerP.GetTexture("alienSheet");

	InitializeAliensGrid(gameManagerP);
	InitializeShields(gameManagerP);

	// Move the Invader ownership to the GameManager when the level is initialized
	gameManagerP.AddObject(std::move(m_invader));
}

void Level1_SpaceInvaders::InitializeAliensGrid(GameManager& gameManagerP)
{
	// The grid is centered horizontally based on the total width of the aliens and the spaces between
	int totalGridWidth = AlienGridConfig::columnNumber * static_cast<int>(AlienGridConfig::alienSize.x) + (AlienGridConfig::columnNumber - 1) * AlienGridConfig::spaceBetweenCols;
	int horizontalMargin = (SCREEN_WIDTH - totalGridWidth) / 2;

	// Create the aliens in a grid pattern
	for (int row = 0; row < AlienGridConfig::rowNumber; ++row)
	{
		// We reverse the order of columns so that the bottom left alien is the last one created
		for (int col = AlienGridConfig::columnNumber - 1; col >= 0; --col)
		{
			auto alien = CreateAlien(row, col, horizontalMargin);
			gameManagerP.AddActor(alien);
			m_invader->AddAlien(alien);
		}
	}
}

void Level1_SpaceInvaders::InitializeShields(GameManager& gameManagerP)
{
	int shieldWidth = 120;
	int shieldHeight = 80;

	// Create the Shields
	for (int i = 0; i < 4; ++i)
	{
		float posX = static_cast<float>((i + 1) * (SCREEN_WIDTH / 5) - shieldWidth / 2);
		float posY = static_cast<float>(SCREEN_HEIGHT - 175);

		auto shield = std::make_shared<Shield>(Vector2{ posX ,posY }, shieldWidth, shieldHeight);
		shield->SetColor(GREEN);
		gameManagerP.AddActor(shield);
	}
}

std::shared_ptr<Alien> Level1_SpaceInvaders::CreateAlien(int rowP, int colP, int horizontalMarginP)
{
	SpriteID alienType = SpriteID::AlienMedium;
	float offSetX = 0.0f;
	float padding = 10.0f;

	switch (rowP)
	{
	case 0:
		alienType = SpriteID::AlienSmall;
		offSetX = (SPRITE_PROPERTIES.at(SpriteID::AlienMedium).maxFrameIndex + 1) * (padding + SPRITE_PROPERTIES.at(SpriteID::AlienMedium).width);
		break;
	case 1:
	case 2:
		alienType = SpriteID::AlienMedium;
		break;
	case 3:
	case 4:
		alienType = SpriteID::AlienLarge;
		offSetX =
			(SPRITE_PROPERTIES.at(SpriteID::AlienMedium).maxFrameIndex + 1) * (padding + SPRITE_PROPERTIES.at(SpriteID::AlienMedium).width) +
			(SPRITE_PROPERTIES.at(SpriteID::AlienSmall).maxFrameIndex + 1) * (padding + SPRITE_PROPERTIES.at(SpriteID::AlienSmall).width);
		break;
	}

	Vector2 position = {
		horizontalMarginP + colP * (AlienGridConfig::alienSize.x + AlienGridConfig::spaceBetweenCols),
		AlienGridConfig::topOffset + rowP * (AlienGridConfig::alienSize.y + AlienGridConfig::spaceBetweenRows)
	};

	if (alienType == SpriteID::AlienSmall)
	{
		// Slightly adjust the x position because there is some pixel missing in the sprite sheet
		position.x += 3.0f;
	}

	Vector2 size = Vector2{
		SPRITE_PROPERTIES.at(alienType).width / 2.7f,
		SPRITE_PROPERTIES.at(alienType).height / 2.7f
	};

	auto alien = std::make_shared<Alien>(position, size, colP, rowP);

	alien->SetColor(CalculateGradientColor(static_cast<float>(rowP), static_cast<float>(colP)));
	alien->SetupSpriteAnimationComponent(
		m_alienSheet,
		SPRITE_PROPERTIES.at(alienType).width,
		SPRITE_PROPERTIES.at(alienType).height,
		offSetX,
		10.0f,
		1,
		2.7f
	);
	return alien;
}

Color Level1_SpaceInvaders::CalculateGradientColor(float rowP, float colP)
{
	// ##
	// Note: The following gradient logic comes from this website:
	// https://happycoding.io/tutorials/processing/for-loops/corner-gradient
	// https://happycoding.io/tutorials/processing/for-loops/radial-gradient
	// We calculate the distance from the center of the grid to create a gradient effect.
	// ##

	if (colP < 0) colP = 0;
	if (colP >= AlienGridConfig::columnNumber - 1) colP = AlienGridConfig::columnNumber - 1;

	// Normalize the column and row indices to a range of 0 to 1
	float normalizeX = colP / (AlienGridConfig::columnNumber - 1);
	float normalizeY = rowP / (AlienGridConfig::rowNumber - 1);

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