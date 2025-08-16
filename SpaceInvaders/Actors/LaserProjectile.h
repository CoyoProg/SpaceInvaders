#pragma once
#include "Actor.h"

class LaserProjectile : public Actor
{
public:
	LaserProjectile(int directionP, Vector2 positionP);

	void Update(float deltaTimeP) override;

private:
	int m_direction;

	bool IsSleeping = false;
};

