#pragma once
#include "Widget.h"
#include <vector>

struct StarInfo
{
	bool isSleeping{ true };

	Vector2 position{};
	int currentFrameIndex{ 0 };
	float alpha{ 0 };
};

/*
 * @brief BackgroundWidget is persistent across all levels and displays a starry background
 */
class BackgroundWidget : public Widget
{
public:
	BackgroundWidget(Texture2D starSheetP);
	virtual void Draw() override;
	virtual void Update(float deltaTimeP) override;

	// Update the sprite animations of the animated stars list
	// Uses deltaTime to controle the alpha value of the stars
	void UpdateStarAnimation(float deltaTimeP);

private:
	const Texture2D m_starSheet;

	std::vector<std::vector<StarInfo>> m_starsTable;
	std::vector<Vector4> m_animatedStarsList;
	int m_animationSpeed{ 100 };
	static constexpr int m_animationListLimit{ 10 };
	static constexpr float m_toggleStarCooldownTime{ 0.2f };
	float m_toggleStarTimer{ 0.0f };
};

