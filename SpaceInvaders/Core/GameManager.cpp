#include "GameManager.h"
#include "../Actors/Actor.h"
#include "../Levels/Level1_SpaceInvaders.h"
#include "../Components/CollisionBoxComponent.h"

GameManager::GameManager()
{
	m_currentLevel = std::make_unique<Level1_SpaceInvaders>(*this);
}

// We need to define the destructor to forward declare the unique_ptr
GameManager::~GameManager() = default;

void GameManager::Update()
{
	// Get the time since the last frame
	float deltaTime = GetFrameTime();
	for (const std::shared_ptr<IUpdatable>& actor : m_actors)
	{
		if(actor) actor->Update(deltaTime);
	}

	for (const std::unique_ptr<IUpdatable>& objects : m_objects)
	{
		if (objects) objects->Update(deltaTime);
	}
}

void GameManager::DrawActors() const
{
	for (const std::shared_ptr<Actor>& actor : m_actors)
	{
		actor->Draw();
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
		if (actor->MarkForDeletion())
		{
			// If the actor is marked for deletion, we move the last actor into the current index position
			// And then remove the last actor from the vector that was just moved (nullify the pointer)
			m_actors[currentIndex] = std::move(m_actors.back());
			m_actors.pop_back();
		}
	}
}

void GameManager::FlushNewActors()
{
	// Move all m_actors from m_pendingActors to m_actors
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
	// Move all objects from objectsToAdd to objects
	m_objects.insert(
		m_objects.end(),
		std::make_move_iterator(m_pendingObjects.begin()),
		std::make_move_iterator(m_pendingObjects.end())
	);

	m_pendingObjects.clear();
}

void GameManager::AddActor(std::shared_ptr<Actor> actorP)
{
	m_pendingActors.emplace_back(std::move(actorP));
}

void GameManager::AddObject(std::unique_ptr<IUpdatable> objectP)
{
	m_pendingObjects.emplace_back(std::move(objectP));
}
