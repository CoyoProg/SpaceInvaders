#pragma once
#include "Actor.h"

class LaserComponent;

class Player : public Actor
{
public:
	Player();
	~Player();

	virtual void Update(float deltaTimeP) override;
	virtual void OnCollisionEvent(const Actor& otherActorP) override;

private:
	void ProcessInput(float deltaTimeP);
	void Move(int directionP, float deltaTimeP);

private:
	std::unique_ptr<LaserComponent> m_laserComponent;

	static constexpr int m_movementSpeed = 300;
	static constexpr float m_shootCooldown = 1.0f;
	double m_shootTimer = -1.0f;
	bool doOnce = false;
};

