#pragma once
#include "Actor.h"

/*
 * @brief LaserProjectile moves in a straight line and can collide with other actors that are not of the same affiliation.
 */
class LaserProjectile : public Actor
{
public:
	LaserProjectile(int directionP, Vector2 positionP, ActorAffiliation ownerP, int movementSpeedP = 350);

	virtual void Draw() override;

	virtual void Update(float deltaTimeP) override;
	virtual void OnCollisionEvent(const Actor& otherActorP) override;

private:
	int m_direction;
	int m_movementSpeed{ 350 };
};

