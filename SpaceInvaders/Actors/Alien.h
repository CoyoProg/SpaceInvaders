#pragma once

#include "Actor.h"

class Alien : public Actor
{
public:
	virtual void Draw() override;

	void SetColor(Color previousColor, Color nextColor)
	{
		m_previousColor = previousColor;
		m_nextColor = nextColor;
	}

private:
	Color m_previousColor{ BLUE };
	Color m_nextColor{ BLUE };
};

