#pragma once
#include "Widget.h"

enum class PlayerColor
{
	PCW_GREEN,
	PCW_BLUE,
	PCW_RED,
	PCW_YELLOW,
	PCW_WHITE,
	PCW_PURPLE,
	PCW_ORANGE,
	PCW_CYAN,
	PCW_MAX_COLORS
};

class PlayerCustomizationWidget : public Widget
{
public:
	PlayerCustomizationWidget();

	virtual void Draw() override;
	void SelectNextColor();
	void SelectPreviousColor();

private:
	Vector2 m_basePosition;
	Vector2 m_canonPosition;
	PlayerColor m_enum{ PlayerColor::PCW_GREEN };
	Color m_playerColor{ GREEN };
	Texture2D m_baseTexture{};
	Texture2D m_canonTexture{};
};

