#pragma once
#include "Actor.h"

class LaserComponent;

class LaserCanon : public Actor
{
public:
	LaserCanon();
	~LaserCanon();

	virtual void Update(float deltaTimeP) override;
	virtual void OnCollisionEvent(const Actor& otherActorP) override;

private:
	void ProcessInput(float deltaTimeP);
	void Move(int directionP, float deltaTimeP);

private:
	std::unique_ptr<LaserComponent> m_laserComponent;

	const int m_movementSpeed = 300;
	bool doOnce = false;
};

