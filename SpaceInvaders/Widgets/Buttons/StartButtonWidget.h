#pragma once
#include "../Widget.h"
#include "../../Components/SpriteAnimationComponent.h"

enum class ButtonState
{
	Idle,
	Hovered,
	Pressed,

	Disabled
};

/*
 * @brief StartButtonWidget starts the game when pressed.
 */
class StartButtonWidget : public Widget
{
public:
	StartButtonWidget();
	virtual void Draw() override;
	virtual void Update(float deltaTimeP) override;

	Rectangle GetBoundingBox() const;

	void OnHover();
	void OnPress();
	void OnRelease();
	void OnLooseFocus();

private:
	ButtonState m_state{ ButtonState::Idle };
	Vector2 m_position{};
	SpriteAnimationComponent m_spriteAnimationComponent;

	bool m_shouldStartGame{ false };
	const float m_startLevelDelay{ 0.0f };
	float m_startLevelTimer{ 0.0f };
};

