#include "Alien.h"
#include "../Components/CollisionBoxComponent.h"

Alien::Alien(Vector2 positionP, Vector2 sizeP)
{
	m_position.x = positionP.x;
	m_position.y = positionP.y;
	m_size = sizeP;

	if (m_CollisionBoxComponent)
	{
		m_CollisionBoxComponent->SetSize(m_size);
	}
}

void Alien::Draw()
{
	DrawRectangleGradientH(m_position.x, m_position.y, m_size.x, m_size.y, m_previousColor, m_nextColor);
}

void Alien::SetColor(Color previousColorP, Color nextColorP)
{
	m_previousColor = previousColorP;
	m_nextColor = nextColorP;
}

void Alien::OnCollisionEvent(const Actor& otherActorP)
{
	// TO DO: Explosion effect and sound
	m_markedForDeletion = true;
}
