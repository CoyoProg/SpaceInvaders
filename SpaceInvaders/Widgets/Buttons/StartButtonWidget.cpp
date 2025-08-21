#include "StartButtonWidget.h"
#include "../../Core/GameManager.h"
#include "../../Core/GameState.h"

StartButtonWidget::StartButtonWidget() :
	m_spriteAnimationComponent(
		GameManager::GetInstance().GetTexture("buttonSheet"),
		SPRITE_PROPERTIES.at(SpriteID::StartButton).width,
		SPRITE_PROPERTIES.at(SpriteID::StartButton).height,
		0.0f,
		SPRITE_SHEET_PADDING,
		SPRITE_PROPERTIES.at(SpriteID::StartButton).maxFrameIndex
	)
{
	m_position = {
		SCREEN_WIDTH / 2 - SPRITE_PROPERTIES.at(SpriteID::StartButton).width / 2.0f,
		SCREEN_HEIGHT * 0.8f - SPRITE_PROPERTIES.at(SpriteID::StartButton).height / 2.0f
	};
}

void StartButtonWidget::Draw()
{
	m_spriteAnimationComponent.Draw(m_position, WHITE);
}

void StartButtonWidget::Update(float deltaTimeP)
{
	// Update is disabled by default until the button is pressed

	// Start the game after a short delay for transition effect
	m_startLevelTimer += deltaTimeP;
	if (m_startLevelTimer > m_startLevelDelay)
	{
		GameState::GetInstance().StartLevel();
	}
}

Rectangle StartButtonWidget::GetBoundingBox() const
{
	return Rectangle{ m_position.x, m_position.y, SPRITE_PROPERTIES.at(SpriteID::StartButton).width, SPRITE_PROPERTIES.at(SpriteID::StartButton).height };
}

void StartButtonWidget::OnHover()
{
	if (m_state != ButtonState::Idle || m_state == ButtonState::Disabled) return;

	m_state = ButtonState::Hovered;
	m_spriteAnimationComponent.SetFrame(1);
}

void StartButtonWidget::OnPress()
{
	if (m_state != ButtonState::Hovered || m_state == ButtonState::Disabled) return;

	m_state = ButtonState::Pressed;
	m_spriteAnimationComponent.SetFrame(2);
}

void StartButtonWidget::OnRelease()
{
	if (m_state != ButtonState::Pressed || m_state == ButtonState::Disabled) return;

	m_state = ButtonState::Disabled;
	m_spriteAnimationComponent.SetFrame(0);
	m_shouldStartGame = true;
	m_IsUpdateEnabled = true;
}

void StartButtonWidget::OnLooseFocus()
{
	if (m_state == ButtonState::Disabled) return;

	m_state = ButtonState::Idle;
	m_spriteAnimationComponent.SetFrame(0);
}
