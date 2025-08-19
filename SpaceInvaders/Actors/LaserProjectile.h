#pragma once
#include "Actor.h"

class CollisionBoxComponent;

class LaserProjectile : public Actor
{
public:
	LaserProjectile(int directionP, Vector2 positionP, ActorAffiliation ownerP, int movementSpeedP = 350);
	~LaserProjectile();

	virtual void Draw() override;

	virtual void Update(float deltaTimeP) override;
	virtual void OnCollisionEvent(const Actor& otherActorP) override;

private:
	int m_direction;
	int m_movementSpeed = 350;
};

