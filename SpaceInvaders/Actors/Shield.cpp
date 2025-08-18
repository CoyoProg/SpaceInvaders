#include "Shield.h"
#include "../Components/CollisionBoxComponent.h"

#include <iostream>
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

// We scale up the pattern to have a better collision detection
static constexpr int UPSCALED_WIDTH = 24;
static constexpr int UPSCALED_HEIGHT = 16;



constexpr int EXPLOSION_SIZE = 6;

constexpr int EXPLOSION_MASK1[EXPLOSION_SIZE][EXPLOSION_SIZE] = {
	{0,0,1,1,0,0},
	{0,1,1,1,0,0},
	{0,1,1,1,1,0},
	{0,0,1,1,1,0},
	{0,0,1,1,0,0},
	{0,0,1,1,0,0},
};

constexpr int EXPLOSION_MASK2[EXPLOSION_SIZE][EXPLOSION_SIZE] = {
	{0,0,1,1,1,1},
	{0,1,1,1,1,1},
	{1,1,1,1,1,1},
	{1,1,1,1,1,0},
	{1,1,1,1,1,0},
	{1,1,1,1,0,1},
};

// Store them in a list
constexpr const int (*EXPLOSION_MASKS[])[EXPLOSION_SIZE] = {
	EXPLOSION_MASK1, EXPLOSION_MASK2
};



Shield::Shield(Vector2 positionP, int widthP, int heightP)
{
	width = static_cast<float>(widthP);
	height = static_cast<float>(heightP);

	m_position = positionP;
	m_size = { width, height };

	cellWidth = width / (float)UPSCALED_WIDTH;
	cellHeight = height / (float)UPSCALED_HEIGHT;

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
	int scaleX = UPSCALED_WIDTH / PATTERN_WIDTH;
	int scaleY = UPSCALED_HEIGHT / PATTERN_HEIGHT;

	cells.resize(UPSCALED_HEIGHT, std::vector<int>(UPSCALED_WIDTH, 0));
	for (int py = 0; py < PATTERN_HEIGHT; ++py)
	{
		for (int px = 0; px < PATTERN_WIDTH; ++px)
		{
			if (SHIELD_PATTERN[py][px] != 0)
			{
				// Scale up each pattern cell into a block of cells
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
			if (cells[y][x] == 1)
			{
				// Remove the excessive cells so we can smooth the edges
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
					// If more than one diagonal is true, we need to keep the cell as a square
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
	//DrawRectangle(m_position.x, m_position.y, m_size.x, m_size.y, Fade(RED, 0.5f));

	for (int y = 0; y < UPSCALED_HEIGHT; y++) {
		for (int x = 0; x < UPSCALED_WIDTH; x++)
		{
			float drawX = m_position.x + x * cellWidth;
			float drawY = m_position.y + y * cellHeight;

			switch (cells[y][x])
			{
			case 1:
				DrawRectangle(drawX, drawY, cellWidth, cellHeight, GREEN);
				break;
			case 2:
				DrawTriangle(
					{ drawX, drawY + cellHeight },
					{ drawX + cellWidth, drawY + cellHeight },
					{ drawX + cellWidth, drawY },
					GREEN
				);
				break;
			case 3:
				DrawTriangle(
					{ drawX, drawY },
					{ drawX, drawY + cellHeight },
					{ drawX + cellWidth, drawY },
					GREEN
				);
				break;
			case 4:
				DrawTriangle(
					{ drawX, drawY + cellHeight },
					{ drawX + cellWidth, drawY + cellHeight },
					{ drawX, drawY },
					GREEN
				);
				break;

			case 5:
				DrawTriangle(
					{ drawX + cellWidth, drawY + cellHeight },
					{ drawX + cellWidth, drawY },
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
	Vector2 otherPosition = otherActorP.GetPosition();

	Vector2 gridCoords = ConvertWorldToGrid(otherPosition);

	if (cells[gridCoords.y][gridCoords.x] != 0)
	{
		m_lastHitPosition = gridCoords;

		return true;
	}

	return false;
}

void Shield::OnCollisionEvent(const Actor& otherActorP)
{
	ExplodeCell(m_lastHitPosition.y, m_lastHitPosition.x);
}

void Shield::ExplodeCell(int x, int y)
{
	Vector2 gridPos = { static_cast<float>(y), static_cast<float>(x) };

	// Pick a random mask
	int maskIndex = rand() % (sizeof(EXPLOSION_MASKS) / sizeof(EXPLOSION_MASKS[0]));
	const int (*mask)[EXPLOSION_SIZE] = EXPLOSION_MASKS[maskIndex];

	for (int y = 0; y < EXPLOSION_SIZE; ++y)
	{
		for (int x = 0; x < EXPLOSION_SIZE; ++x)
		{
			if (mask[y][x] == 1)
			{
				int gx = gridPos.x + x - EXPLOSION_SIZE / 2;
				int gy = gridPos.y + y - EXPLOSION_SIZE / 2;

				if (gx >= 0 && gx < UPSCALED_WIDTH &&
					gy >= 0 && gy < UPSCALED_HEIGHT)
				{
					cells[gy][gx] = 0; // destroy this cell
				}
			}
		}
	}


	//int radius = rand()% 2 + 2;
	//
	//for (int dy = -radius; dy <= radius; ++dy)
	//{
	//	for (int dx = -radius; dx <= radius; ++dx)
	//	{
	//		int x = gridPos.x + dx;
	//		int y = gridPos.y + dy;
	//
	//		if (x < 0 || x >= UPSCALED_WIDTH || y < 0 || y >= UPSCALED_HEIGHT)
	//			continue;
	//
	//		// Circle check
	//		if (dx * dx + dy * dy <= radius * radius)
	//		{
	//			// Random jitter to make irregular edges
	//			if (rand() % 100 < 80) // 80% chance to destroy
	//			{
	//				cells[y][x] = 0;
	//			}
	//		}
	//	}
	//}
}

Vector2 Shield::ConvertWorldToGrid(const Vector2& worldPos) const
{
	static Vector2 gridPos;
	gridPos.x = static_cast<int>((worldPos.x - m_position.x) / cellWidth);
	gridPos.y = static_cast<int>((worldPos.y - m_position.y) / cellHeight);

	// Clamp the grid position to the shield's internal grid
	gridPos.x = Clamp(gridPos.x, 0, UPSCALED_WIDTH - 1);
	gridPos.y = Clamp(gridPos.y, 0, UPSCALED_HEIGHT - 1);
	return gridPos;
}

