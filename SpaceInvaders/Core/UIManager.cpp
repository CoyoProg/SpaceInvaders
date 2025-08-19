#include "UIManager.h"
#include "GameManager.h"
#include "../Widgets/HUDWidget.h"

UIManager::UIManager(GameManager& gameManagerP)
{
	m_hud = std::make_shared<HUDWidget>();
	gameManagerP.AddWidget(m_hud);
}
