#pragma once

class Alien;

/*
 * @brief Interface for classes that observe alien events
 */
class IAlienObserver
{
public:
	virtual ~IAlienObserver() = default;
	virtual void NotifyAlienDied(Alien& alienP) = 0;
};

