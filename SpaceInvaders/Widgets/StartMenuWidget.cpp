#include "StartMenuWidget.h"
#include "../Core/GameManager.h"
#include "../Core/GameState.h"

#include <iostream>
StartMenuWidget::StartMenuWidget()
{
	std::unique_ptr<ButtonWidget> startButtonPtr = std::make_unique<ButtonWidget>();
	startButtonPtr->SetupButton(
		SpriteID::StartButton,
		GameManager::GetInstance().GetTexture("buttonSheet"),
		Vector2{ SCREEN_WIDTH / 2 - SPRITE_PROPERTIES.at(SpriteID::StartButton).width / 2.0f, SCREEN_HEIGHT * 0.8f - SPRITE_PROPERTIES.at(SpriteID::StartButton).height / 2.0f },
		ButtonAction::PlayGame
	);

	std::unique_ptr<ButtonWidget> baseLB = std::make_unique<ButtonWidget>();
	baseLB->SetupButton(
		SpriteID::StartButton,
		GameManager::GetInstance().GetTexture("buttonSheet"),
		Vector2{ SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2.0f + 75 },
		ButtonAction::PreviousBase
	);

	std::unique_ptr<ButtonWidget> baseRB = std::make_unique<ButtonWidget>();
	baseRB->SetupButton(
		SpriteID::StartButton,
		GameManager::GetInstance().GetTexture("buttonSheet"),
		Vector2{ SCREEN_WIDTH / 2 + 150 - SPRITE_PROPERTIES.at(SpriteID::StartButton).width, SCREEN_HEIGHT / 2.0f + 75 },
		ButtonAction::NextBase
	);

	m_buttons.emplace_back(std::move(startButtonPtr));
	m_buttons.emplace_back(std::move(baseLB));
	m_buttons.emplace_back(std::move(baseRB));

	m_titleTexture = GameManager::GetInstance().GetTexture("title");
	m_IsUpdateEnabled = true;
}

StartMenuWidget::~StartMenuWidget()
{
}

void StartMenuWidget::SetupWidgetBindings()
{
	for(auto& button : m_buttons)
	{
		button->AddObserver(shared_from_this());
	}
}

void StartMenuWidget::Update(float deltaTimeP)
{
	HandleMouseInput();

	if (m_shouldStartGame)
	{
		// Start the game after a short delay for transition effect
		m_startLevelTimer += deltaTimeP;
		if (m_startLevelTimer > m_startLevelDelay)
		{
			GameState::GetInstance().StartLevel();
		}
	}
}

void StartMenuWidget::OnButtonPressed(ButtonAction buttonActionP)
{
	switch (buttonActionP)
	{
	case ButtonAction::PlayGame:
		m_shouldStartGame = true;
		for (auto& button : m_buttons) button->DisableButton();
		break;
	}
}

void StartMenuWidget::HandleMouseInput()
{
	m_mousePosition = GetMousePosition();

	for(auto& button : m_buttons)
	{
		if (CheckCollisionPointRec(m_mousePosition, button->GetBoundingBox()))
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				button->OnPress();
				m_LBPressed = true;
			}

			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			{
				button->OnHover();
			}

			if (IsMouseButtonUp(MOUSE_BUTTON_LEFT) && m_LBPressed)
			{
				button->OnRelease();
			}
			else if (!m_LBPressed) button->OnHover();
		}
		else
		{
			button->OnLooseFocus();
		}
	}

	if (IsMouseButtonUp(MOUSE_BUTTON_LEFT) && m_LBPressed)
	{
		m_LBPressed = false;
	}

	if (IsKeyPressed(KEY_SPACE))
	{
		m_playerCustomizationWidget.SelectNextColor();
	}
}

void StartMenuWidget::Draw()
{
	for (auto& button : m_buttons)
	{
		button->Draw();
	}

	// Draw the title
	// We need to add this offset because the texture isn't centered properly
	int offsetX = 10;
	DrawTextureV(
		m_titleTexture,
		Vector2{ SCREEN_WIDTH / 2 - m_titleTexture.width / 2.0f + offsetX, SCREEN_HEIGHT / 4.0f - m_titleTexture.height / 2.0f },
		WHITE
	);
}
