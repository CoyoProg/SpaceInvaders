#pragma once
#include "Actor.h"
#include "../Components/LaserComponent.h"

/*
 * @brief Player controls the laser canon
 */
class Player : public Actor
{
public:
	Player();

	virtual void Update(float deltaTimeP) override;
	virtual void OnCollisionEvent(const Actor& otherActorP) override;

private:
	void ProcessInput(float deltaTimeP);
	void Move(int directionP, float deltaTimeP);

private:
	LaserComponent m_laserComponent;

	static constexpr int m_movementSpeed{ 200 };
	static constexpr int m_maxLaserCount{ 1 };
	static constexpr float m_shootCooldown{ 1.0f };
	double m_shootTimer{ -1.0 };
};

