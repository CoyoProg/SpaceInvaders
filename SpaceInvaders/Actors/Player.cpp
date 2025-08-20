#include "Player.h"
#include "../Core/GameManager.h"
#include "../Core/GameState.h"
#include "../Components/CollisionBoxComponent.h"

Player::Player() : Actor(ActorAffiliation::Player)
{
	m_position.x = SCREEN_WIDTH / 2 - m_size.x / 2;
	m_position.y = SCREEN_HEIGHT - m_size.y - 20;
	m_size = { 50, 20 };
	m_color = GREEN;

	if (m_CollisionBoxComponent)
	{
		m_CollisionBoxComponent->SetSize(m_size);
	}
}

void Player::Update(float deltaTimeP)
{
	Actor::Update(deltaTimeP);

	ProcessInput(deltaTimeP);
}

void Player::OnCollisionEvent(const Actor& otherActorP)
{
	GameState::GetInstance().OnPlayerDied();
}

void Player::ProcessInput(float deltaTimeP)
{
	// Process input for movement
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) Move(-1, deltaTimeP);
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) Move(1, deltaTimeP);
	if (IsKeyPressed(KEY_SPACE))
	{
		if (m_laserComponent.GetLaserCount() > m_maxLaserCount || GetTime() - m_shootTimer < m_shootCooldown) return;
		m_shootTimer = GetTime();
		m_laserComponent.Shoot(-1, Vector2{ m_position.x + m_size.x / 2, m_position.y - m_size.y}, m_actorAffiliation, 800);
	}
}

void Player::Move(int directionP, float deltaTimeP)
{
	m_position.x += directionP * deltaTimeP * m_movementSpeed;

	// Clamp the position to the playground boundaries
	if (m_position.x < 0 + PLAYGROUND_OFFSET + 1)
	{
		m_position.x = PLAYGROUND_OFFSET + 1;
	}
	else if (m_position.x + m_size.x > SCREEN_WIDTH - PLAYGROUND_OFFSET - 1)
	{
		m_position.x = SCREEN_WIDTH - PLAYGROUND_OFFSET - 1 - m_size.x;
	}
}
