#pragma once
#include "../Widget.h"
#include "../../Components/SpriteAnimationComponent.h"

class StartButtonWidget : public Widget
{
public:
	StartButtonWidget();
	virtual void Draw() override;

	Rectangle GetBoundingBox() const;

	void OnHover();
	void OnPress();
	void OnRelease();

private:
	Vector2 m_position{};
	SpriteAnimationComponent m_spriteAnimationComponent;
};

