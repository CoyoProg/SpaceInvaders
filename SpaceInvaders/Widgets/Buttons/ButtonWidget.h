#pragma once
#include "../Widget.h"
#include "../../Components/SpriteAnimationComponent.h"

#include <memory>
#include <vector>

enum class SpriteID;
class IButtonObserver;

enum class ButtonState
{
	Idle,
	Hovered,
	Pressed,

	Disabled
};

enum class ButtonAction
{
	PlayGame,
	NextBase,
	PreviousBase,
	NextCanon,
	PreviousCanon,
	NextColor,
	PreviousColor
};

/*
 * @brief ButtonWidget starts the game when pressed.
 */
class ButtonWidget : public Widget
{
public:
	void SetupButton(SpriteID spriteIDP, Texture2D textureP, Vector2 positionP, ButtonAction buttonActionP);
	virtual void Draw() override;

	void AddObserver(const std::weak_ptr<IButtonObserver> observerP);
	void RemoveObserver(const std::weak_ptr<IButtonObserver> observerP);

	void DisableButton() { m_buttonState = ButtonState::Disabled; }

	ButtonAction GetButtonAction() const { return m_buttonAction; }
	Rectangle GetBoundingBox() const;

	void OnHover();
	void OnPress();
	void OnRelease();
	void OnLooseFocus();

private:
	SpriteID m_spriteID;
	ButtonAction m_buttonAction{ ButtonAction::PlayGame };
	ButtonState m_buttonState{ ButtonState::Idle };

	SpriteAnimationComponent m_spriteAnimationComponent;
	std::vector<std::weak_ptr<IButtonObserver>> m_observers;
};

