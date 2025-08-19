#pragma once

#include <vector>
#include <memory>
#include "raylib.h"

class GameManager;
class Alien;
class Invader;

struct AlienGridConfig
{
	static constexpr Vector2 alienSize{ 45, 45 };
	static constexpr int spaceBetweenRows = 30;
	static constexpr int spaceBetweenCols = 18;
	static constexpr int columnNumber = 11;
	static constexpr int rowNumber = 5;
	static constexpr int topOffset = 150; // Offset from the top of the screen
};

class Level1_SpaceInvaders
{
public:
	Level1_SpaceInvaders(GameManager& gameManagerP);

private:
	void InitializeAliensGrid(GameManager& gameManagerP);
	void InitializeShields(GameManager& gameManagerP);

	std::shared_ptr<Alien> CreateAlien(int rowP, int colP, int horizontalMarginP = 0);
	Color CalculateGradientColor(float rowP, float colP);

private:
	std::shared_ptr<Invader> m_invader;
};

