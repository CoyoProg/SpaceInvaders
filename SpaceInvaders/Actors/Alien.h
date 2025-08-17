#pragma once
#include "Actor.h"

class Alien : public Actor
{
public:
	Alien(Vector2 positionP, Vector2 sizeP);
	virtual void Draw() override;

	void SetColor(Color previousColorP, Color nextColorP);

	virtual void OnCollisionEvent(const Actor& otherActorP) override;

private:
	Color m_previousColor{ BLUE };
	Color m_nextColor{ BLUE };
};

