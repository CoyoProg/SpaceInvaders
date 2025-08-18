#pragma once
#include "Component.h"
#include "raylib.h"
#include <vector>
#include <memory>

class Actor;
enum class ActorOwner;

class LaserComponent : public Component
{
public:
	void Shoot(int directionP, Vector2 positionP, ActorOwner ownerP);
};

