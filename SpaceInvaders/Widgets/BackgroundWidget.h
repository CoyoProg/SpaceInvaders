#pragma once
#include "Widget.h"
#include <vector>
#include <raylib.h>

struct StarInfo
{
	bool m_isSleeping{ true };

	Vector2 m_position{};
	int m_currentFrameIndex{ 0 };
	float m_alpha{ 0 };
};

class BackgroundWidget : public Widget
{
public:
	BackgroundWidget(Texture2D starSheetP);
	virtual void Draw() override;
	virtual void Update(float deltaTimeP) override;
	void UpdateStarAnimation(float deltaTimeP);

private:
	Texture2D m_starSheet;

	std::vector<std::vector<StarInfo>> m_starsMatrice;
	std::vector<Vector4> m_animatedStarsList;
	int m_animationListLimit{ 10 };
	int m_animationSpeed{ 100 };
	float m_timeAccumulator{ 0.0f };
};

