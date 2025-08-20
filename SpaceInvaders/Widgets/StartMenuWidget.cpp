#include "StartMenuWidget.h"
#include "../Core/GameManager.h"

StartMenuWidget::StartMenuWidget()
{
	m_titleTexture = GameManager::GetInstance().GetTexture("title");

	m_IsUpdateEnabled = true;
}

void StartMenuWidget::Update(float deltaTimeP)
{
	HandleMouseInput();

	if (m_startButtonWidget.m_IsUpdateEnabled) m_startButtonWidget.Update(deltaTimeP);
}

void StartMenuWidget::HandleMouseInput()
{
	m_mousePosition = GetMousePosition();

	// ##
	// TO DO: Have a vector with all buttons that needs to be checked for mouse input
	if (CheckCollisionPointRec(m_mousePosition, m_startButtonWidget.GetBoundingBox()))
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			m_startButtonWidget.OnPress();
			m_LBPressed = true;
		}
		
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !m_LBPressed)
		{
			m_startButtonWidget.OnHover();
			m_LBPressed = true;
		}

		if (IsMouseButtonUp(MOUSE_BUTTON_LEFT) && m_LBPressed)
		{
			m_startButtonWidget.OnRelease();
			m_LBPressed = false;
		}
		else if(!m_LBPressed) m_startButtonWidget.OnHover();
	}
	else
	{
		m_startButtonWidget.OnLooseFocus();
		m_LBPressed = false;
	}
	// ##
}

void StartMenuWidget::Draw()
{
	m_startButtonWidget.Draw();

	DrawTextureV(
		m_titleTexture,
		Vector2{ SCREEN_WIDTH / 2 - 700 / 4.0f, SCREEN_HEIGHT / 2 - 400.f / 4 },
		WHITE
	);
}
