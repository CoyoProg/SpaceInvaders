#include "LaserProjectile.h"
#include "../Core/GameManager.h"

LaserProjectile::LaserProjectile(int directionP, Vector2 positionP) : m_direction(directionP)
{
	m_position.x = positionP.x;
	m_position.y = positionP.y;
	m_size = { 5, 20 };
	m_color = RED;
}

void LaserProjectile::Update(float deltaTime)
{
	if (IsSleeping) return;

	m_position.y += m_direction * deltaTime * 500;

	// Mark for deletion if it goes off-screen
	if (m_position.y < 0 - m_size.y || m_position.y > SCREEN_HEIGHT)
	{
		IsSleeping = true;
		// m_markedForDeletion = true;
	}
}
