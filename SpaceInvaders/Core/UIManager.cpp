#include "UIManager.h"
#include "GameManager.h"
#include "GameState.h"
#include "../Widgets/HUDWidget.h"
#include "../Widgets/GameMessageWidget.h"

UIManager::UIManager(GameManager& gameManagerP, GameState& gameStateP)
{
	std::shared_ptr<GameMessageWidget> gameMessageWidget = std::make_shared<GameMessageWidget>(Vector2{ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.8f }, 60);
	std::shared_ptr<HUDWidget> hudWidget = std::make_shared<HUDWidget>();

	m_hud = hudWidget;
	m_gameMessageWidget = gameMessageWidget;

	// Initialize the GameState singleton
	gameStateP.AddObserver(hudWidget);
	gameStateP.AddObserver(gameMessageWidget);
	gameManagerP.AddWidget(hudWidget);
	gameManagerP.AddWidget(gameMessageWidget);
}
