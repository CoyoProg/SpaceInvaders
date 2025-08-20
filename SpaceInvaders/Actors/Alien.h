#pragma once
#include "../Components/SpriteAnimationComponent.h"
#include "Actor.h"

#include <vector>

class LaserComponent;
class IAlienObserver;

class Alien : public Actor
{
public:
	Alien(Vector2 positionP, Vector2 sizeP, int initialCoordX, int initialCoordY, int scoreValueP = 50);

	virtual void Draw() override;

	void AddObserver(const std::shared_ptr<IAlienObserver> observerP);
	void RemoveObserver(const std::shared_ptr<IAlienObserver> observerP);

	void SetupSpriteAnimationComponent(Texture2D spriteSheetP, float spriteWidthP, float spriteHeightP = 84.0f, float spriteOffsetP = 0.0f, float spritePaddingP = 10.0f, int maxFramesP = 1, int scaleFactorP = 1);
	int GetCoordX() const { return m_initialCoordsX; }
	int GetCoordY() const { return m_initialCoordsY; }

	virtual void OnCollisionEvent(const Actor& otherActorP) override;
	void OnAlienMoved();

	void ShootLaser();
	bool IsLaserAvailable() const;

private:
	SpriteAnimationComponent m_SpriteAnimationComponent;
	std::unique_ptr<LaserComponent> m_laserComponent;
	std::vector<std::shared_ptr<IAlienObserver>> m_observers;

	int m_initialCoordsX{ 0 };
	int m_initialCoordsY{ 0 };
};

