#include "Actor.h"

void Actor::Draw()
{
	DrawRectangle(m_position.x, m_position.y, m_size.x, m_size.y, m_color); // Example drawing a red square
}
