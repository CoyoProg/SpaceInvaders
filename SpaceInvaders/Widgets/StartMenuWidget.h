#pragma once
#include "Widget.h"
#include "Buttons/StartButtonWidget.h"

#include <raylib.h>

/*
 * @brief StartMenuWidget class is the root of all the widgets that are displayed in the start menu.
 */
class StartMenuWidget : public Widget
{
public:
	StartMenuWidget();

	virtual void Draw() override;
	virtual void Update(float deltaTimeP) override;

	void HandleMouseInput();

private:
	bool m_LBPressed{ false };
	StartButtonWidget m_startButtonWidget;
	Vector2 m_mousePosition{};

	Texture2D m_titleTexture{};
};

