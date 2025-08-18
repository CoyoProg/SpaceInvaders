#pragma once
#include "Actor.h"

#include <vector>

class LaserComponent;
class IAlienObserver;

class Alien : public Actor
{
public:
	Alien(Vector2 positionP, Vector2 sizeP);

	virtual void Draw() override;

	void AddObersver(const std::shared_ptr<IAlienObserver> observerP)
	{
		m_observers.push_back(observerP);
	}

	void RemoveObserver(const std::shared_ptr<IAlienObserver> observerP)
	{
		m_observers.erase(
			std::remove(
				m_observers.begin(),
				m_observers.end(),
				observerP),
			m_observers.end()
		);
	}

	void SetColor(Color previousColorP, Color nextColorP);
	virtual void OnCollisionEvent(const Actor& otherActorP) override;

	void ShootLaser();

private:
	std::unique_ptr<LaserComponent> m_laserComponent;
	std::vector<std::shared_ptr<IAlienObserver>> m_observers;

	Color m_previousColor{ BLUE };
	Color m_nextColor{ BLUE };
};

