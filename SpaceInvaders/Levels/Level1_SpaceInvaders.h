#pragma once

#include <vector>
#include <memory>
#include "raylib.h"

class GameManager;
class Alien;
class Invader;

struct AlienGridConfig
{
	static constexpr Vector2 alienSize{ 64, 64 };
	static constexpr int spaceBetweenRows = 30;
	static constexpr int spaceBetweenCols = 18;
	static constexpr int columnNumber = 11;
	static constexpr int rowNumber = 5;
	static constexpr int topOffset = 100; // Offset from the top of the screen
};

class Level1_SpaceInvaders
{
public:
	Level1_SpaceInvaders(GameManager& gameManagerP);
	~Level1_SpaceInvaders();

private:
	void InitializeAliensGrid(GameManager& gameManagerP);
	void InitializeShields(GameManager& gameManagerP);

	std::shared_ptr<Alien> CreateAlien(int rowP, int colP, int horizontalMarginP = 0);
	Color CalculateGradientColor(int rowP, float colP);

private:
	std::unique_ptr<Invader> m_invader;
};

