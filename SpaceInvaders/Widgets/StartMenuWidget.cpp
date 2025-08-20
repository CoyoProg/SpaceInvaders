#include "StartMenuWidget.h"
#include "../Core/GameManager.h"


StartMenuWidget::StartMenuWidget()
{
	m_IsUpdateEnabled = true;
}

void StartMenuWidget::Update(float deltaTimeP)
{
	m_mousePosition = GetMousePosition();

	if (CheckCollisionPointRec(m_mousePosition, m_startButtonWidget.GetBoundingBox()))
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) m_startButtonWidget.OnPress();
		else m_startButtonWidget.OnHover();
	}
	else m_startButtonWidget.OnRelease();
}

void StartMenuWidget::Draw()
{
	m_startButtonWidget.Draw();

	DrawTexture(
		GameManager::GetInstance().GetTexture("title"),
		SCREEN_WIDTH / 2 - 105.0f, SCREEN_HEIGHT / 2 - 69.0f,
		WHITE
	);
}
