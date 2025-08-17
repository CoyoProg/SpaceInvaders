#pragma once
#include "../Interfaces/IUpdatable.h"
#include <vector>
#include <memory>
#include "raylib.h"

class Alien;

class Invader : public IUpdatable
{
public:
	Invader(int spaceBetweenRowsP = 30);
	virtual void Update(float deltaSecP) override;

	void AddAlien(std::shared_ptr<Alien> alienP);

private:
	void UpdateCurrentAlienPosition();
	bool ShouldChangeDirection() const;

private:
	std::vector<std::shared_ptr<Alien>> m_aliens;
	int m_currentAlienIndex = 0;

	int m_direction = 1; // 1 for right, -1 for left
	int m_spaceBetweenRows = 30;
	const int m_distancePerStep = 5;
	bool m_shouldChangeDirection = false;
	float m_movementDelay = 0.02f;
	float m_delayMovementTimer = 0.0f;
};

