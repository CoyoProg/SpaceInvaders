#include "Actor.h"
#include "../Components/CollisionBoxComponent.h"

Actor::Actor()
{
	m_CollisionBoxComponent = std::make_unique<CollisionBoxComponent>(Rectangle{ m_position.x, m_position.y, m_size.x, m_size.y });
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

const CollisionBoxComponent* Actor::GetCollisionBoxComponent() const
{
	return m_CollisionBoxComponent.get();
}
