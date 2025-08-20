// ##
// Logic for the Shield destruction
// https://gamedev.stackexchange.com/questions/96404/space-invader-barrier-implementation-algorithm
// https://gamedev.stackexchange.com/questions/117739/how-could-i-implement-destruction-of-bases-in-space-invaders-type-game
// https://ronjeffries.com/articles/020-invaders/i-20/
// ##

#pragma once
#include "Actor.h"
#include <vector>

/*
 * @brief Represents a destructible shield that protects the player from enemy fire.
 */
class Shield : public Actor
{
public:
	Shield(Vector2 positionP, Vector2 sizeP);

	virtual void Draw() override;

	// The shield needs advanced collision detection, checks if the hit position is inside a cell
	virtual bool AdvancedCollidesWith(const Actor& otherActorP) const override;
	virtual void OnCollisionEvent(const Actor& otherActorP);

	// Remove part of the shield at the hit position
	void ExplodeCell(Vector2 hitLocationP);

	// Upsacle the shield pattern to fit the grid
	void ResizePatternToCells();
	// Remove the extra cells to have a smooth edge
	void RemoveExtraCells();
	// Replace the edge cells with triangles
	void UpdateTrianglesCells();

public:
	Vector2 m_cellSize{};
	std::vector<std::vector<int>> cells; // if 0, the the cell is empty

private:
	Vector2 ConvertWorldToGrid(const Vector2& worldPos) const;
	mutable Vector2 m_lastHitLocation{ 0, 0 };
};

