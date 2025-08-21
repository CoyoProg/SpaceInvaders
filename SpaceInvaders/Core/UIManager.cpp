#include "UIManager.h"
#include "GameManager.h"
#include "GameState.h"
#include "../Widgets/HUDWidget.h"
#include "../Widgets/CounterWidget.h"

UIManager::UIManager(GameManager& gameManagerP, GameState& gameStateP)
{
	std::shared_ptr<CounterWidget> counterWidget = std::make_shared<CounterWidget>(Vector2{ SCREEN_WIDTH / 2.0f - 10.0f, SCREEN_HEIGHT / 1.8f }, 60);
	std::shared_ptr<HUDWidget> hudWidget = std::make_shared<HUDWidget>();

	m_hud = hudWidget;
	m_counterWidget = counterWidget;

	// Initialize the GameState singleton
	gameStateP.AddObserver(hudWidget);
	gameManagerP.AddWidget(hudWidget);
	gameManagerP.AddWidget(counterWidget);
}
