#include "LaserProjectile.h"
#include "../Core/GameManager.h"
#include "../Components/CollisionBoxComponent.h"

LaserProjectile::LaserProjectile(int directionP, Vector2 positionP, ActorAffiliation ownerP, int movementSpeedP) :
	Actor(ownerP),
	m_direction(directionP),
	m_movementSpeed(movementSpeedP)
{
	m_position.x = positionP.x;
	m_position.y = positionP.y;
	m_size = { 5, 20 };
	m_color = RED;

	if (m_CollisionBoxComponent)
	{
		m_CollisionBoxComponent->SetSize(m_size);
	}
}

void LaserProjectile::Draw()
{
	if (m_markedForDeletion) return;

	Actor::Draw();
}

void LaserProjectile::Update(float deltaTimeP)
{
	if (m_markedForDeletion) return;

	Actor::Update(deltaTimeP);

	m_position.y += m_direction * deltaTimeP * m_movementSpeed;

	// We mark it for deletion but it will not be deleted, it will only freeze its state until set back to false
	if (m_position.y < 0 - m_size.y || m_position.y > SCREEN_HEIGHT)
	{
		m_markedForDeletion = true;
	}
}

void LaserProjectile::OnCollisionEvent(const Actor& otherActorP)
{
	m_markedForDeletion = true;
}
