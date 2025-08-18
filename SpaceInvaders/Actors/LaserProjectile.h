#pragma once
#include "Actor.h"

class CollisionBoxComponent;

class LaserProjectile : public Actor
{
public:
	LaserProjectile(int directionP, Vector2 positionP, ActorOwner ownerP);
	~LaserProjectile();

	virtual void Update(float deltaTimeP) override;
	virtual void OnCollisionEvent(const Actor& otherActorP) override;

private:
	int m_direction;

	bool IsSleeping = false;
};

