#pragma once
#include <vector>
#include <memory>
#include <iostream>

class Actor;
class IUpdatable;
class Level1_SpaceInvaders;

constexpr int SCREEN_WIDTH = 1200;
constexpr int SCREEN_HEIGHT = 900;

class GameManager
{
public:
	static GameManager& GetInstance() {
		static GameManager instance;
		return instance;
	}

	~GameManager();
	GameManager(const GameManager& other) = delete;
	GameManager& operator=(const GameManager& other) = delete;
	GameManager(GameManager&&) = delete;
	GameManager& operator=(GameManager&&) = delete;

	void Update();
	void CollisionCheck();
	void CleanupActors();
	void FlushNewActors();

	void DrawActors() const;
	
	void AddActor(std::shared_ptr<Actor> actorP)
	{
		actorsToAdd.emplace_back(std::move(actorP));
	}

	void AddObject(std::unique_ptr<IUpdatable> objectP)
	{
		updatableObjects.emplace_back(std::move(objectP));
	}

private:
	GameManager();

	std::unique_ptr<Level1_SpaceInvaders> m_currentLevel;
	std::vector<std::unique_ptr<IUpdatable>> updatableObjects;
	std::vector<std::shared_ptr<Actor>> actors;
	std::vector<std::shared_ptr<Actor>> actorsToAdd;
};

