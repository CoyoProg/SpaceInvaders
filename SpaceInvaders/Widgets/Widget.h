#pragma once
#include "../Interfaces/IDrawable.h"
#include "../Interfaces/IUpdatable.h"

class Widget : public IUpdatable, public IDrawable
{
public:
	Widget() = default;

	virtual void Draw() override = 0;
	virtual void Update(float deltaTimeP) override {};

	bool m_IsUpdateEnabled = false;
};

