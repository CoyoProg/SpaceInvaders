#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include "raylib.h"

class GameState;

class Actor;
class Object;
class Widget;


constexpr int SCREEN_WIDTH = 900;
constexpr int PLAYGROUND_OFFSET = 60;
constexpr int SCREEN_HEIGHT = 900;
constexpr int SPRITE_SHEET_PADDING = 10;

enum class SpriteID
{
	AlienSmall,
	AlienMedium,
	AlienLarge,
	Explosion,
	Laser,
	Star,
	StartButton,
	LeftSelectionButton,
	RightSelectionButton
};

struct SpriteProperties
{
	float width;
	float height;
	int maxFrameIndex;
	float spriteOffset = 0.0f;
};

static const std::unordered_map<SpriteID, SpriteProperties> SPRITE_PROPERTIES
{
	{ SpriteID::AlienSmall, { 81.0f, 84.0f, 1, 244.0f } },
	{ SpriteID::AlienMedium, { 112.0f, 84.0f, 1, 0.0f} },
	{ SpriteID::AlienLarge, { 122.0f, 84.0f, 1, 426.0f} },
	{ SpriteID::Explosion, { 64.0f, 64.0f, 16 } },
	{ SpriteID::Laser, { 10.0f, 30.0f, 1 } },
	{ SpriteID::Star, { 76.0f, 76.0f, 3 } },
	{ SpriteID::StartButton, { 220.0f, 120.0f, 2 } },
	{ SpriteID::LeftSelectionButton, { 28.0f, 42.0f, 2} },
	{ SpriteID::RightSelectionButton, { 28.0f, 42.0f, 2, 114} }
};

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

	// Load all the textures needed for the game
	void LoadRessources();
	void UnloadTextures();

	// Delete copy and move constructors and assignment operators to prevent copying
	GameManager(const GameManager& other) = delete;
	GameManager& operator=(const GameManager& other) = delete;
	GameManager(GameManager&&) = delete;
	GameManager& operator=(GameManager&&) = delete;

	// Update all actors and objects
	void Update(float deltaTimeP);
	// Draw all actors to the screen
	void Draw();
	// Check for collisions between all actors
	void CollisionCheck();
	// Destroy actors that are marked for deletion
	void CleanupActors();
	// Flush all pending lists at the end of the frame
	void FlushPendingLists();

	void SetPauseGame(bool isGamePausedP) { m_isGamePaused = isGamePausedP; }
	void ClearLevel();
	void ClearAllProjectiles();
	void ClearAllWidgets();

	// Add actor to the list of pending actors
	void AddActor(std::shared_ptr<Actor> actorP);
	// Add object to the list of pending objects
	void AddObject(std::shared_ptr<Object> objectP);
	// Add widget to the list of pending widgets
	void AddWidget(std::shared_ptr<Widget> WidgetP);

	Texture2D GetTexture(const std::string& textureName) const;
	int GetActorCount() const { return static_cast<int>(m_actors.size()); }

private:
	// Private constructor for singleton pattern
	GameManager() = default;

	// Flush new actors that have been added since the last update
	void FlushNewActors();
	// Flush new objects that have been added since the last update
	void FlushNewObjects();
	// Flush new widgts that have been added since the last update
	void FlushNewWidgets();

private:
	std::vector<std::shared_ptr<Object>> m_objects;
	std::vector<std::shared_ptr<Object>> m_pendingObjects;
	std::vector<std::shared_ptr<Widget>> m_widgets;
	std::vector<std::shared_ptr<Widget>> m_pendingWidgets;
	std::vector<std::shared_ptr<Actor>> m_actors;
	std::vector<std::shared_ptr<Actor>> m_pendingActors;

	std::unordered_map<std::string, Texture2D> m_textures;

	bool m_isGamePaused{ false };
};

