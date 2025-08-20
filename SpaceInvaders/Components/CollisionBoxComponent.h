#pragma once

#include "Component.h"
#include "raylib.h"

/*
 * @brief CollisionBoxComponent defines a rectangular collision box for an actor.
 */
class CollisionBoxComponent : public Component
{
public:
	CollisionBoxComponent(Rectangle rectangleP);

	void SetPosition(Vector2 positionP);
	void SetSize(Vector2 sizeP);

	Rectangle GetBounds() const { return m_rectangle; }

private:
	Rectangle m_rectangle;
};

