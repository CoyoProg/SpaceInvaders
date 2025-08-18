#include "Actor.h"
#include "../Components/CollisionBoxComponent.h"

#include <iostream>

Actor::Actor(ActorOwner ownerP)
{
	m_CollisionBoxComponent = std::make_unique<CollisionBoxComponent>(Rectangle{ m_position.x, m_position.y, m_size.x, m_size.y });

	m_owner = ownerP;
}

Actor::~Actor() = default;

void Actor::Draw()
{
	DrawRectangle(m_position.x, m_position.y, m_size.x, m_size.y, m_color); // Example drawing a red square
}

void Actor::Update(float deltaTimeP)
{
	m_CollisionBoxComponent->SetPosition(m_position);
}

void Actor::SetPosition(Vector2 positionP)
{
	m_position = positionP;

	if (m_CollisionBoxComponent)
	{
		m_CollisionBoxComponent->SetPosition(m_position);
	}
}

void Actor::SetSize(Vector2 sizeP)
{
	m_size = sizeP;

	if (m_CollisionBoxComponent)
	{
		m_CollisionBoxComponent->SetSize(sizeP);
	}
}

const CollisionBoxComponent* Actor::GetCollisionBoxComponent() const
{
	return m_CollisionBoxComponent.get();
}

bool Actor::CollidesWith(const Actor& otherActorP) const
{
	if (otherActorP.GetOwner() == m_owner) return false;

	if (!otherActorP.GetCollisionBoxComponent()) return false;
	if (!CheckCollisionRecs(m_CollisionBoxComponent->GetBounds(), otherActorP.GetCollisionBoxComponent()->GetBounds())) return false;

	if (!otherActorP.NeedsAdvancedCollisionCheck() && !m_hasComplexCollision) return true;

	if (otherActorP.NeedsAdvancedCollisionCheck())
	{
		return otherActorP.AdvancedCollidesWith(*this);
	}

	if (m_hasComplexCollision)
	{
		return AdvancedCollidesWith(otherActorP);
	}

	// Note: We shoud also check if both needs advanced collision check; but it will never happen in our case
	// Because we only use advanced collision check for the Shield m_actors

	return false;
}
