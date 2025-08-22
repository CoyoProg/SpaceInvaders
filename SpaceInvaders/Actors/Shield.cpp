#include "Shield.h"
#include "../Components/CollisionBoxComponent.h"

#include <cmath>
#include <raymath.h>

// Small pattern for the shield: 1 = alive, 0 = empty
static constexpr int PATTERN_WIDTH = 12;
static constexpr int PATTERN_HEIGHT = 8;
static constexpr int SHIELD_PATTERN[PATTERN_HEIGHT][PATTERN_WIDTH] = {
	{0,0,1,1,1,1,1,1,1,1,0,0},
	{0,1,1,1,1,1,1,1,1,1,1,0},
	{1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1,1,1,1,1},
	{1,1,1,1,1,0,0,1,1,1,1,1},
	{1,1,1,1,0,0,0,0,1,1,1,1},
};

// We want to scale up the pattern to have a better collision detection
static constexpr int UPSCALED_WIDTH = 24;
static constexpr int UPSCALED_HEIGHT = 16;

constexpr int EXPLOSION_SIZE = 6;
constexpr int EXPLOSION_MASKS_COUNT = 2;
constexpr int EXPLOSION_MASKS[EXPLOSION_MASKS_COUNT][EXPLOSION_SIZE][EXPLOSION_SIZE] = {
	{
		{0,0,1,1,0,0},
		{0,1,1,1,0,0},
		{0,1,1,1,1,0},
		{0,0,1,1,1,0},
		{0,0,1,1,0,0},
		{0,0,1,1,0,0},
	},
	{
		{0,0,1,1,1,1},
		{0,1,1,1,1,1},
		{1,1,1,1,1,1},
		{1,1,1,1,1,0},
		{1,1,1,1,1,0},
		{1,1,1,1,0,1},
	}
};

Shield::Shield(Vector2 positionP, Vector2 sizeP) : Actor(ActorAffiliation::Neutral)
{
	m_position = positionP;
	m_size =sizeP;

	m_cellSize.x = m_size.x / static_cast<float>(UPSCALED_WIDTH);
	m_cellSize.y = m_size.y / static_cast<float>(UPSCALED_HEIGHT);

	ResizePatternToCells();
	RemoveExtraCells();
	UpdateTrianglesCells();

	if (m_CollisionBoxComponent)
	{
		m_CollisionBoxComponent->SetSize(m_size);
		m_CollisionBoxComponent->SetPosition(m_position);
	}

	m_hasComplexCollision = true;
}

void Shield::ResizePatternToCells()
{
	// Scale the shield pattern to fit the upscaled grid
	int scaleX = UPSCALED_WIDTH / PATTERN_WIDTH;
	int scaleY = UPSCALED_HEIGHT / PATTERN_HEIGHT;

	// ## 
	// Source code: 
	// https://stackoverflow.com/questions/32846846/quick-way-to-upsample-numpy-array-by-nearest-neighbor-tiling
	// ##
	cells.resize(UPSCALED_HEIGHT, std::vector<int>(UPSCALED_WIDTH, 0));
	for (int py = 0; py < PATTERN_HEIGHT; ++py)
	{
		for (int px = 0; px < PATTERN_WIDTH; ++px)
		{
			if (SHIELD_PATTERN[py][px] != 0)
			{
				for (int iy = 0; iy < scaleY; ++iy)
				{
					for (int ix = 0; ix < scaleX; ++ix)
					{
						int y = py * scaleY + iy;
						int x = px * scaleX + ix;
	
						cells[y][x] = SHIELD_PATTERN[py][px];
					}
				}
			}
		}
	}
}

void Shield::RemoveExtraCells()
{
	for (int y = 0; y < UPSCALED_HEIGHT; ++y)
	{
		for (int x = 0; x < UPSCALED_WIDTH; ++x)
		{
			if (cells[y][x] != 1) continue;

			bool topLeft = y > 0 && cells[y - 1][x] == 0 && x > 0 && cells[y][x - 1] == 0
				|| y > 0 && cells[y - 1][x] == 0 && x <= 0
				|| x > 0 && cells[y][x - 1] == 0 && y <= 0;

			bool topRight = y > 0 && cells[y - 1][x] == 0 && x < UPSCALED_WIDTH - 1 && cells[y][x + 1] == 0
				|| y > 0 && cells[y - 1][x] == 0 && x >= UPSCALED_WIDTH - 1
				|| x < UPSCALED_WIDTH - 1 && cells[y][x + 1] == 0 && y <= 0;

			bool bottomLeft = y < UPSCALED_HEIGHT - 1 && cells[y + 1][x] == 0 && x < UPSCALED_WIDTH - 1 && cells[y][x + 1] == 0
				|| y < UPSCALED_HEIGHT - 1 && cells[y + 1][x] == 0 && x >= UPSCALED_WIDTH - 1
				|| x < UPSCALED_WIDTH - 1 && cells[y][x + 1] == 0 && y >= UPSCALED_HEIGHT - 1;

			bool bottomRight = y < UPSCALED_HEIGHT - 1 && cells[y + 1][x] == 0 && x > 0 && cells[y][x - 1] == 0
				|| x > 0 && cells[y][x - 1] == 0 && y >= UPSCALED_HEIGHT - 1
				|| y < UPSCALED_HEIGHT - 1 && cells[y + 1][x] == 0 && x <= 0;

			if (topLeft || topRight || bottomLeft || bottomRight)
			{
				cells[y][x] = -1;
			}
		}
	}
}

void Shield::UpdateTrianglesCells()
{
	for (int y = 0; y < UPSCALED_HEIGHT; ++y)
	{
		for (int x = 0; x < UPSCALED_WIDTH; ++x)
		{
			if (cells[y][x] == 1)
			{
				// Check Top-left diagonal
				bool topLeft = y > 0 && cells[y - 1][x] <= 0 && x > 0 && cells[y][x - 1] <= 0
					|| y > 0 && cells[y - 1][x] <= 0 && x <= 0
					|| x > 0 && cells[y][x - 1] <= 0 && y <= 0;

				// Check Top-right diagonal
				bool topRight = y > 0 && cells[y - 1][x] <= 0 && x < UPSCALED_WIDTH - 1 && cells[y][x + 1] <= 0
					|| y > 0 && cells[y - 1][x] <= 0 && x >= UPSCALED_WIDTH - 1
					|| x < UPSCALED_WIDTH - 1 && cells[y][x + 1] <= 0 && y <= 0;

				// Check Bottom-right diagonal
				bool bottomRight = y < UPSCALED_HEIGHT - 1 && cells[y + 1][x] <= 0 && x > 0 && cells[y][x - 1] <= 0
					|| x > 0 && cells[y][x - 1] <= 0 && y >= UPSCALED_HEIGHT - 1
					|| y < UPSCALED_HEIGHT - 1 && cells[y + 1][x] <= 0 && x <= 0;

				// Check Bottom-left diagonal
				bool bottomLeft = y < UPSCALED_HEIGHT - 1 && cells[y + 1][x] <= 0 && x < UPSCALED_WIDTH - 1 && cells[y][x + 1] <= 0
					|| y < UPSCALED_HEIGHT - 1 && cells[y + 1][x] <= 0 && x >= UPSCALED_WIDTH - 1
					|| x < UPSCALED_WIDTH - 1 && cells[y][x + 1] <= 0 && y >= UPSCALED_HEIGHT - 1;

				if (topLeft + bottomRight + topRight + bottomLeft > 1)
				{
					// If more than one diagonal is true, we want to keep the cell as a square
					cells[y][x] = 1;
				}
				else if (topLeft) cells[y][x] = 2;
				else if (topRight) cells[y][x] = 4;
				else if (bottomRight) cells[y][x] = 5;
				else if (bottomLeft) cells[y][x] = 3;
			}
		}
	}
}

void Shield::Draw()
{
	for (int y = 0; y < UPSCALED_HEIGHT; y++) {
		for (int x = 0; x < UPSCALED_WIDTH; x++)
		{
			float drawX = m_position.x + x * m_cellSize.x;
			float drawY = m_position.y + y * m_cellSize.y;

			switch (cells[y][x])
			{
			case 1:
				DrawRectangle(
					static_cast<int>(drawX),
					static_cast<int>(drawY),
					static_cast<int>(m_cellSize.x),
					static_cast<int>(m_cellSize.y),
					GREEN);
				break;
			case 2:
				DrawTriangle(
					{ drawX, drawY + m_cellSize.y },
					{ drawX + m_cellSize.x, drawY + m_cellSize.y },
					{ drawX + m_cellSize.x, drawY },
					GREEN
				);
				break;
			case 3:
				DrawTriangle(
					{ drawX, drawY },
					{ drawX, drawY + m_cellSize.y },
					{ drawX + m_cellSize.x, drawY },
					GREEN
				);
				break;
			case 4:
				DrawTriangle(
					{ drawX, drawY + m_cellSize.y },
					{ drawX + m_cellSize.x, drawY + m_cellSize.y },
					{ drawX, drawY },
					GREEN
				);
				break;

			case 5:
				DrawTriangle(
					{ drawX + m_cellSize.x, drawY + m_cellSize.y },
					{ drawX + m_cellSize.x, drawY },
					{ drawX, drawY },
					GREEN
				);
				break;
			}
		}
	}
}

bool Shield::AdvancedCollidesWith(const Actor& otherActorP) const
{
	Vector2 actorPosition = otherActorP.GetPosition();
	Vector2 actorSize = otherActorP.GetSize();
	Vector2 gridCoords = ConvertWorldToGrid(actorPosition);

	int minX = static_cast<int>(gridCoords.x);
	int maxX = static_cast<int>(gridCoords.x + actorSize.x / m_cellSize.x);
	int minY = static_cast<int>(gridCoords.y);
	int maxY = static_cast<int>(gridCoords.y + actorSize.y / m_cellSize.y);

	// Check if the actor is colliding with cells that are not empty
	for(int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			if (x < 0 || x >= UPSCALED_WIDTH || y < 0 || y >= UPSCALED_HEIGHT) continue;

			if (cells[y][x] != 0)
			{
				m_lastHitLocation = Vector2{ static_cast<float>(y), static_cast<float>(x) };
				return true;
			}
		}
	}

	return false;
}

void Shield::OnCollisionEvent(const Actor& otherActorP)
{
	ExplodeCell(m_lastHitLocation);
}

void Shield::ExplodeCell(Vector2 hitPositionP)
{
	// ##
	// TO DO: Refactor
	// Random pattern cell explosion (different patterns of different sizes)
	// Random outer radius cell explosion (break the pattern on the edges)
	// Random outer radius jitter explosion
	// Reverse pattern depending on the hit direction
	// ##

	Vector2 gridPos = { hitPositionP.y, hitPositionP.x };

	// Pick a random mask
	int maskIndex = rand() % EXPLOSION_MASKS_COUNT;
	for (int y = 0; y < EXPLOSION_SIZE; ++y)
	{
		for (int x = 0; x < EXPLOSION_SIZE; ++x)
		{
			if (EXPLOSION_MASKS[maskIndex][y][x] == 1)
			{
				int gx = static_cast<int>(gridPos.x + x - EXPLOSION_SIZE / 2);
				int gy = static_cast<int>(gridPos.y + y - EXPLOSION_SIZE / 2);

				if (gx >= 0 && gx < UPSCALED_WIDTH &&
					gy >= 0 && gy < UPSCALED_HEIGHT)
				{
					cells[gy][gx] = 0; // destroy this cell
				}
			}
		}
	}

	int radius = 4;
	
	for (int dy = -radius; dy <= radius; ++dy)
	{
		for (int dx = -radius; dx <= radius; ++dx)
		{
			int x = static_cast<int>(gridPos.x + dx);
			int y = static_cast<int>(gridPos.y + dy);
	
			if (x < 0 || x >= UPSCALED_WIDTH || y < 0 || y >= UPSCALED_HEIGHT)
				continue;
	
			// Circle check
			if (dx * dx + dy * dy <= radius * radius && dx * dx + dy * dy > 10)
			{
				// Random jitter to make irregular edges
				if (rand() % 100 < 30) // 80% chance to destroy
				{
					cells[y][x] = 0;
				}
			}
		}
	}
}

Vector2 Shield::ConvertWorldToGrid(const Vector2& worldPos) const
{
	static Vector2 gridPos;
	gridPos.x = std::floor((worldPos.x - m_position.x) / m_cellSize.x);
	gridPos.y = std::floor((worldPos.y - m_position.y) / m_cellSize.y);

	// Clamp the grid position to the shield's internal grid
	gridPos.x = Clamp(gridPos.x, 0, UPSCALED_WIDTH - 1);
	gridPos.y = Clamp(gridPos.y, 0, UPSCALED_HEIGHT - 1);
	return gridPos;
}

