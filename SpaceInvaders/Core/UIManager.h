#pragma once
#include <memory>

class HUDWidget;
class GameManager;

/*
 * @brief UIManager manages the UI components of the game when the level and the player are loaded
 */
class UIManager
{
public:
	UIManager(GameManager& gameManagerP);
	
	const std::shared_ptr<HUDWidget>& GetHUD() const { return m_hud; }

private:
	std::shared_ptr<HUDWidget> m_hud;
};

