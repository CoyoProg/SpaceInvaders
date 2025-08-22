#include "ButtonWidget.h"
#include "../../Core/GameManager.h"
#include "../../Interfaces/IButtonObserver.h"

void ButtonWidget::SetupButton(AnimatedSpriteID spriteIDP, Texture2D textureP, Vector2 positionP, ButtonAction buttonActionP)
{
	m_position = positionP;
	m_buttonAction = buttonActionP;
	m_spriteID = spriteIDP;

	m_spriteAnimationComponent.SetupSpriteAnimation(
		textureP,
		SPRITE_PROPERTIES.at(spriteIDP).width,
		SPRITE_PROPERTIES.at(spriteIDP).height,
		SPRITE_PROPERTIES.at(spriteIDP).spriteOffset,
		SPRITE_SHEET_PADDING,
		SPRITE_PROPERTIES.at(spriteIDP).maxFrameIndex
	);
}

void ButtonWidget::AddObserver(const std::weak_ptr<IButtonObserver> observerP)
{
	m_observers.push_back(observerP);
}

void ButtonWidget::RemoveObserver(const std::weak_ptr<IButtonObserver> observerP)
{
	if (observerP.expired())
	{
		return;
	}
	std::shared_ptr<IButtonObserver> observerPtr = observerP.lock();

	m_observers.erase(
		std::remove_if(
			m_observers.begin(),
			m_observers.end(),
			[&observerPtr](const std::weak_ptr<IButtonObserver>& wptr) {
				auto sp = wptr.lock();
				return sp == observerPtr; // compare shared_ptrs
			}),
		m_observers.end()
	);
}

void ButtonWidget::Draw()
{
	m_spriteAnimationComponent.Draw(m_position, WHITE);
}

Rectangle ButtonWidget::GetBoundingBox() const
{
	return Rectangle{ m_position.x, m_position.y, SPRITE_PROPERTIES.at(m_spriteID).width, SPRITE_PROPERTIES.at(m_spriteID).height};
}

void ButtonWidget::OnHover()
{
	if (m_buttonState != ButtonState::Idle || m_buttonState == ButtonState::Disabled)
	{
		return;
	}

	m_buttonState = ButtonState::Hovered;
	m_spriteAnimationComponent.SetFrame(1);
}

void ButtonWidget::OnPress()
{
	if (m_buttonState != ButtonState::Hovered || m_buttonState == ButtonState::Disabled)
	{
		return;
	}

	PlaySound(GameManager::GetInstance().GetSound("buttonClick"));

	m_buttonState = ButtonState::Pressed;
	m_spriteAnimationComponent.SetFrame(2);
}

void ButtonWidget::OnRelease()
{
	if (m_buttonState != ButtonState::Pressed || m_buttonState == ButtonState::Disabled)
	{
		return;
	}
	m_buttonState = ButtonState::Hovered;
	m_spriteAnimationComponent.SetFrame(1);

	// Notify the StartMenuWidget that the button was pressed
	for (auto it = m_observers.begin(); it != m_observers.end(); )
	{
		std::shared_ptr<IButtonObserver>  observerPtr = it->lock();
		if (!observerPtr)
		{
			it = m_observers.erase(it);
			continue;
		}

		observerPtr->OnButtonPressed(m_buttonAction);
		++it;
	}
}

void ButtonWidget::OnLooseFocus()
{
	if (m_buttonState == ButtonState::Disabled)
	{
		return;
	}

	m_buttonState = ButtonState::Idle;
	m_spriteAnimationComponent.SetFrame(0);
}
