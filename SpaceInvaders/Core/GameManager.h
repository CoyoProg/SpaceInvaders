#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include "raylib.h"

class GameState;
class UIManager;
class Actor;
class Object;
class Widget;
class Level1_SpaceInvaders;

constexpr int SCREEN_WIDTH = 900;
constexpr int PLAYGROUND_OFFSET = 80;
constexpr int SCREEN_HEIGHT = 900;

enum class SpriteID
{
	AlienSmall,
	AlienMedium,
	AlienLarge,
	Explosion,
	Laser,
	Player
};

struct SpriteProperties
{
	float width;
	float height;
	int frameCount;
};

static const std::unordered_map<SpriteID, SpriteProperties> SPRITE_PROPERTIES
{
	{ SpriteID::AlienSmall, { 81.0f, 84.0f, 1 } },
	{ SpriteID::AlienMedium, { 112.0f, 84.0f, 1 } },
	{ SpriteID::AlienLarge, { 122.0f, 84.0f, 1 } },
	{ SpriteID::Explosion, { 64, 64.0f, 16 } },
	{ SpriteID::Laser, { 10, 30.0f, 1 } },
	{ SpriteID::Player, { 50, 50.0f, 1 } }
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

	void InitializeGame();
	void LoadRessources();
	void UnloadTextures();

	~GameManager();

	// Delete copy and move constructors and assignment operators to prevent copying
	GameManager(const GameManager& other) = delete;
	GameManager& operator=(const GameManager& other) = delete;
	GameManager(GameManager&&) = delete;
	GameManager& operator=(GameManager&&) = delete;

	// Update all m_actors and objects
	void Update();
	// Draw all m_actors to the screen
	void Draw() const;
	// Check for collisions between all m_actors
	void CollisionCheck();
	// Destroy m_actors that are marked for deletion
	void CleanupActors();
	// Flush all pending lists at the end of the frame
	void FlushPendingLists();

	// Add actor to the list of m_actors to be added
	// They will be flushed at the end of the frame
	void AddActor(std::shared_ptr<Actor> actorP);
	// Add object to the list of pending objects
	void AddObject(std::shared_ptr<Object> objectP);
	// Add widget to the list of pending widgets
	void AddWidget(std::shared_ptr<Widget> WidgetP);

	Texture2D GetTexture(const std::string& textureName) const;

private:
	// Private constructor for singleton pattern
	GameManager();

	// Flush new actors that have been added since the last update
	void FlushNewActors();
	// Flush new objects that have been added since the last update
	void FlushNewObjects();
	// Flush new widgts that have been added since the last update
	void FlushNewWidgets();

private:
	std::unique_ptr<Level1_SpaceInvaders> m_currentLevel;
	std::unique_ptr<UIManager> m_uiManager;

	std::vector<std::shared_ptr<Object>> m_objects;
	std::vector<std::shared_ptr<Object>> m_pendingObjects;
	std::vector<std::shared_ptr<Widget>> m_widgets;
	std::vector<std::shared_ptr<Widget>> m_pendingWidgets;
	std::vector<std::shared_ptr<Actor>> m_actors;
	std::vector<std::shared_ptr<Actor>> m_pendingActors;

	std::unordered_map<std::string, Texture2D> textures;
};

