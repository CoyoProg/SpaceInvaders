#pragma once
#include "../Interfaces/IUpdatable.h"
#include <vector>
#include <memory>
#include "raylib.h"

class Alien;

/*
 * @brief Invader class manages all the aliens in the game. 
 * It handles their movement and when an alien shoot.
 */
class Invader : public IUpdatable
{
public:
	Invader(int spaceBetweenRowsP = 30);

	// IUpdatable interface implementation
	virtual void Update(float deltaSecP) override;
	// IUpdatable interface

	void AddAlien(std::shared_ptr<Alien> alienP);

private:
	// Updates the position of one alien at a time
	void UpdateAlienPosition();
	// Checks if any alien will reach the side of the screen at the next step
	bool ShouldChangeDirection() const;

private:
	std::vector<std::shared_ptr<Alien>> m_aliens;
	int m_currentAlienIndex = 0;

	// 1 for right, -1 for left
	int m_direction = 1;
	bool m_shouldChangeDirection = false;

	// How many pixels the aliens move per step
	static constexpr int m_distancePerStep = 5;
	int m_spaceBetweenRows = 30;

	// How long the aliens wait before moving again
	// This is used to stagger the movement of the aliens so they don't all move at the same time
	// This is also used to speed up the movement of the aliens
	float m_movementDelay = 0.02f;
	float m_delayMovementTimer = 0.0f;
};

