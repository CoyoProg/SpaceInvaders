#pragma once
#include "Widget.h"
#include "../Interfaces/IButtonObserver.h"
#include "PlayerCustomizationWidget.h"
#include "Buttons/ButtonWidget.h"

#include <raylib.h>

/*
 * @brief StartMenuWidget class is the root of all the widgets that are displayed in the start menu.
 */
class StartMenuWidget : public Widget, public IButtonObserver, public std::enable_shared_from_this<StartMenuWidget>
{
public:
	StartMenuWidget();
	~StartMenuWidget();

	void SetupWidgetBindings();

	virtual void Draw() override;
	virtual void Update(float deltaTimeP) override;
	virtual void OnButtonPressed(ButtonAction buttonActionP) override;

	void HandleMouseInput();

private:
	std::vector<std::unique_ptr<ButtonWidget>> m_buttons;
	bool m_LBPressed{ false };

	PlayerCustomizationWidget m_playerCustomizationWidget;
	Vector2 m_mousePosition{};

	Texture2D m_titleTexture{};

	bool m_shouldStartGame{ false };
	const float m_startLevelDelay{ 0.25f };
	float m_startLevelTimer{ 0.0f };
};

