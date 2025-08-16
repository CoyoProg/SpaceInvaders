#include "GameManager.h"
#include "../Actors/Actor.h"
#include "../Levels/Level1_SpaceInvaders.h"

GameManager::GameManager()
{
	level1 = std::make_unique<Level1_SpaceInvaders>(*this);
}

GameManager::~GameManager()
{
}

void GameManager::CleanupActors()
{
	int i = actors.size() - 1;
	for (i; i >= 0; --i)
	{
		auto& actor = actors[i];
		if (actor->MarkForDeletion())
		{
			actors.erase(actors.begin() + i);
		}
	}
}

void GameManager::FlushNewActors()
{
	// Move all actors from actorsToAdd to actors
	actors.insert(actors.end(), actorsToAdd.begin(), actorsToAdd.end());
	actorsToAdd.clear();
}

void GameManager::UpdateActors()
{
	// 1. Update positions
	// --------------------
	// Move player, bullets, aliens, etc.
	
	// 2. Check collisions
	// --------------------
	// Bullet vs alien
	// Bullet vs shield
	// Alien vs player or bottom of screen
	
	// 3. Game Logic
	// --------------------
	// Update scores, lives, shield health here



	float deltaTime = GetFrameTime(); // Get the time elapsed since the last frame
	for (const auto& actor : actors)
	{
		if(actor) actor->Update(deltaTime);
	}

	level1->Update(deltaTime);
}

void GameManager::DrawActors() const
{
	// 4. Draw everything
	for (const auto& actor :actors)
	{
		actor->Draw();
	}
}
