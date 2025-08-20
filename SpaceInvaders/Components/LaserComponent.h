#pragma once
#include "Component.h"
#include <vector>
#include <memory>
#include "raylib.h"

class LaserProjectile;
enum class ActorAffiliation;

class LaserComponent : public Component
{
public:
	void Shoot(int directionP, Vector2 positionP, ActorAffiliation ownerP, int movementSpeedP = 350);

	const int GetLaserCount() const;

private:
	std::vector<std::shared_ptr<LaserProjectile>> m_lasers;
};

