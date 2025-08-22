#include "Alien.h"
#include "../Components/CollisionBoxComponent.h"
#include "../Interfaces/IAlienObserver.h"
#include "../Core/GameState.h"
#include "../Core/GameManager.h"

Alien::Alien(Vector2 positionP, Vector2 sizeP, int initialCoordX, int initialCoordY, SpriteID spriteTypeP, Color colorP, int scoreValueP) :
	Actor(ActorAffiliation::Enemy),
	m_initialCoordsX(initialCoordX),
	m_initialCoordsY(initialCoordY),
	m_scoreValue(scoreValueP)
{
	m_position.x = positionP.x;
	m_position.y = positionP.y;
	m_size = sizeP;
	m_color = colorP;

	if (m_CollisionBoxComponent)
	{
		m_CollisionBoxComponent->SetSize(m_size);
		m_CollisionBoxComponent->SetPosition(m_position);
	}

	m_SpriteAnimationComponent.SetupSpriteAnimation(
		GameManager::GetInstance().GetTexture("alienSheet"),
		SPRITE_PROPERTIES.at(spriteTypeP).width,
		SPRITE_PROPERTIES.at(spriteTypeP).height,
		SPRITE_PROPERTIES.at(spriteTypeP).spriteOffset,
		10.0f,
		SPRITE_PROPERTIES.at(spriteTypeP).maxFrameIndex,
		2.7f
	);
}

void Alien::AddObserver(const std::weak_ptr<IAlienObserver> observerP)
{
	m_observers.push_back(observerP);
}

void Alien::RemoveObserver(const std::weak_ptr<IAlienObserver> observerP)
{
	if (observerP.expired()) return;
	std::shared_ptr<IAlienObserver> observerPtr = observerP.lock();

	m_observers.erase(
		std::remove_if(
			m_observers.begin(),
			m_observers.end(),
			[&observerPtr](const std::weak_ptr<IAlienObserver>& wptr) {
				auto sp = wptr.lock();
				return sp == observerPtr; // compare shared_ptrs
			}),
		m_observers.end()
	);
}

void Alien::Draw()
{
	m_SpriteAnimationComponent.Draw(m_position, m_color);
}

void Alien::SetForDeletion(bool markedForDeletionP)
{
	m_markedForDeletion = markedForDeletionP;

	if (!markedForDeletionP) return;

	// Notify all observers about the alien's death
	for (auto it = m_observers.begin(); it != m_observers.end(); )
	{
		// Check if the observer is still valid and erase it if not
		std::shared_ptr<IAlienObserver>  observerPtr = it->lock();
		if (!observerPtr)
		{
			it = m_observers.erase(it);
			continue;
		}

		observerPtr->NotifyAlienDied(*this);
		++it;
	}
}

void Alien::OnAlienMoved()
{
	m_SpriteAnimationComponent.NextAnimationFrame();
}

void Alien::OnCollisionEvent(const Actor& otherActorP)
{
	GameState::GetInstance().AddScore(m_scoreValue);
	m_markedForDeletion = true;

	// Notify all observers about the alien's death
	for (auto it = m_observers.begin(); it != m_observers.end(); )
	{
		// Check if the observer is still valid and erase it if not
		std::shared_ptr<IAlienObserver>  observerPtr = it->lock();
		if (!observerPtr)
		{
			it = m_observers.erase(it);
			continue;
		}

		observerPtr->NotifyAlienDied(*this);
		++it;
	}
}

void Alien::ShootLaser()
{
	m_laserComponent.Shoot(1, Vector2{ m_position.x + m_size.x / 2, m_position.y + m_size.y }, m_actorAffiliation, m_laserSpeed);
}

bool Alien::IsLaserAvailable() const
{
	return m_laserComponent.GetLaserCount() <= m_maxLaserCount;
}