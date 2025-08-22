#pragma once
#include <raylib.h>

struct PlayerData
{
	Color playerColor{ GREEN };
	Texture2D baseTexture{};
	Texture2D canonTexture{};
	int canonLaserOffset{ 0 };
	int lives{ 3 };
	int maxLasers{ 1 };
	int laserPerShot{ 1 };
	int laserSpeed{ 800 };
	float shootCooldown{ 1.0f };
	int movementSpeed{ 200};
};
