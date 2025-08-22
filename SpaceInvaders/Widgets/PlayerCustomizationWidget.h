#pragma once
#include "Widget.h"

/*
 * @brief PlayerCustomizationWidget displays the player's laser canon with selected base, canon and color.
 */
class PlayerCustomizationWidget : public Widget
{
public:
	PlayerCustomizationWidget();

	void SetBaseTexture(Texture2D baseTextureP);
	void SetCanonTexture(Texture2D canonTextureP);
	void SetColor(Color colorP) { m_playerColor = colorP; }

	virtual void Draw() override;
	void DrawCanon();
	void DrawBase();

private:
	float m_screenOffsetY{ 100.0f };
	int m_spritePadding{ 1 };
	int m_scaleFactor{ 3 };

	Color m_playerColor{ GREEN };
	Vector2 m_basePosition;
	Vector2 m_canonPosition;
	Texture2D m_baseTexture{};
	Texture2D m_canonTexture{};
};

