#include "BackgroundWidget.h"
#include "../Core/GameManager.h"

BackgroundWidget::BackgroundWidget(Texture2D starSheetP) :
	m_starSheet(starSheetP)
{
	int cellWidth = SCREEN_WIDTH / 10;
	int cellHeight = SCREEN_HEIGHT / 10;

	// Initialize the stars table with random star placements
	for (int i = 0; i < 9; ++i)
	{
		std::vector<StarInfo> row;
		for (int j = 0; j < 10; ++j)
		{
			// 20% chance for a coordinate to be empty (no star)
			int randomStarValue = rand() % 100;
			if (randomStarValue > 80)
			{
				// Star is sleeping by default
				row.push_back(StarInfo{});
				continue;
			}

			StarInfo properties;
			properties.isSleeping = false;
			properties.alpha = 255;

			// Randomly choose a frame index for the star
			properties.currentFrameIndex = rand() % 4;

			// Randomly position the star within the cell
			properties.position = {
				j * cellWidth + ((rand() % cellWidth) - cellWidth / 2.0f),
				i * cellHeight + ((rand() % cellHeight) - cellHeight / 2.0f)
			};

			row.push_back(properties);
		}

		m_starsTable.push_back(row);
	}
}

void BackgroundWidget::Draw()
{
	// ##
	// TO DO: Replace with a kill zone actor
	// Draw the earth line
	DrawLine(0, SCREEN_HEIGHT - 45, SCREEN_WIDTH, SCREEN_HEIGHT - 45, GREEN);
	// ##

	// Draw the stars from the stars table
	for (int i = 0; i < m_starsTable.size(); ++i)
	{
		for (int j = 0; j < m_starsTable[i].size(); ++j)
		{
			if (m_starsTable[i][j].isSleeping)
			{
				continue;
			}

			Rectangle sourceRec = {
				10.0f + m_starsTable[i][j].currentFrameIndex * (10.0f + SPRITE_PROPERTIES.at(AnimatedSpriteID::Star).width),
				SPRITE_SHEET_PADDING,
				SPRITE_PROPERTIES.at(AnimatedSpriteID::Star).width,
				SPRITE_PROPERTIES.at(AnimatedSpriteID::Star).height
			};

			// Adjust the scale of the sprite
			Rectangle destRec = {
				m_starsTable[i][j].position.x,
				m_starsTable[i][j].position.y,
				sourceRec.width / 4.f,
				sourceRec.height / 4.f
			};

			Color aplhaWhite = { 255, 255, 255, static_cast<unsigned char>(m_starsTable[i][j].alpha) };
			DrawTexturePro(m_starSheet, sourceRec, destRec, Vector2{ 0, 0 }, 0.0f, aplhaWhite);
		}
	}
}

void BackgroundWidget::Update(float deltaTimeP)
{
	UpdateStarAnimation(deltaTimeP);

	m_toggleStarTimer += deltaTimeP;

	// We don't want to wake up stars too frequently or exceed the animation list limit
	if (m_toggleStarTimer < m_toggleStarCooldownTime || m_animatedStarsList.size() >= m_animationListLimit)
	{
		return;
	}

	m_toggleStarTimer = 0;

	// 50% chance to toggle a star's sleeping state
	if (rand() % 100 < 50)
	{
		// Randomly choose a star and the animation speed
		int row = rand() % m_starsTable.size();
		int col = rand() % m_starsTable[row].size();
		int animationSpeed = rand() % m_animationSpeed + 100;
		int animationDirection = m_starsTable[row][col].isSleeping;
		if (animationDirection) m_starsTable[row][col].currentFrameIndex = rand() % 4;

		m_animatedStarsList.emplace_back(Vector4{ static_cast<float>(row), static_cast<float>(col), static_cast<float>(animationSpeed), static_cast<float>(animationDirection) });
	}
}

void BackgroundWidget::UpdateStarAnimation(float deltaTimeP)
{
	if (m_animatedStarsList.empty())
	{
		return;
	}

	// Update all the stars animations and remove the ones that are done animating
	int currentIndex = static_cast<int>(m_animatedStarsList.size()) - 1;
	for (currentIndex; currentIndex >= 0; --currentIndex)
	{
		int row = static_cast<int>(m_animatedStarsList[currentIndex].x);
		int col = static_cast<int>(m_animatedStarsList[currentIndex].y);

		switch (static_cast<int>(m_animatedStarsList[currentIndex].w))
		{
		case 0: // Reverse animation to sleep
		{
			m_starsTable[row][col].alpha -= deltaTimeP * m_animatedStarsList[currentIndex].z;
			if (m_starsTable[row][col].alpha <= 0)
			{
				m_starsTable[row][col].alpha = 0;
				m_starsTable[row][col].isSleeping = true; // Go to sleep

				m_animatedStarsList[currentIndex] = std::move(m_animatedStarsList.back());
				m_animatedStarsList.pop_back();
			}
			break;
		}
		case 1: // Forward Animation to sprite target
		{
			m_starsTable[row][col].alpha += deltaTimeP * m_animatedStarsList[currentIndex].z;
			if (m_starsTable[row][col].alpha > 255)
			{
				m_starsTable[row][col].alpha = 255;
				m_starsTable[row][col].isSleeping = false; // Wake up

				m_animatedStarsList[currentIndex] = std::move(m_animatedStarsList.back());
				m_animatedStarsList.pop_back();
			}
			break;
		}
		}
	}
}
