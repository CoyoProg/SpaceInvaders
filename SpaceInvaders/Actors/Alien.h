#pragma once
#include "Actor.h"
#include "../Components/SpriteAnimationComponent.h"
#include "../Components/LaserComponent.h"

#include <vector>

enum class AnimatedSpriteID;
class IAlienObserver;

/*
 * @brief Alien class that moves across the screen and shoots lasers at the player.
 */
class Alien : public Actor
{
public:
	Alien(Vector2 positionP, Vector2 sizeP, int initialCoordX, int initialCoordY, AnimatedSpriteID spriteTypeP, Color colorP, int scoreValueP = 50);

	virtual void Draw() override;
	virtual void SetForDeletion(bool markedForDeletionP = true) override;
	virtual void Death() override;

	void AddObserver(const std::weak_ptr<IAlienObserver> observerP);
	void RemoveObserver(const std::weak_ptr<IAlienObserver> observerP);

	void SetLaserSpeed(int speedP) { m_laserSpeed = speedP; }
	
	int GetCoordX() const { return m_initialCoordsX; }
	int GetCoordY() const { return m_initialCoordsY; }

	virtual void OnCollisionEvent(const Actor& otherActorP) override;
	void OnAlienMoved();

	void ShootLaser();
	bool IsLaserAvailable() const;

private:
	SpriteAnimationComponent m_SpriteAnimationComponent;
	LaserComponent m_laserComponent;
	std::vector<std::weak_ptr<IAlienObserver>> m_observers;

	static constexpr int m_maxLaserCount{ 1 };
	int m_laserSpeed{ 350 };
	const int m_scoreValue{ 50 };
	int m_initialCoordsX{ 0 };
	int m_initialCoordsY{ 0 };
};

