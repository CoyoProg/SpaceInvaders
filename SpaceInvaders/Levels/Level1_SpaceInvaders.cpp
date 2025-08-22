#include "Level1_SpaceInvaders.h"
#include "../Core/GameManager.h"
#include "../Core/GameState.h"
#include "../Actors/Player.h"
#include "../Actors/Alien.h"
#include "../Actors/Shield.h"
#include "../Objects/Invader.h"

struct GradientPalette {
	Color center;
	Color edge;
};

static constexpr int PALETTE_COUNT = 8;
static constexpr GradientPalette LEVEL_PALETTES[] = {
	// Level 0
	{ Color{255, 192, 64, 255}, Color{255, 0, 0, 255} },
	// Level 1
	{ Color{255, 128, 128, 255}, Color{128, 64, 192, 255} },
	// Level 2
	{ Color{255, 255, 128, 255}, Color{192, 64, 64, 255} },
	// Level 3
	{ Color{255, 96, 96, 255}, Color{128, 0, 0, 255} },
	// Level 4
	{ Color{192, 128, 255, 255}, Color{64, 0, 128, 255} },
	// Level 5
	{ Color{128, 255, 224, 255}, Color{0, 128, 128, 255} },
	// Level 6
	{ Color{255, 224, 128, 255}, Color{128, 64, 0, 255} },
	// Level 7
	{ Color{255, 160, 192, 255}, Color{128, 0, 64, 255} },
};

Color LerpColor(const Color& colorAP, const Color& colorB, float distanceFromCenterP) {
	Color result;
	result.r = static_cast<unsigned char>(colorAP.r + (colorB.r - colorAP.r) * distanceFromCenterP);
	result.g = static_cast<unsigned char>(colorAP.g + (colorB.g - colorAP.g) * distanceFromCenterP);
	result.b = static_cast<unsigned char>(colorAP.b + (colorB.b - colorAP.b) * distanceFromCenterP);
	result.a = static_cast<unsigned char>(colorAP.a + (colorB.a - colorAP.a) * distanceFromCenterP);
	return result;
}

// Retrospection Note: Ideally, we could use object pooling instead of creating new actors every level.
// But because of the way the game manager removes dead actors, we will just create new ones for now.
void Level1_SpaceInvaders::InitializeLevel(GameManager& gameManagerP, GameState& gameStateP, int levelIndexP)
{
	m_currentLevel = levelIndexP;

	SpawnPlayer(gameManagerP);
	SpawnInvader(gameManagerP, gameStateP);

	// Reset the shields to have full one only for level 1 and 2
	if (m_currentLevel < 3)
	{
		for (std::weak_ptr<Shield>& shieldWeakPtr : m_shields)
		{
			if (std::shared_ptr<Shield> shield = shieldWeakPtr.lock())
			{
				shield->SetForDeletion(true);
			}
		}
		m_shields.clear();
		InitializeShields(gameManagerP);
	}

	InitializeAliensGrid(gameManagerP, *m_invader.lock());
}

void Level1_SpaceInvaders::SpawnPlayer(GameManager& gameManagerP)
{
	if (m_player.expired())
	{
		std::shared_ptr<Player> player = std::make_shared<Player>(m_playerData);
		m_player = player;
		gameManagerP.AddActor(player);
	}
}

void Level1_SpaceInvaders::SpawnInvader(GameManager& gameManagerP, GameState& gameStateP)
{
	int clampedLevel = static_cast<int>(m_currentLevel > LEVEL_CONFIG.size() ? LEVEL_CONFIG.size() : m_currentLevel);

	if (m_invader.expired())
	{
		std::shared_ptr<Invader> invader = std::make_shared<Invader>();
		invader->SetInvaderSettings(LEVEL_CONFIG.at(clampedLevel).alienMovementDelay, LEVEL_CONFIG.at(clampedLevel).alienShootProbability);
		m_invader = invader;
		gameManagerP.AddObject(invader);
		gameStateP.AddObserver(invader);
	}
	else
	{
		std::shared_ptr<Invader> invader = m_invader.lock();
		invader->SetInvaderSettings(LEVEL_CONFIG.at(clampedLevel).alienMovementDelay, LEVEL_CONFIG.at(clampedLevel).alienShootProbability);
	}
}

void Level1_SpaceInvaders::SetPlayerData(PlayerData playerDataP)
{
	m_playerData = playerDataP;
}

void Level1_SpaceInvaders::InitializeAliensGrid(GameManager& gameManagerP, Invader& invaderP)
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
			invaderP.AddAlien(alien);
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

		std::shared_ptr<Shield> shield = std::make_shared<Shield>(Vector2{ posX ,posY }, shieldSize, m_playerData.playerColor);
		gameManagerP.AddActor(shield);
		m_shields.emplace_back(shield);
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

	if (alienInfo.type == AnimatedSpriteID::AlienSmall)
	{
		// Slightly adjust the x position because there is some pixel missing in the sprite sheet
		position.x += 3.0f;
	}

	Color alienColor = ComputeAlienRadialColor(static_cast<float>(rowP), static_cast<float>(colP));
	std::shared_ptr<Alien> alien = std::make_shared<Alien>(position, size, colP, rowP, alienInfo.type, alienColor, alienInfo.score);

	int clampedLevel = static_cast<int>(m_currentLevel > LEVEL_CONFIG.size() ? LEVEL_CONFIG.size() : m_currentLevel);
	alien->SetLaserSpeed(LEVEL_CONFIG.at(clampedLevel).alienProjectileSpeed);

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
	if (colP < 0)
	{
		colP = 0;
	}
	else if (colP >= AlienGridConfig::COLUMN_NUMBER - 1)
	{
		colP = AlienGridConfig::COLUMN_NUMBER - 1;
	}

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

	// Pick palette based on level
	const GradientPalette& palette = LEVEL_PALETTES[(m_currentLevel - 1) % PALETTE_COUNT];

	// Interpolate from center to edge
	return LerpColor(palette.center, palette.edge, distFromCenter);
}

void Level1_SpaceInvaders::AssignAlienType(AlienInfo& alienInfoP, int row)
{
	switch (row)
	{
	case 0:
	{
		alienInfoP = { AnimatedSpriteID::AlienSmall,10 };
		break;
	}
	case 1:
	case 2:
	{
		alienInfoP = { AnimatedSpriteID::AlienMedium, 20 };
		break;
	}
	case 3:
	case 4:
	{
		alienInfoP = { AnimatedSpriteID::AlienLarge, 10 };
		break;
	}
	}
}
