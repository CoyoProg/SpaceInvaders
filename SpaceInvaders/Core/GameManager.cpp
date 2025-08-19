#include "GameManager.h"
#include "GameState.h"
#include "UIManager.h"
#include "../Actors/Actor.h"
#include "../Objects/Object.h"
#include "../Widgets/Widget.h"
#include "../Levels/Level1_SpaceInvaders.h"
#include "../Components/CollisionBoxComponent.h"

#include "../Interfaces/IGameStateObserver.h"
#include "../Widgets/HUDWidget.h"

GameManager::GameManager()
{
	m_currentLevel = std::make_unique<Level1_SpaceInvaders>(*this);
	m_uiManager = std::make_unique<UIManager>(*this);

	// Initialize the GameState singleton
	GameState::GetInstance().AddObserver(m_uiManager->GetHUD());
}

// We need to define the destructor to forward declare the unique_ptr
GameManager::~GameManager() = default;

void GameManager::Update()
{
	// Get the time since the last frame
	float deltaTime = GetFrameTime();

	// Update all actors
	for (const std::shared_ptr<IUpdatable>& actor : m_actors)
	{
		if(actor) actor->Update(deltaTime);
	}

	// Update all objects
	for (const std::shared_ptr<IUpdatable>& objects : m_objects)
	{
		if (objects) objects->Update(deltaTime);
	}

	// Update widgets if tick is enabled
	for (const std::shared_ptr<Widget>& widget : m_widgets)
	{
		if (widget->m_IsUpdateEnabled)
		{
			widget->Update(deltaTime);
		}
	}
}

void GameManager::Draw() const
{
	// Draw all actors
	for (const std::shared_ptr<Actor>& actor : m_actors)
	{
		actor->Draw();
	}

	// Draw all widgets
	for (const std::shared_ptr<Widget>& widget : m_widgets)
	{
		widget->Draw();
	}
}

void GameManager::CollisionCheck()
{
	size_t actorsCount = m_actors.size();

	// Iterate through each actors and check for collisions with every other actor
	for (size_t i = 0; i < actorsCount; ++i)
	{
		for (size_t j = i + 1; j < actorsCount; ++j)
		{
			if (m_actors[i]->CollidesWith(*m_actors[j]))
			{
				m_actors[i]->OnCollisionEvent(*m_actors[j]);
				m_actors[j]->OnCollisionEvent(*m_actors[i]);
			}
		}
	}
}

void GameManager::CleanupActors()
{
	int currentIndex = static_cast<int>(m_actors.size()) - 1;

	for (currentIndex; currentIndex >= 0; --currentIndex)
	{
		std::shared_ptr<Actor>& actor = m_actors[currentIndex];
		if (actor->IsMarkedForDeletion())
		{
			// If the actor is marked for deletion, we move the last actor into the current index position
			// And then remove the last actor from the vector that was just moved (nullify the pointer)
			m_actors[currentIndex] = std::move(m_actors.back());
			m_actors.pop_back();
		}
	}
}

void GameManager::FlushPendingLists()
{
	FlushNewActors();
	FlushNewObjects();
	FlushNewWidgets();
}

void GameManager::FlushNewActors()
{
	if(m_pendingActors.empty()) return;

	// ##
	// Note: Code to insert unique_ptr into vector
	// https://stackoverflow.com/questions/13463570/how-can-i-erase-a-shared-ptr-from-vector
	// ##

	// Move all pending actors to the main actor list
	// We use std::make_move_iterator to move the objects instead of copying them
	m_actors.insert(
		m_actors.end(),
		std::make_move_iterator(m_pendingActors.begin()),
		std::make_move_iterator(m_pendingActors.end())
	);

	m_pendingActors.clear();
}

void GameManager::FlushNewObjects()
{
	if (m_pendingObjects.empty()) return;

	// Move all pending objects to the main object list
	m_objects.insert(
		m_objects.end(),
		std::make_move_iterator(m_pendingObjects.begin()),
		std::make_move_iterator(m_pendingObjects.end())
	);

	m_pendingObjects.clear();
}

void GameManager::FlushNewWidgets()
{
	if (m_pendingWidgets.empty()) return;

	// Move all pending widgets to the main widget list
	m_widgets.insert(
		m_widgets.end(),
		std::make_move_iterator(m_pendingWidgets.begin()),
		std::make_move_iterator(m_pendingWidgets.end())
	);

	m_pendingWidgets.clear();
}

void GameManager::AddActor(std::shared_ptr<Actor> actorP)
{
	m_pendingActors.emplace_back(std::move(actorP));
}

void GameManager::AddObject(std::shared_ptr<Object> objectP)
{
	m_pendingObjects.emplace_back(std::move(objectP));
}

void GameManager::AddWidget(std::shared_ptr<Widget> WidgetP)
{
	m_pendingWidgets.emplace_back(std::move(WidgetP));
}
