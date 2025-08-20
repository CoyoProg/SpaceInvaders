#include "Alien.h"
#include "../Components/CollisionBoxComponent.h"
#include "../Components/SpriteAnimationComponent.h"
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

void Alien::SetupSpriteAnimationComponent(Texture2D spriteSheetP, float spriteWidthP, float spriteHeightP, float spriteOffsetP, float spritePaddingP, int maxFramesP, int scaleFactorP)
{
	m_SpriteAnimationComponent.SetupSpriteAnimation(spriteSheetP, spriteWidthP, spriteHeightP, spriteOffsetP, spritePaddingP, maxFramesP, scaleFactorP);
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

bool Alien::IsLaserAvailable() const
{
	return m_laserComponent->GetLaserCount() == 0;
}