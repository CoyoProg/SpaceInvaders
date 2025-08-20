#pragma once
#include "Actor.h"
#include <vector>

class LaserComponent;
class IAlienObserver;
class SpriteAnimationComponent;

class Alien : public Actor
{
public:
	Alien(Vector2 positionP, Vector2 sizeP, int initialCoordX, int initialCoordY, int scoreValueP = 50);

	virtual void Draw() override;

	void AddObserver(const std::shared_ptr<IAlienObserver> observerP);
	void RemoveObserver(const std::shared_ptr<IAlienObserver> observerP);

	SpriteAnimationComponent& GetSpriteAnimationComponent() const { return *m_SpriteAnimationComponent; }
	int GetCoordX() const { return m_initialCoordsX; }
	int GetCoordY() const { return m_initialCoordsY; }

	void SetColor(Color previousColorP, Color nextColorP);
	virtual void OnCollisionEvent(const Actor& otherActorP) override;

	void OnAlienMoved();

	void ShootLaser();
	bool IsLaserAvailable() const;

private:
	std::unique_ptr<LaserComponent> m_laserComponent;
	std::unique_ptr<SpriteAnimationComponent> m_SpriteAnimationComponent;
	std::vector<std::shared_ptr<IAlienObserver>> m_observers;

	int counterFrame{ 0};
	int maxFrames{ 1 };
	Rectangle spriteRect1{ 10.0f, 10.0f, 112.f, 84.0f };

	Color m_previousColor{ BLUE };
	Color m_nextColor{ BLUE };
	int m_initialCoordsX{ 0 };
	int m_initialCoordsY{ 0 };
};

