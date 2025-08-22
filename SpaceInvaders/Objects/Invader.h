#pragma once
#include "../Objects/Object.h"
#include "../Interfaces/IUpdatable.h"
#include "../Interfaces/IAlienObserver.h"
#include "../Interfaces/IGameStateObserver.h"
#include <vector>
#include <memory>

class Alien;
class Ovni;
/*
 * @brief Invader class manages all the aliens in the game.
 * It handles their movement and when an alien shoot.
 */
class Invader : public Object, public IAlienObserver, public IGameStateObserver, public std::enable_shared_from_this<Invader>
{
public:
	Invader(int spaceBetweenRowsP = 30);
	void SetInvaderSettings(float movementDelayP, int shootProbabilityP);

	virtual void Update(float deltaSecP) override;

	virtual void NotifyAlienDied(Alien& alienP) override;
	virtual void NotifyPlayerDied() override;
	virtual void NotifyPlayerRespawn() override;

	// Add an alien to be tracked by the Invader
	void AddAlien(std::shared_ptr<Alien> alienP);
	void RemoveAllAliens();

private:
	// Updates the position of one alien at a time
	void UpdateAlienPosition(float deltaSecP);
	// Update the chance of an alien shooting
	void UpdateShootProbability(float deltaSecP);
	// Returns randomly one of the lowest aliens on columns
	std::shared_ptr<Alien> GetRandomBottomAlien() const;

	// Checks if any alien will reach the side of the screen at the next step
	bool ShouldChangeDirection() const;

	// Remove aliens that are marked for deletion from the list
	void CleanupAliens();

private:
	std::vector<std::weak_ptr<Alien>> m_aliens;
	std::weak_ptr<Ovni> m_ovni;
	int m_alienToMoveIndex{ 0 };

	// 1 for right, -1 for left
	int m_direction{ 1 };
	bool m_shouldChangeDirection{ false };


	// How many pixels the aliens move per step
	static constexpr int m_distancePerStep{ 5 };
	int m_spaceBetweenRows{ 30 };

	// How long the aliens wait before moving again
	// This is used to stagger the movement of the aliens so they don't all move at the same time
	// This is also used to speed up the movement of the aliens
	float m_movementDelay{ 0.014f };
	float m_delayMovementTimer{ 0.0f };
	bool m_freezeMovement{ false };

	// Probability of an alien shooting per second
	static constexpr float m_shootCooldown{ 0.5f };
	int m_shootProbability{ 70 };
	float m_shootTimer{ 0.0f };
	mutable int m_bottomAliensCount{ 11 };

	// Probability of an ovni spawning per second
	static constexpr float m_ovniSpawnCooldown{ 10.0f };
	float m_ovniSpawnTimer{ 0.0f };
	int m_ovniProbability{ 3 };
};

