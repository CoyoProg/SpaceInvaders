#pragma once
#include <memory>

class HUDWidget;
class GameManager;

class UIManager
{
public:
	UIManager(GameManager& gameManagerP);
	
	const std::shared_ptr<HUDWidget>& GetHUD() const { return m_hud; }

private:
	std::shared_ptr<HUDWidget> m_hud;
};

