#include "StartButtonWidget.h"
#include "../../Core/GameManager.h"

StartButtonWidget::StartButtonWidget()
{
	m_spriteAnimationComponent.SetupSpriteAnimation(
		GameManager::GetInstance().GetTexture("buttonSheet"),
		SPRITE_PROPERTIES.at(SpriteID::StartButton).width,
		SPRITE_PROPERTIES.at(SpriteID::StartButton).height,
		10.0f,
		10.0f,
		SPRITE_PROPERTIES.at(SpriteID::StartButton).maxFrameIndex
	);

	m_position = {
		SCREEN_WIDTH / 2 - SPRITE_PROPERTIES.at(SpriteID::StartButton).width / 2.0f,
		SCREEN_HEIGHT * 0.8f - SPRITE_PROPERTIES.at(SpriteID::StartButton).height / 2.0f
	};
}

void StartButtonWidget::Draw()
{
	m_spriteAnimationComponent.Draw(m_position, WHITE);
}

Rectangle StartButtonWidget::GetBoundingBox() const
{
	return Rectangle{ m_position.x, m_position.y, SPRITE_PROPERTIES.at(SpriteID::StartButton).width, SPRITE_PROPERTIES.at(SpriteID::StartButton).height };
}

void StartButtonWidget::OnHover()
{
	m_spriteAnimationComponent.SetFrame(1);
}

void StartButtonWidget::OnPress()
{
	m_spriteAnimationComponent.SetFrame(2);
}

void StartButtonWidget::OnRelease()
{
	m_spriteAnimationComponent.SetFrame(0);
}
