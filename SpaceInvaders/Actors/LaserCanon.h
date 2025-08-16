#pragma once
#include "Actor.h"
#include <memory>

class LaserComponent;

class LaserCanon : public Actor
{
public:
	LaserCanon();
	~LaserCanon();

	virtual void Update(float deltaTimeP) override;

private:
	void ProcessInput(float deltaTimeP);
	void Move(int directionP, float deltaTimeP);

private:
	std::unique_ptr<LaserComponent> m_laserComponent;

	const int m_movementSpeed = 300;
	bool doOnce = false;
};

