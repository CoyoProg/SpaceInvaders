#pragma once
#include <vector>
#include <memory>
#include <iostream>

class Actor;
class IUpdatable;
class Level1_SpaceInvaders;

constexpr int SCREEN_WIDTH = 1200;
constexpr int SCREEN_HEIGHT = 900;

/*
 * @brief GameManager is a singleton class that updates and manages all game actors and objects.
 */
class GameManager
{
public:
	// Singleton pattern to ensure only one instance of GameManager exists
	static GameManager& GetInstance() {
		static GameManager instance;
		return instance;
	}

	~GameManager();

	// Delete copy and move constructors and assignment operators to prevent copying
	GameManager(const GameManager& other) = delete;
	GameManager& operator=(const GameManager& other) = delete;
	GameManager(GameManager&&) = delete;
	GameManager& operator=(GameManager&&) = delete;

	// Update all m_actors and objects
	void Update();
	// Draw all m_actors to the screen
	void DrawActors() const;
	// Check for collisions between all m_actors
	void CollisionCheck();
	// Destroy m_actors that are marked for deletion
	void CleanupActors();
	// Flush new m_actors that have been added since the last update
	void FlushNewActors();
	// Flush new objects that have been added since the last update
	void FlushNewObjects();

	// Add actor to the list of m_actors to be added
	// They will be flushed at the end of the frame
	void AddActor(std::shared_ptr<Actor> actorP);
	// Add updatable object to the list of updatable objects
	void AddObject(std::shared_ptr<IUpdatable> objectP);

private:
	// Private constructor for singleton pattern
	GameManager();

	std::unique_ptr<Level1_SpaceInvaders> m_currentLevel;
	std::vector<std::shared_ptr<IUpdatable>> m_objects;
	std::vector<std::shared_ptr<IUpdatable>> m_pendingObjects;
	std::vector<std::shared_ptr<Actor>> m_actors;
	std::vector<std::shared_ptr<Actor>> m_pendingActors;
};

