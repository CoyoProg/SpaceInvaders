#include "Player.h"
#include "../Core/GameManager.h"
#include "../Core/GameState.h"
#include "../Components/CollisionBoxComponent.h"
#include "ParticlesEffect.h"

Player::Player(PlayerData playerDataP) : Actor(ActorAffiliation::Player)
{
	m_size = Vector2{
		static_cast<float>(playerDataP.baseTexture.width - 2),
		static_cast<float>(playerDataP.baseTexture.height + playerDataP.canonTexture.height - 4)
	};

	m_color = playerDataP.playerColor;
	m_maxLaserCount = playerDataP.maxLasers;
	m_shootCooldown = playerDataP.shootCooldown;
	m_movementSpeed = playerDataP.movementSpeed;
	m_baseTexture = playerDataP.baseTexture;
	m_canonTexture = playerDataP.canonTexture;
	m_laserPerShot = playerDataP.laserPerShot;
	m_laserSpeed = playerDataP.laserSpeed;
	m_canonLaserOffset = playerDataP.canonLaserOffset;

	m_position.x = SCREEN_WIDTH / 2 - m_size.x / 2;
	m_position.y = SCREEN_HEIGHT - 45 - m_size.y;

	if (m_CollisionBoxComponent)
	{
		m_CollisionBoxComponent->SetSize(m_size);
	}
}

void Player::Draw()
{
	DrawBase();
	DrawCanon();

}

void Player::DrawBase()
{
	DrawTextureRec(
		m_baseTexture,
		Rectangle{ 1.0f, 1.0f, static_cast<float>(m_baseTexture.width - 2), static_cast<float>(m_baseTexture.height - 2) },
		Vector2{ m_position.x, m_position.y + m_canonTexture.height - 4 },
		m_color
	);
}

void Player::DrawCanon()
{
	DrawTextureRec(
		m_canonTexture,
		Rectangle{1.0f, 1.0f, static_cast<float>(m_canonTexture.width - 2), static_cast<float>(m_canonTexture.height - 2)
		},
		Vector2{ m_position.x + m_baseTexture.width / 2.0f - m_canonTexture.width / 2.0f + 1, m_position.y - 2 },
		m_color
	);
}

void Player::Update(float deltaTimeP)
{
	Actor::Update(deltaTimeP);

	ProcessInput(deltaTimeP);
}

void Player::OnCollisionEvent(const Actor& otherActorP)
{
	Death();
}

void Player::Death()
{
	Vector2 explosionPosition = { m_position.x + m_size.x / 2, m_position.y + m_size.y / 2 };
	std::shared_ptr<ParticlesEffect> explosion = std::make_shared<ParticlesEffect>(explosionPosition, GameManager::GetInstance().GetTexture("explosionA"));
	GameManager::GetInstance().AddActor(explosion);

	SetForDeletion(true);
	GameState::GetInstance().OnPlayerDied();
}

void Player::ProcessInput(float deltaTimeP)
{
	// Process input for movement
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) Move(-1, deltaTimeP);
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) Move(1, deltaTimeP);
	if (IsKeyDown(KEY_SPACE))
	{
		HandleShoot();
	}
}

void Player::HandleShoot()
{
	if (m_laserComponent.GetLaserCount() > m_maxLaserCount || GetTime() - m_shootTimer < m_shootCooldown) return;
	m_shootTimer = GetTime();

	switch (m_laserPerShot)
	{
	case 1:
		m_laserComponent.Shoot(-1, Vector2{ m_position.x + m_size.x / 2, m_position.y - m_size.y }, m_actorAffiliation, m_laserSpeed);
		break;
	case 2:
		m_laserComponent.Shoot(-1, Vector2{ m_position.x + m_size.x / 2 + m_canonLaserOffset, m_position.y - m_size.y }, m_actorAffiliation, m_laserSpeed);
		m_laserComponent.Shoot(-1, Vector2{ m_position.x + m_size.x / 2 - m_canonLaserOffset, m_position.y - m_size.y }, m_actorAffiliation, m_laserSpeed);
		break;
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
