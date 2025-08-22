#pragma once
#include "../Data/PlayerData.h"
#include <memory>
#include <vector>
#include <unordered_map>

class GameManager;
class GameState;
class Alien;
class Invader;
class Player;
class Shield;

struct AlienInfo {
	enum class AnimatedSpriteID type;
	int score;
};

struct AlienGridConfig
{
	static constexpr Vector2 ALIEN_SIZE{ 45, 33 };
	static constexpr int SPACE_BETWEEN_ROWS{ 30 };
	static constexpr int SPACE_BETWEEN_COLS{ 18 };
	static constexpr int COLUMN_NUMBER{ 11 };
	static constexpr int ROW_NUMBER{ 5 };
	static constexpr int TOP_OFFSET{ 150 };
};

struct LevelConfig
{
	float alienMovementDelay = 0.014f;
	int alienShootProbability = 70;
	int alienProjectileSpeed = 350;
};

static const std::unordered_map<int, LevelConfig> LEVEL_CONFIG
{
	{ 1, LevelConfig{} },
	{ 2, {0.010f, 80, 450} },
	{ 3, {0.005f, 90, 600} }
};

/*
 * @brief Represents the first level of the game
 */
class Level1_SpaceInvaders
{
public:
	void InitializeLevel(GameManager& gameManagerP, GameState& gameStateP, int levelIndexP = 1);
	void SpawnPlayer(GameManager& gameManagerP);
	void SpawnInvader(GameManager& gameManagerP, GameState& gameStateP);
	void SetPlayerData(PlayerData playerDataP);

private:
	// Initialize the aliens on the grid
	void InitializeAliensGrid(GameManager& gameManagerP, Invader& invaderP);
	// Initialize the shields at the bottom of the screen
	void InitializeShields(GameManager& gameManagerP);

	// Create an alien of a specific type depending on its row position
	std::shared_ptr<Alien> CreateAlien(int rowP, int colP, int horizontalMarginP = 0);
	// Compute the color of the alien based on its position from the center of the grid
	Color ComputeAlienRadialColor(float rowP, float colP);
	// Assign the alien type and score based on its row
	void AssignAlienType(AlienInfo& alienInfoP, int row);

private:
	PlayerData m_playerData{};
	std::weak_ptr<Player> m_player;
	std::weak_ptr<Invader> m_invader;
	std::vector<std::weak_ptr<Shield>> m_shields;
	int m_currentLevel{ 1 };
};

