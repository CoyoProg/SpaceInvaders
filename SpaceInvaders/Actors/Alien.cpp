#include "Alien.h"
#include "../Components/CollisionBoxComponent.h"
#include "../Interfaces/IAlienObserver.h"
#include "../Core/GameState.h"
#include "../Core/GameManager.h"

Alien::Alien(Vector2 positionP, Vector2 sizeP, int initialCoordX, int initialCoordY, SpriteID spriteTypeP, Color colorP, int scoreValueP) :
	Actor(ActorAffiliation::Enemy),
	m_initialCoordsX(initialCoordX),
	m_initialCoordsY(initialCoordY),
	m_scoreValue(scoreValueP),
	m_SpriteAnimationComponent(
		GameManager::GetInstance().GetTexture("alienSheet"),
		SPRITE_PROPERTIES.at(spriteTypeP).width,
		SPRITE_PROPERTIES.at(spriteTypeP).height,
		SPRITE_PROPERTIES.at(spriteTypeP).spriteOffset,
		10.0f,
		SPRITE_PROPERTIES.at(spriteTypeP).maxFrameIndex,
		2.7f
	)
{
	m_position.x = positionP.x;
	m_position.y = positionP.y;
	m_size = sizeP;
	m_color = colorP;

	if (m_CollisionBoxComponent)
	{
		m_CollisionBoxComponent->SetSize(m_size);
	}
}

void Alien::AddObserver(const std::shared_ptr<IAlienObserver> observerP)
{
	m_observers.push_back(observerP);
}

void Alien::RemoveObserver(const std::shared_ptr<IAlienObserver> observerP)
{
	m_observers.erase(
		std::remove(
			m_observers.begin(),
			m_observers.end(),
			observerP),
		m_observers.end()
	);
}

void Alien::Draw()
{
	m_SpriteAnimationComponent.Draw(m_position, m_color);
}

void Alien::OnAlienMoved()
{
	m_SpriteAnimationComponent.NextAnimationFrame();
}

void Alien::OnCollisionEvent(const Actor& otherActorP)
{
	GameState::GetInstance().AddScore(m_scoreValue);

	m_markedForDeletion = true;

	for (const std::shared_ptr<IAlienObserver>& observer : m_observers)
	{
		observer->OnAlienDied(*this);
	}
}

void Alien::ShootLaser()
{
	m_laserComponent.Shoot(1, Vector2{ m_position.x + m_size.x / 2, m_position.y + m_size.y + 10.f }, m_actorAffiliation);
}

bool Alien::IsLaserAvailable() const
{
	return m_laserComponent.GetLaserCount() <= m_maxLaserCount;
}