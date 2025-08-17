#include "CollisionBoxComponent.h"

CollisionBoxComponent::CollisionBoxComponent(Rectangle rectangleP) : m_rectangle(rectangleP)
{
}

void CollisionBoxComponent::SetPosition(Vector2 positionP)
{
	m_rectangle.x = positionP.x;
	m_rectangle.y = positionP.y;
}

void CollisionBoxComponent::SetSize(Vector2 sizeP)
{
	m_rectangle.width = sizeP.x; m_rectangle.height = sizeP.y;
}
