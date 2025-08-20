#pragma once
#include "../Objects/Object.h"
#include "../Interfaces/IUpdatable.h"
#include "../Interfaces/IAlienObserver.h"
#include <vector>
#include <memory>


class Alien;

/*
 * @brief Invader class manages all the aliens in the game. 
 * It handles their movement and when an alien shoot.
 */
class Invader : public Object, public IAlienObserver, public std::enable_shared_from_this<Invader>
{
public:
	Invader(int spaceBetweenRowsP = 30);

	virtual void Update(float deltaSecP) override;
	virtual void OnAlienDied(Alien& alienP) override;

	void AddAlien(std::shared_ptr<Alien> alienP);

private:
	// Updates the position of one alien at a time
	void UpdateAlienPosition(float deltaSecP);
	// Updates the position of one alien at a time
	void UpdateShootProbability(float deltaSecP);
	std::shared_ptr<Alien> GetRandomBottomAlien() const;

	// Checks if any alien will reach the side of the screen at the next step
	bool ShouldChangeDirection() const;

	// Cleans up aliens that are marked for deletion
	void CleanupAliens();

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
	float m_movementDelay = 0.014f;
	float m_delayMovementTimer = 0.0f;

	// Probability of an alien shooting per second
	int m_shootChancePerSecond = 2;
	float m_shootTimer = 0.0f;
	mutable int m_bottomAliensCount = 11;
};

