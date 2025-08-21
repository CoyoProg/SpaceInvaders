#include "GameMessageWidget.h"
#include "../Core/GameState.h"
#include "../Core/GameManager.h"

GameMessageWidget::GameMessageWidget(Vector2 positionP, int fontSizeP) :
	m_fontSize(fontSizeP)
{
	m_position = positionP;
}

void GameMessageWidget::Draw()
{
	if (m_visibility == WidgetVisibilty::Hidden)
	{
		return;
	}

	DrawText(
		m_message,
		static_cast<int>(m_position.x) - MeasureText(m_message, m_fontSize) / 2,
		static_cast<int>(m_position.y) - m_fontSize / 2,
		m_fontSize,
		m_color
	);

	if (m_showRestartMessage)
	{
		DrawText(
			"PRESS SPACE TO RESTART",
			SCREEN_WIDTH / 2 - MeasureText("PRESS SPACE TO RESTAR", 40) / 2,
			static_cast<int>(SCREEN_HEIGHT / 1.5f - m_fontSize / 2.f),
			40,
			GREEN
		);
	}
}

void GameMessageWidget::Update(float deltaTimeP)
{
	m_countdown -= deltaTimeP;

	switch (m_messageType)
	{
	case MessageType::Countdown:
	{
		m_message = TextFormat("%01i", static_cast<int>(m_countdown));

		if (m_countdown <= 1)
		{
			GameState::GetInstance().OnCountdownFinished();
			m_visibility = WidgetVisibilty::Hidden;

			// Reset the timer for next use
			m_countdown = 4.0f;
			m_IsUpdateEnabled = false;
		}

		break;
	}

	case MessageType::GameOver:
	{
		if (m_countdown <= 1)
		{
			m_showRestartMessage = true;

			// Reset the timer for next use
			m_countdown = 4.0f;
			m_IsUpdateEnabled = false;
		}
		break;
	}
	}

}

void GameMessageWidget::NotifyGameOver()
{
	m_messageType = MessageType::GameOver;
	m_position.y = SCREEN_HEIGHT / 8.0f;
	m_message = "Game Over!";
	m_color = RED;
	m_visibility = WidgetVisibilty::Visible;
	m_IsUpdateEnabled = true;
}

void GameMessageWidget::NotifyLevelStart()
{
	m_messageType = MessageType::Countdown;
	m_showRestartMessage = false;
	m_message = "3";
	m_position.y = SCREEN_HEIGHT / 1.8f;
	m_color = GREEN;

	m_visibility = WidgetVisibilty::Visible;
	m_IsUpdateEnabled = true;
}
