#pragma once
#include "Actor.h"
#include "../Data/PlayerData.h"
#include "../Components/LaserComponent.h"

/*
 * @brief Player controls the laser canon
 */
class Player : public Actor
{
public:
	Player(PlayerData playerDataP);

	virtual void Draw() override;
	void DrawBase();
	void DrawCanon();
	virtual void Update(float deltaTimeP) override;
	virtual void OnCollisionEvent(const Actor& otherActorP) override;

private:
	void ProcessInput(float deltaTimeP);
	void HandleShoot();
	void Move(int directionP, float deltaTimeP);

private:
	LaserComponent m_laserComponent;
	Texture2D m_baseTexture{};
	Texture2D m_canonTexture{};

	int m_movementSpeed{ 200 };
	int m_maxLaserCount{ 1 };
	int m_laserPerShot{ 1 };
	int m_laserSpeed{ 800 };
	int m_canonLaserOffset{ 0 };
	float m_shootCooldown{ 1.0f };
	double m_shootTimer{ -1.0 };
};

