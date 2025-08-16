#pragma once
#include <vector>
#include <memory>
#include "raylib.h"

class GameManager;
class Alien;

struct AlienGridConfig
{
	static constexpr Vector2 alienSize{ 64, 64 };
	static constexpr int spaceBetweenRows = 30;
	static constexpr int spaceBetweenCols = 18;
	static constexpr int columnNumber = 11;
	static constexpr int rowNumber = 5;
	static constexpr int topOffset = 100; // Offset from the top of the screen
};

struct AwesomeColor
{
	Color previousColor{ BLUE };
	Color nextColor{ BLUE };
};

class Level1_SpaceInvaders
{
public:
	Level1_SpaceInvaders(GameManager& gameManagerP);
	void Update(float deltaSecP);

private:
	void InitializeAliensGrid(GameManager& gameManagerP);
	std::shared_ptr<Alien> CreateAlien(int rowP, int colP, int horizontalMarginP = 0);
	Color CalculateGradientColor(int rowP, float colP);

	void UpdateCurrentAlienPosition();
	bool ShouldChangeDirection() const;

private:
	std::vector<std::shared_ptr<Alien>> m_aliens;

	int m_currentAlienIndex = 0;
	int m_direction = 1; // 1 for right, -1 for left
	const int m_distancePerStep = 5;
	bool m_shouldChangeDirection = false;
	float m_movementDelay = 0.02f;
	float m_delayMovementTimer = 0.0f;

	AwesomeColor myAwesomeColor{};
	Color startColor = { 255, 0, 0, 255 };
	Color endColor = { 0, 0, 255, 255 };
};

