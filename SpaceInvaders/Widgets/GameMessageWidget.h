#pragma once
#include "Widget.h"
#include "../Interfaces/IGameStateObserver.h"
#include <string>

enum class MessageType
{
	Countdown,
	GameOver
};

/*
 * @brief GameMessageWidget displays in game messages such as countdowns and game over prompts.
 */
class GameMessageWidget : public Widget, public IGameStateObserver
{
public:
	GameMessageWidget(Vector2 positionP = { 25.0f, 25.0f }, int fontSizeP = 20);

	virtual void Draw() override;
	virtual void Update(float deltaTimeP) override;

	virtual void NotifyGameOver() override;
	virtual void NotifyLevelStart(int levelIndexP) override;

private:
	bool m_showRestartMessage{ false };
	MessageType m_messageType{ MessageType::Countdown };
	std::string m_message{ "" };
	const int m_fontSize{ 20 };
	Color m_color{ GREEN };
	float m_countdown{ 4.0f };

	// Used to create a typewriter effect when displaying messages
	int m_framesCounter = 100;
	int m_framesSpeed = 15;
};

