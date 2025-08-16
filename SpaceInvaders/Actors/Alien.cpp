#include "Alien.h"

void Alien::Draw()
{
	DrawRectangleGradientH(m_position.x, m_position.y, m_size.x, m_size.y, m_previousColor, m_nextColor);
}
