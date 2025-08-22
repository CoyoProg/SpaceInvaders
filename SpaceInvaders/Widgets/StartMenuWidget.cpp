#include "StartMenuWidget.h"
#include "../Core/GameManager.h"
#include "../Core/GameState.h"

#include <iostream>
StartMenuWidget::StartMenuWidget()
{
	std::unique_ptr<ButtonWidget> startButtonPtr = std::make_unique<ButtonWidget>();
	startButtonPtr->SetupButton(
		SpriteID::StartButton,
		GameManager::GetInstance().GetTexture("startButtonSheet"),
		Vector2{ SCREEN_WIDTH / 2 - SPRITE_PROPERTIES.at(SpriteID::StartButton).width / 2.0f, SCREEN_HEIGHT * 0.8f - SPRITE_PROPERTIES.at(SpriteID::StartButton).height / 2.0f },
		ButtonAction::PlayGame
	);

	std::unique_ptr<ButtonWidget> baseLB = std::make_unique<ButtonWidget>();
	baseLB->SetupButton(
		SpriteID::LeftSelectionButton,
		GameManager::GetInstance().GetTexture("selectionButtonSheet"),
		Vector2{ SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2.0f + 125 },
		ButtonAction::PreviousBase
	);

	std::unique_ptr<ButtonWidget> baseRB = std::make_unique<ButtonWidget>();
	baseRB->SetupButton(
		SpriteID::RightSelectionButton,
		GameManager::GetInstance().GetTexture("selectionButtonSheet"),
		Vector2{ SCREEN_WIDTH / 2 + 150 - SPRITE_PROPERTIES.at(SpriteID::LeftSelectionButton).width, SCREEN_HEIGHT / 2.0f + 125 },
		ButtonAction::NextBase
	);

	std::unique_ptr<ButtonWidget> canonLB = std::make_unique<ButtonWidget>();
	canonLB->SetupButton(
		SpriteID::LeftSelectionButton,
		GameManager::GetInstance().GetTexture("selectionButtonSheet"),
		Vector2{ SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2.0f + 70 },
		ButtonAction::PreviousCanon
	);

	std::unique_ptr<ButtonWidget> canonRB = std::make_unique<ButtonWidget>();
	canonRB->SetupButton(
		SpriteID::RightSelectionButton,
		GameManager::GetInstance().GetTexture("selectionButtonSheet"),
		Vector2{ SCREEN_WIDTH / 2 + 150 - SPRITE_PROPERTIES.at(SpriteID::LeftSelectionButton).width, SCREEN_HEIGHT / 2.0f + 70 },
		ButtonAction::NextCanon
	);

	std::unique_ptr<ButtonWidget> colorLB = std::make_unique<ButtonWidget>();
	colorLB->SetupButton(
		SpriteID::LeftSelectionButton,
		GameManager::GetInstance().GetTexture("selectionButtonSheet"),
		Vector2{ SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT / 2.0f },
		ButtonAction::PreviousColor
	);

	std::unique_ptr<ButtonWidget> colorRB = std::make_unique<ButtonWidget>();
	colorRB->SetupButton(
		SpriteID::RightSelectionButton,
		GameManager::GetInstance().GetTexture("selectionButtonSheet"),
		Vector2{ SCREEN_WIDTH / 2 + 75 - SPRITE_PROPERTIES.at(SpriteID::LeftSelectionButton).width, SCREEN_HEIGHT / 2.0f },
		ButtonAction::NextColor
	);

	m_buttons.emplace_back(std::move(startButtonPtr));
	m_buttons.emplace_back(std::move(baseLB));
	m_buttons.emplace_back(std::move(baseRB));
	m_buttons.emplace_back(std::move(canonLB));
	m_buttons.emplace_back(std::move(canonRB));
	m_buttons.emplace_back(std::move(colorLB));
	m_buttons.emplace_back(std::move(colorRB));

	m_titleTexture = GameManager::GetInstance().GetTexture("title");
	m_finalData.baseTexture = GameManager::GetInstance().GetTexture("baseD");
	m_finalData.canonTexture = GameManager::GetInstance().GetTexture("canonA");
	m_IsUpdateEnabled = true;
}

StartMenuWidget::~StartMenuWidget()
{
}

void StartMenuWidget::SetupWidgetBindings()
{
	for (auto& button : m_buttons)
	{
		button->AddObserver(shared_from_this());
	}
}

void StartMenuWidget::SelectNextBase()
{
	switch (m_currentBaseIndex)
	{
	case 0:
	{
		m_currentBaseIndex = 1;
		m_finalData.baseTexture = GameManager::GetInstance().GetTexture("baseB");
		m_finalData.movementSpeed = 125;
		m_finalData.lives = 4;
		break;
	}
	case 1:
	{
		m_currentBaseIndex = 2;
		m_finalData.baseTexture = GameManager::GetInstance().GetTexture("baseC");
		m_finalData.movementSpeed = 100;
		m_finalData.lives = 5;
		break;
	}
	case 2:
	{
		m_currentBaseIndex = 3;
		m_finalData.baseTexture = GameManager::GetInstance().GetTexture("baseA");
		m_finalData.movementSpeed = 250;
		m_finalData.lives = 2;
		break;
	}
	case 3:
	{
		m_currentBaseIndex = 0;
		m_finalData.baseTexture = GameManager::GetInstance().GetTexture("baseD");
		m_finalData.movementSpeed = 200;
		m_finalData.lives = 3;
		break;
	}
	}

	m_playerCustomizationWidget.SetBaseTexture(m_finalData.baseTexture);
}

void StartMenuWidget::SelectPreviousBase()
{
	switch (m_currentBaseIndex)
	{
	case 0:
	{
		m_currentBaseIndex = 3;
		m_finalData.baseTexture = GameManager::GetInstance().GetTexture("baseA");
		m_finalData.movementSpeed = 250;
		m_finalData.lives = 2;
		break;
	}
	case 1:
	{
		m_currentBaseIndex = 0;
		m_finalData.baseTexture = GameManager::GetInstance().GetTexture("baseD");
		m_finalData.movementSpeed = 200;
		m_finalData.lives = 3;
		break;
	}
	case 2:
	{
		m_currentBaseIndex = 1;
		m_finalData.baseTexture = GameManager::GetInstance().GetTexture("baseB");
		m_finalData.movementSpeed = 150;
		m_finalData.lives = 4;
		break;
	}
	case 3:
	{
		m_currentBaseIndex = 2;
		m_finalData.baseTexture = GameManager::GetInstance().GetTexture("baseC");
		m_finalData.movementSpeed = 100;
		m_finalData.lives = 4;
		break;
	}
	}

	m_playerCustomizationWidget.SetBaseTexture(m_finalData.baseTexture);
}

void StartMenuWidget::SelectNextCanon()
{
	switch (m_currentCanonIndex)
	{
	case 0:
	{
		m_currentCanonIndex = 1;
		m_finalData.canonTexture = GameManager::GetInstance().GetTexture("canonB");
		m_finalData.maxLasers = 10;
		m_finalData.laserPerShot = 2;
		m_finalData.laserSpeed = 700;
		m_finalData.canonLaserOffset = 10;
		m_finalData.shootCooldown = 1.25f;
		break;
	}
	case 1:
	{
		m_currentCanonIndex = 2;
		m_finalData.canonTexture = GameManager::GetInstance().GetTexture("canonC");
		m_finalData.canonLaserOffset = 5;
		m_finalData.maxLasers = 10;
		m_finalData.laserPerShot = 2;
		m_finalData.laserSpeed = 800;
		m_finalData.shootCooldown = 1.5f;
		break;
	}
	case 2:
	{
		m_currentCanonIndex = 0;
		m_finalData.canonTexture = GameManager::GetInstance().GetTexture("canonA");
		m_finalData.canonLaserOffset = 0;
		m_finalData.maxLasers = 1;
		m_finalData.laserPerShot = 1;
		m_finalData.laserSpeed = 800;
		m_finalData.shootCooldown = 1.0f;
		break;
	}
	}

	m_playerCustomizationWidget.SetCanonTexture(m_finalData.canonTexture);
}

void StartMenuWidget::SelectPreviousCanon()
{
	switch (m_currentCanonIndex)
	{
	case 0:
	{
		m_currentCanonIndex = 2;
		m_finalData.canonTexture = GameManager::GetInstance().GetTexture("canonC");
		m_finalData.canonLaserOffset = 5;
		m_finalData.maxLasers = -1;
		m_finalData.laserPerShot = 2;
		m_finalData.laserSpeed = 800;
		m_finalData.shootCooldown = 1.5f;
		break;
	}
	case 1:
	{
		m_currentCanonIndex = 0;
		m_finalData.canonTexture = GameManager::GetInstance().GetTexture("canonA");
		m_finalData.canonLaserOffset = 0;
		m_finalData.maxLasers = 1;
		m_finalData.laserPerShot = 1;
		m_finalData.laserSpeed = 800;
		m_finalData.shootCooldown = 1.0f;
		break;
	}
	case 2:
	{
		m_currentCanonIndex = 1;
		m_finalData.canonTexture = GameManager::GetInstance().GetTexture("canonB");
		m_finalData.canonLaserOffset = 10;
		m_finalData.maxLasers = -1;
		m_finalData.laserPerShot = 2;
		m_finalData.laserSpeed = 800;
		m_finalData.shootCooldown = 1.5f;
		break;
	}
	}

	m_playerCustomizationWidget.SetCanonTexture(m_finalData.canonTexture);
}

void StartMenuWidget::SelectNextColor()
{
	switch (m_currentColorIndex)
	{
	case 0:
	{
		m_currentColorIndex = 1;
		m_finalData.playerColor = SKYBLUE;
		break;
	}
	case 1:
	{
		m_currentColorIndex = 2;
		m_finalData.playerColor = PURPLE;
		break;
	}
	case 2:
	{
		m_currentColorIndex = 3;
		m_finalData.playerColor = RED;
		break;
	}
	case 3:
	{
		m_currentColorIndex = 0;
		m_finalData.playerColor = GREEN;
		break;
	}
	}

	m_playerCustomizationWidget.SetColor(m_finalData.playerColor);
}

void StartMenuWidget::SelectPreviousColor()
{
	switch (m_currentColorIndex)
	{
	case 0:
	{
		m_currentColorIndex = 3;
		m_finalData.playerColor = GREEN;
		break;
	}
	case 1:
	{
		m_currentColorIndex = 0;
		m_finalData.playerColor = SKYBLUE;
		break;
	}
	case 2:
	{
		m_currentColorIndex = 1;
		m_finalData.playerColor = PURPLE;
		break;
	}
	case 3:
	{
		m_currentColorIndex = 2;
		m_finalData.playerColor = RED;
		break;
	}
	}

	m_playerCustomizationWidget.SetColor(m_finalData.playerColor);
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
			GameState::GetInstance().StartLevel(m_finalData);
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
	case ButtonAction::NextBase:
	{
		SelectNextBase();
		break;
	}
	case ButtonAction::PreviousBase:
	{
		SelectPreviousBase();
		break;
	}
	case ButtonAction::NextCanon:
	{
		SelectNextCanon();
		break;
	}
	case ButtonAction::PreviousCanon:
	{
		SelectPreviousCanon();
		break;
	}
	case ButtonAction::NextColor:
	{
		SelectNextColor();
		break;
	}
	case ButtonAction::PreviousColor:
	{
		SelectPreviousColor();
		break;
	}
	}
}

void StartMenuWidget::HandleMouseInput()
{
	m_mousePosition = GetMousePosition();

	for (auto& button : m_buttons)
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
}

void StartMenuWidget::Draw()
{
	for (auto& button : m_buttons)
	{
		button->Draw();
	}

	m_playerCustomizationWidget.Draw();

	// Draw the title
	// We need to add this offset because the texture isn't centered properly
	int offsetX = 10;
	DrawTextureV(
		m_titleTexture,
		Vector2{ SCREEN_WIDTH / 2 - m_titleTexture.width / 2.0f + offsetX, SCREEN_HEIGHT / 4.0f - m_titleTexture.height / 2.0f },
		WHITE
	);
}
