#pragma once
#include <memory>

class HUDWidget;
class CounterWidget;
class GameManager;
class GameState;

/*
 * @brief UIManager manages the UI components of the game when the level and the player are loaded
 */
class UIManager
{
public:
	UIManager(GameManager& gameManagerP, GameState& gameStateP);

private:
	std::weak_ptr<HUDWidget> m_hud;
	std::weak_ptr<CounterWidget> m_counterWidget;
};

