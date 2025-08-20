#include "Actor.h"
#include "../Components/CollisionBoxComponent.h"

#include <iostream>

Actor::Actor(ActorAffiliation ownerP, Color colorP) :
	m_actorAffiliation(ownerP),
	m_color(colorP)
{
	m_CollisionBoxComponent = std::make_unique<CollisionBoxComponent>(Rectangle{ m_position.x, m_position.y, m_size.x, m_size.y });
}

// We need to define the destructor here to be able to forward declare the unique_ptr
Actor::~Actor() = default;

void Actor::Draw()
{
	DrawRectangle(
		static_cast<int>(m_position.x),
		static_cast<int>(m_position.y),
		static_cast<int>(m_size.x),
		static_cast<int>(m_size.y),
		m_color);
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
	// If the actor have the same affiliation, they don't collide (Aliens doesn't collide with their lasers)
	if (otherActorP.GetActorAffiliation() == m_actorAffiliation) return false;
	if (!otherActorP.GetCollisionBoxComponent()) return false;

	// If the two actors collides, we check if one of them needs an advanced collision check
	if (!CheckCollisionRecs(m_CollisionBoxComponent->GetBounds(), otherActorP.GetCollisionBoxComponent()->GetBounds())) return false;
	if (!otherActorP.NeedsAdvancedCollisionCheck() && !m_hasComplexCollision) return true;

	// Do an advanced collision check if needed
	if (otherActorP.NeedsAdvancedCollisionCheck())
	{
		return otherActorP.AdvancedCollidesWith(*this);
	}

	if (m_hasComplexCollision)
	{
		return AdvancedCollidesWith(otherActorP);
	}

	// Note: We shoud also check if both needs advanced collision check; but it will never happen in our case
	// Because we only use advanced collision for the Shields

	return false;
}
