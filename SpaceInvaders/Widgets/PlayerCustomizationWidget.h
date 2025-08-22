#pragma once
#include "Widget.h"

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
	float screenOffsetY = 100.0f;
	int spritePadding = 1;
	int scaleFactor = 3;

	Color m_playerColor{ GREEN };
	Vector2 m_basePosition;
	Vector2 m_canonPosition;
	Texture2D m_baseTexture{};
	Texture2D m_canonTexture{};
};

