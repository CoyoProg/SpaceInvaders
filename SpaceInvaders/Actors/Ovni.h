#pragma once
#include "Actor.h"

/*
 * @brief Alien Bonus that go from one side to another, giving extra points if hit
 */
class Ovni : public Actor
{
public:
	Ovni(int directionP);

	virtual void Draw() override;
	virtual void Update(float deltaTimeP) override;
	virtual void OnCollisionEvent(const Actor& otherActorP) override;
	virtual void Death() override;

private:
	Texture2D m_texture{};

	// 1 = right, -1 = left
	int m_direction{ 1 };
	float m_soundDelay{ 0.2f };
};

