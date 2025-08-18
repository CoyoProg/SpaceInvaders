#include "LaserProjectile.h"
#include "../Core/GameManager.h"
#include "../Components/CollisionBoxComponent.h"

LaserProjectile::LaserProjectile(int directionP, Vector2 positionP, ActorOwner ownerP) : 
	Actor(ownerP),
	m_direction(directionP)
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

LaserProjectile::~LaserProjectile() = default;

void LaserProjectile::Update(float deltaTimeP)
{
	Actor::Update(deltaTimeP);

	if (IsSleeping) return;

	m_position.y += m_direction * deltaTimeP * 500;

	if (m_position.y < 0 - m_size.y || m_position.y > SCREEN_HEIGHT)
	{
		IsSleeping = true;
		m_markedForDeletion = true; // Need to be removed when pooling is implemented
	}
}

void LaserProjectile::OnCollisionEvent(const Actor& otherActorP)
{
	// TO DO: Explosion effect and sound

	m_markedForDeletion = true;
}
