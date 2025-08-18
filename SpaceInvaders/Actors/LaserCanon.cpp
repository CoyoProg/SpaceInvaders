#include "LaserCanon.h"
#include "../Core/GameManager.h"
#include "../Components/LaserComponent.h"
#include "../Components/CollisionBoxComponent.h"

LaserCanon::LaserCanon() : Actor(ActorOwner::Player)
{
	// Move the laser canon to the bottom center of the screen
	m_position.x = SCREEN_WIDTH / 2 - m_size.x / 2;
	m_position.y = SCREEN_HEIGHT - m_size.y - 20;
	m_size = { 50, 20 };
	m_color = GREEN;

	m_laserComponent = std::make_unique<LaserComponent>();

	if (m_CollisionBoxComponent)
	{
		m_CollisionBoxComponent->SetSize(m_size);
	}
}

LaserCanon::~LaserCanon() = default;

void LaserCanon::Update(float deltaTimeP)
{
	Actor::Update(deltaTimeP);

	ProcessInput(deltaTimeP);
}

void LaserCanon::OnCollisionEvent(const Actor& otherActorP)
{
}

void LaserCanon::ProcessInput(float deltaTimeP)
{
	// Process input for movement
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) Move(-1, deltaTimeP);
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) Move(1, deltaTimeP);
	if(IsKeyPressed(KEY_SPACE))
	{
		// Fire a laser (this is just a placeholder, actual firing logic would go here)
		m_laserComponent->Shoot(-1, Vector2{ m_position.x + m_size.x / 2, m_position.y - m_size.y - 1.0f}, m_owner);
	}
}

void LaserCanon::Move(int directionP, float deltaTimeP)
{
	m_position.x += directionP * deltaTimeP * m_movementSpeed;

	if (m_position.x < 0)
		m_position.x = 0; // Prevent moving off the left edge
	else if (m_position.x + m_size.x > SCREEN_WIDTH)
		m_position.x = SCREEN_WIDTH - m_size.x; // Prevent moving off the right edge
}
