#pragma once
#include "Component.h"
#include "raylib.h"
#include <vector>
#include <memory>

class Actor;

class LaserComponent : public Component
{
public:
	void Shoot(int directionP, Vector2 positionP);

public:
	std::vector<std::shared_ptr<Actor>> m_lasers; // Store active lasers
};

