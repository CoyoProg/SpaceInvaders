#pragma once
#include "Widget.h"
#include "../Data/PlayerData.h"
#include "../Interfaces/IButtonObserver.h"
#include "PlayerCustomizationWidget.h"
#include "Buttons/ButtonWidget.h"
#include <string>
#include <raylib.h>

/*
 * @brief StartMenuWidget class is the root of all the widgets that are displayed in the start menu.
 */
class StartMenuWidget : public Widget, public IButtonObserver, public std::enable_shared_from_this<StartMenuWidget>
{
public:
	StartMenuWidget();
	~StartMenuWidget();

	void AddButton(AnimatedSpriteID spriteIDP, const std::string& textureName, Vector2 positionP, ButtonAction buttonActionP);
	// Subscribe to button pressed events
	void SetupWidgetBindings();

	void SelectNextBase();
	void SelectPreviousBase();
	void SelectNextCanon();
	void SelectPreviousCanon();
	void SelectNextColor();
	void SelectPreviousColor();

	virtual void Draw() override;
	virtual void Update(float deltaTimeP) override;
	virtual void OnButtonPressed(ButtonAction buttonActionP) override;

	// Check if any button is hovered or pressed
	void HandleMouseInput();

private:
	Texture2D m_titleTexture{};
	PlayerCustomizationWidget m_playerCustomizationWidget;
	std::vector<std::unique_ptr<ButtonWidget>> m_buttons;

	PlayerData m_finalData{};
	int m_currentBaseIndex{ 0 };
	int m_currentCanonIndex{ 0 };
	int m_currentColorIndex{ 0 };

	Vector2 m_mousePosition{};
	bool m_LBPressed{ false };
	bool m_shouldStartGame{ false };
	const float m_startLevelDelay{ 0.25f };
	float m_startLevelTimer{ 0.0f };

};

