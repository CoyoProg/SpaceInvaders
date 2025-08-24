#include "GameManager.h"
#include "GameState.h"
#include "../Actors/Actor.h"
#include "../Objects/Object.h"
#include "../Widgets/Widget.h"
#include "../Components/CollisionBoxComponent.h"
#include "../Actors/LaserProjectile.h"
#include "../Actors/ParticlesEffect.h"

void GameManager::LoadRessources()
{
	LoadTextures();
	LoadSounds();
}

void GameManager::LoadTextures()
{
	m_textures["alienSheet"] = LoadTexture("Resources/Textures/invadersSpriteSheet.png");
	m_textures["startButtonSheet"] = LoadTexture("Resources/Textures/startButtonSpriteSheet.png");
	m_textures["selectionButtonSheet"] = LoadTexture("Resources/Textures/selectionButtonSpriteSheet.png");
	m_textures["starSheet"] = LoadTexture("Resources/Textures/starSpriteSheet.png");

	Image image = LoadImage("Resources/Textures/ovni.png");
	ImageResize(&image, image.width / 2, image.height / 2);
	m_textures["ovni"] = LoadTextureFromImage(image);
	UnloadImage(image);

	m_textures["title"] = LoadTexture("Resources/Textures/spaceInvadersTitle.png");
	m_textures["baseA"] = LoadTexture("Resources/Textures/Base_A.png");
	m_textures["baseB"] = LoadTexture("Resources/Textures/Base_B.png");
	m_textures["baseC"] = LoadTexture("Resources/Textures/Base_C.png");
	m_textures["baseD"] = LoadTexture("Resources/Textures/Base_D.png");
	m_textures["canonA"] = LoadTexture("Resources/Textures/Canon_A.png");
	m_textures["canonB"] = LoadTexture("Resources/Textures/Canon_B.png");
	m_textures["canonC"] = LoadTexture("Resources/Textures/Canon_C.png");
	m_textures["explosionA"] = LoadTexture("Resources/Textures/explosion_A.png");
	m_textures["explosionB"] = LoadTexture("Resources/Textures/explosion_B.png");
}

void GameManager::LoadSounds()
{
	// ##
	// Bank sound from: https://classicgaming.cc/classics/space-invaders/sounds
	// ##
	m_sounds["gameOver"] = LoadSound("Resources/Sounds/gameOverSoundB.wav");
	m_sounds["playerDeath"] = LoadSound("Resources/Sounds/playerDeathSound.wav");
	m_sounds["alienDeath"] = LoadSound("Resources/Sounds/alienDeathSound.wav");
	m_sounds["laserShoot"] = LoadSound("Resources/Sounds/laserShootSound.wav");
	m_sounds["buttonClick"] = LoadSound("Resources/Sounds/buttonClickSound.wav");
	m_sounds["ovni"] = LoadSound("Resources/Sounds/ovniSound.wav");
}

void GameManager::UnloadTextures()
{
	for (const std::pair<std::string, Texture2D>& texturePair : m_textures)
	{
		UnloadTexture(texturePair.second);
	}
}

void GameManager::UnloadSounds()
{
	for (const std::pair<std::string, Sound>& soundPair : m_sounds)
	{
		UnloadSound(soundPair.second);
	}
}

void GameManager::Update(float deltaTimeP)
{
	// Update widgets if tick is enabled
	for (const std::shared_ptr<Widget>& widget : m_widgets)
	{
		if (widget->m_IsUpdateEnabled)
		{
			widget->Update(deltaTimeP);
		}
	}

	if (m_isGamePaused) return;

	// Update all actors
	for (const std::shared_ptr<IUpdatable>& actor : m_actors)
	{
		if (actor) actor->Update(deltaTimeP);
	}

	// Update all objects
	for (const std::shared_ptr<IUpdatable>& objects : m_objects)
	{
		if (objects) objects->Update(deltaTimeP);
	}
}

void GameManager::Draw()
{
	// Draw all widgets
	for (const std::shared_ptr<Widget>& widget : m_widgets)
	{
		widget->Draw();
	}

	// Draw all actors
	for (const std::shared_ptr<Actor>& actor : m_actors)
	{
		actor->Draw();
	}
}

void GameManager::CollisionCheck()
{
	if (m_isGamePaused) return;

	size_t actorsCount = m_actors.size();

	// Iterate through each actors and check for collisions with every other actor
	for (size_t i = 0; i < actorsCount; ++i)
	{
		for (size_t j = i + 1; j < actorsCount; ++j)
		{
			if (m_actors[i]->IsMarkedForDeletion() || m_actors[j]->IsMarkedForDeletion()) continue;
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

void GameManager::ClearLevel()
{
	m_objects.clear();

	for (const std::shared_ptr<Actor>& actor : m_actors)
	{
		if (actor) actor->SetForDeletion();
	}
}

void GameManager::ClearAllProjectiles()
{
	for (const std::shared_ptr<Actor>& actor : m_actors)
	{
		if (actor && dynamic_cast<LaserProjectile*>(actor.get()))
		{
			actor->SetForDeletion();
		}else if(actor && dynamic_cast<ParticlesEffect*>(actor.get()))
		{
			actor->SetForDeletion();
		}
	}

	m_pendingActors.clear();
}

void GameManager::ClearAllWidgets()
{
	m_widgets.clear();
}

void GameManager::FlushNewActors()
{
	if (m_pendingActors.empty()) return;

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

Texture2D GameManager::GetTexture(const std::string& textureNameP) const
{
	// Check if the texture exists in the map
	auto it = m_textures.find(textureNameP);
	if (it != m_textures.end())
	{
		return it->second;
	}
	else return Texture2D(); // Return an empty texture if not found
}

Sound GameManager::GetSound(const std::string& soundNameP) const
{
	// Check if the texture exists in the map
	auto it = m_sounds.find(soundNameP);
	if (it != m_sounds.end())
	{
		return it->second;
	}
	else return Sound(); // Return an empty texture if not found
}
