#pragma once
#include "Widget.h"
#include "Buttons/StartButtonWidget.h"

#include <raylib.h>

class StartMenuWidget : public Widget
{
public:
	StartMenuWidget();

	virtual void Draw() override;
	virtual void Update(float deltaTimeP) override;

private:
	StartButtonWidget m_startButtonWidget;
	Vector2 m_mousePosition{};
};

