#include "CounterWidget.h"
#include "../Core/GameState.h"

CounterWidget::CounterWidget(Vector2 positionP, int fontSizeP) :
	m_fontSize(fontSizeP)
{
	m_position = positionP;
	m_IsUpdateEnabled = true;
}

void CounterWidget::Draw()
{
	if(m_visibility == WidgetVisibilty::Hidden)
	{
		return;
	}

	// Draw the score
	DrawText(TextFormat(
		"%01i", static_cast<int>(m_elapsedTime)),
		static_cast<int>(m_position.x),
		static_cast<int>(m_position.y),
		m_fontSize,
		GREEN
	);
}

void CounterWidget::Update(float deltaTimeP)
{
	m_elapsedTime -= deltaTimeP;

	if (m_elapsedTime <= 1)
	{
		GameState::GetInstance().OnCounterFinished();
		m_visibility = WidgetVisibilty::Hidden;

		// Reset the timer for next use
		m_elapsedTime = 4.0f;
		m_IsUpdateEnabled = false;
	}
}
