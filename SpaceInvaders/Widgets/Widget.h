#pragma once
#include "../Interfaces/IDrawable.h"
#include "../Interfaces/IUpdatable.h"
#include "raylib.h"

/*
 * @brief Widget class that serves as a base class for all UI elements.
 */
class Widget : public IUpdatable, public IDrawable
{
public:
	virtual void Draw() override = 0;
	virtual void Update(float deltaTimeP) override {};

	// When set to false the game engine will not call the Update method
	bool m_IsUpdateEnabled{ false };

protected:
	Vector2 m_position{ 0,0 };
};

