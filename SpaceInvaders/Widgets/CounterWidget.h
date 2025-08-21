#pragma once
#include "Widget.h"

class CounterWidget : public Widget
{
public:
	CounterWidget(Vector2 positionP = { 25.0f, 25.0f }, int fontSizeP = 20);

	virtual void Draw() override;
	virtual void Update(float deltaTimeP) override;

private:
	const int m_fontSize{ 20 };
	float m_elapsedTime{ 4.0f };
	int m_cooldown{ 3 };
};

