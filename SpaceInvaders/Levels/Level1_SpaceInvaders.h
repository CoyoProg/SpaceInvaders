#pragma once
#include "../Interfaces/IGameStateObserver.h"

#include <memory>
#include "raylib.h"

class GameManager;
class Alien;
class Invader;

struct AlienInfo {
	enum class SpriteID type;
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

/*
 * @brief Represents the first level of the game
 */
class Level1_SpaceInvaders : public IGameStateObserver
{
public:
	Level1_SpaceInvaders(GameManager& gameManagerP);
	virtual void OnGameOver() override;

private:
	// Initialize the aliens on the grid
	void InitializeAliensGrid(GameManager& gameManagerP);
	// Initialize the shields at the bottom of the screen
	void InitializeShields(GameManager& gameManagerP);

	// Create an alien of a specific type depending on its row position
	std::shared_ptr<Alien> CreateAlien(int rowP, int colP, int horizontalMarginP = 0);
	// Compute the color of the alien based on its position from the center of the grid
	Color ComputeAlienRadialColor(float rowP, float colP);
	// Assign the alien type and score based on its row
	void AssignAlienType(AlienInfo& alienInfoP, int row);

private:
	std::shared_ptr<Invader> m_invader;
};

