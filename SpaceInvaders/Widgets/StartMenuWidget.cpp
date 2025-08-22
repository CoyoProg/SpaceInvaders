#include "StartMenuWidget.h"
#include "../Core/GameManager.h"
#include "../Core/GameState.h"

StartMenuWidget::StartMenuWidget()
{
	// Setup all buttons widgets and their actions
	float posX = SCREEN_WIDTH / 2.0f - SPRITE_PROPERTIES.at(AnimatedSpriteID::StartButton).width / 2.0f;
	float posY = SCREEN_HEIGHT * 0.8f - SPRITE_PROPERTIES.at(AnimatedSpriteID::StartButton).height / 2.0f;

	// Start Button
	AddButton(AnimatedSpriteID::StartButton, "startButtonSheet", Vector2{ posX, posY }, ButtonAction::PlayGame);

	posX = SCREEN_WIDTH / 2.0f;
	posY = SCREEN_HEIGHT / 2.0f;
	int offsetX = 150;
	int offsetY = 125;
	const std::string& textureName = "selectionButtonSheet";

	// Base Selection Buttons
	AddButton(AnimatedSpriteID::LeftSelectionButton, textureName, Vector2{ posX - offsetX, posY + offsetY }, ButtonAction::PreviousBase);
	AddButton(AnimatedSpriteID::RightSelectionButton, textureName, Vector2{ posX + offsetX - SPRITE_PROPERTIES.at(AnimatedSpriteID::LeftSelectionButton).width, posY + offsetY }, ButtonAction::NextBase);

	offsetY = 70;
	// Canon Selection Buttons
	AddButton(AnimatedSpriteID::LeftSelectionButton, textureName, Vector2{ posX - offsetX, posY + offsetY }, ButtonAction::PreviousCanon);
	AddButton(AnimatedSpriteID::RightSelectionButton, textureName, Vector2{ posX + offsetX - SPRITE_PROPERTIES.at(AnimatedSpriteID::LeftSelectionButton).width, posY + offsetY }, ButtonAction::NextCanon);

	offsetX = 75;
	// Color Selection Buttons
	AddButton(AnimatedSpriteID::LeftSelectionButton, textureName, Vector2{ posX - offsetX, posY }, ButtonAction::PreviousColor);
	AddButton(AnimatedSpriteID::RightSelectionButton, textureName, Vector2{ posX + offsetX - SPRITE_PROPERTIES.at(AnimatedSpriteID::LeftSelectionButton).width, posY }, ButtonAction::NextColor);

	m_titleTexture = GameManager::GetInstance().GetTexture("title");
	m_finalData.baseTexture = GameManager::GetInstance().GetTexture("baseD");
	m_finalData.canonTexture = GameManager::GetInstance().GetTexture("canonA");
	m_IsUpdateEnabled = true;
}

StartMenuWidget::~StartMenuWidget()
{
}

void StartMenuWidget::AddButton(AnimatedSpriteID spriteIDP, const std::string& textureName, Vector2 positionP, ButtonAction buttonActionP)
{
	Texture2D texture = GameManager::GetInstance().GetTexture(textureName);
	std::unique_ptr<ButtonWidget> button = std::make_unique<ButtonWidget>();
	button->SetupButton(spriteIDP, texture, positionP, buttonActionP);
	m_buttons.emplace_back(std::move(button));
}

void StartMenuWidget::SetupWidgetBindings()
{
	for (auto& button : m_buttons)
	{
		button->AddObserver(shared_from_this());
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
	// Add an offsetX to the title because the texture isn't centered properly
	int offsetX = 10;
	DrawTextureV(m_titleTexture, Vector2{ SCREEN_WIDTH / 2 - m_titleTexture.width / 2.0f + offsetX, SCREEN_HEIGHT / 4.0f - m_titleTexture.height / 2.0f }, WHITE);
}

void StartMenuWidget::Update(float deltaTimeP)
{
	HandleMouseInput();

	if (m_shouldStartGame)
	{
		// Start the game after a short delay (mainly to see the button being released)
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

	// Check if the mouse collides with any button
	for (auto& button : m_buttons)
	{
		if (CheckCollisionPointRec(m_mousePosition, button->GetBoundingBox()))
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				m_LBPressed = true;
				button->OnPress();
			}
			else if ((IsMouseButtonDown(MOUSE_BUTTON_LEFT) && m_LBPressed) || (IsMouseButtonUp(MOUSE_BUTTON_LEFT) && !m_LBPressed))
			{
				button->OnHover();
			}
			else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && m_LBPressed)
			{
				m_LBPressed = false;
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
		m_finalData.maxLasers = 10;
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
		m_finalData.maxLasers = 10;
		m_finalData.laserPerShot = 2;
		m_finalData.laserSpeed = 700;
		m_finalData.canonLaserOffset = 10;
		m_finalData.shootCooldown = 1.25f;
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
		m_finalData.playerColor = RED;
		break;
	}
	case 1:
	{
		m_currentColorIndex = 0;
		m_finalData.playerColor = GREEN;
		break;
	}
	case 2:
	{
		m_currentColorIndex = 1;
		m_finalData.playerColor = SKYBLUE;
		break;
	}
	case 3:
	{
		m_currentColorIndex = 2;
		m_finalData.playerColor = PURPLE;
		break;
	}
	}

	m_playerCustomizationWidget.SetColor(m_finalData.playerColor);
}
