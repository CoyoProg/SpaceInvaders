#include "Alien.h"
#include "../Components/CollisionBoxComponent.h"
#include "../Components/LaserComponent.h"
#include "../Interfaces/IAlienObserver.h"
#include "../Core/GameState.h"

Alien::Alien(Vector2 positionP, Vector2 sizeP, int initialCoordX, int initialCoordY, int scoreValueP) :
	Actor(ActorAffiliation::Enemy),
	m_initialCoordsX(initialCoordX),
	m_initialCoordsY(initialCoordY)
{
	m_position.x = positionP.x;
	m_position.y = positionP.y;
	m_size = sizeP;

	m_laserComponent = std::make_unique<LaserComponent>();

	if (m_CollisionBoxComponent)
	{
		m_CollisionBoxComponent->SetSize(m_size);
	}
}

void Alien::Draw()
{
	DrawRectangleGradientH(
		static_cast<int>(m_position.x),
		static_cast<int>(m_position.y),
		static_cast<int>(m_size.x),
		static_cast<int>(m_size.y), 
		m_previousColor,
		m_nextColor);
}

void Alien::SetColor(Color previousColorP, Color nextColorP)
{
	m_previousColor = previousColorP;
	m_nextColor = nextColorP;
}

void Alien::OnCollisionEvent(const Actor& otherActorP)
{
	GameState::GetInstance().AddScore(10);

	m_markedForDeletion = true;

	// TO DO: Explosion effect and sound
	for(const auto& observer : m_observers)
	{
		observer->OnAlienDied(*this);
	}
}

void Alien::ShootLaser()
{
	m_laserComponent->Shoot(1, Vector2{ m_position.x + m_size.x / 2, m_position.y + m_size.y + 10.f }, m_owner);
}
