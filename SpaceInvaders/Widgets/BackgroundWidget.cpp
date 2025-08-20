#include "BackgroundWidget.h"
#include "../Core/GameManager.h"

#include <raylib.h>

BackgroundWidget::BackgroundWidget(Texture2D starSheetP) :
	m_starSheet(starSheetP)
{
	int cellWidth = SCREEN_WIDTH / 10;
	int cellHeight = SCREEN_HEIGHT / 10;

	for (int i = 0; i < 9; ++i)
	{
		std::vector<StarInfo> row;
		for (int j = 0; j < 10; ++j)
		{
			int randomStarValue = rand() % 100; // Randomly choose to place a star or not
			if (randomStarValue < 80) // 80% chance to place a star
			{
				StarInfo properties;
				properties.m_isSleeping = false; // Star is awake
				properties.m_alpha = 255;
				properties.m_currentFrameIndex = rand() % 4;
				properties.m_position = {
					j * cellWidth + ((rand() % cellWidth) - cellWidth / 2.0f),
					i * cellHeight + ((rand() % cellHeight) - cellHeight / 2.0f)
				};

				row.push_back(properties);
			}
			else
			{
				row.push_back(StarInfo{}); // Default properties for no star
			}
		}

		m_starsMatrice.push_back(row);
	}
}

void BackgroundWidget::Draw()
{
	for (int i = 0; i < m_starsMatrice.size(); ++i)
	{
		for (int j = 0; j < m_starsMatrice[i].size(); ++j)
		{
			if (!m_starsMatrice[i][j].m_isSleeping)
			{
				Rectangle sourceRec = {
				10.0f + m_starsMatrice[i][j].m_currentFrameIndex * (10.0f + 76.0f),
				10.0f,
				76.0f,
				76.0f
				};

				Rectangle destRec = {
				m_starsMatrice[i][j].m_position.x,
				m_starsMatrice[i][j].m_position.y,
				sourceRec.width / 4.5f,
				sourceRec.height / 4.5f
				}; // Scale downed by 4.5f

				Color aplhaWhite = { 255, 255, 255, static_cast<unsigned char>(m_starsMatrice[i][j].m_alpha) }; // Full opacity
				DrawTexturePro(m_starSheet, sourceRec, destRec, Vector2{ 0, 0 }, 0.0f, aplhaWhite);
			}
		}
	}
}

void BackgroundWidget::Update(float deltaTimeP)
{
	UpdateStarAnimation(deltaTimeP);

	m_timeAccumulator += deltaTimeP;

	// We don't want to wake up stars too frequently or exceed the animation list limit
	if (m_timeAccumulator < 0.2f || m_animatedStarsList.size() >= m_animationListLimit) return;
	m_timeAccumulator = 0;

	if (rand() % 100 < 50) // 10% chance to wake up a star
	{
		// Randomly choose a star to toggle its sleeping state
		int row = rand() % m_starsMatrice.size();
		int col = rand() % m_starsMatrice[row].size();
		int animationSpeed = rand() % m_animationSpeed + 100;
		int direction = m_starsMatrice[row][col].m_isSleeping;
		if (direction) m_starsMatrice[row][col].m_currentFrameIndex = rand() % 4;

		m_animatedStarsList.emplace_back(Vector4{ static_cast<float>(row), static_cast<float>(col), static_cast<float>(animationSpeed), static_cast<float>(direction) });
	}
}

void BackgroundWidget::UpdateStarAnimation(float deltaTimeP)
{
	if (m_animatedStarsList.empty()) return;

	int currentIndex = static_cast<int>(m_animatedStarsList.size()) - 1;
	for (currentIndex; currentIndex >= 0; --currentIndex)
	{
		int row = static_cast<int>(m_animatedStarsList[currentIndex].x);
		int col = static_cast<int>(m_animatedStarsList[currentIndex].y);

		switch (static_cast<int>(m_animatedStarsList[currentIndex].w))
		{
		case 0: // Reverse animation
		{
			m_starsMatrice[row][col].m_alpha -= deltaTimeP * m_animatedStarsList[currentIndex].z;
			if (m_starsMatrice[row][col].m_alpha <= 0)
			{
				m_starsMatrice[row][col].m_alpha = 0;
				m_starsMatrice[row][col].m_isSleeping = true; // Go to sleep

				m_animatedStarsList[currentIndex] = std::move(m_animatedStarsList.back());
				m_animatedStarsList.pop_back();
			}
			break;
		}
		case 1: // Forward Animation
		{
			m_starsMatrice[row][col].m_alpha += deltaTimeP * m_animatedStarsList[currentIndex].z;
			if (m_starsMatrice[row][col].m_alpha > 255)
			{
				m_starsMatrice[row][col].m_alpha = 255;
				m_starsMatrice[row][col].m_isSleeping = false; // Wake up

				m_animatedStarsList[currentIndex] = std::move(m_animatedStarsList.back());
				m_animatedStarsList.pop_back();
			}
			break;
		}
		default:
			break;
		}
	}
}
