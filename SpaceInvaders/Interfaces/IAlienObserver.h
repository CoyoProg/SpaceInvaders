#pragma once

class Alien;

class IAlienObserver
{
public:
	virtual ~IAlienObserver() = default;
	virtual void OnAlienDied(Alien& alienP) = 0;
};

