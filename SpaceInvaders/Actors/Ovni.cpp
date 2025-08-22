#include "Ovni.h"
#include "../Core/GameManager.h"
#include "../Core/GameState.h"
#include "ParticlesEffect.h"

Ovni::Ovni(int directionP) :  Actor(ActorAffiliation::Enemy)
{
	m_direction = directionP;

	float randomY = rand() % 50 + 45.0f;
	m_position = { (m_direction == 1) ? 0 - 50.0f : static_cast<float>(SCREEN_WIDTH) + 50.0f, randomY };

	m_texture = GameManager::GetInstance().GetTexture("ovni");
	m_size = { static_cast<float>(m_texture.width), static_cast<float>(m_texture.height) };
}

void Ovni::Draw()
{
	DrawTextureV(m_texture, m_position, RED);
}

void Ovni::Update(float deltaTimeP)
{
	Actor::Update(deltaTimeP);

	m_position.x += 100.f * deltaTimeP * m_direction;
	if (m_position.x < 0 - m_size.x || m_position.x > SCREEN_WIDTH + m_size.x)
	{
		StopSound(GameManager::GetInstance().GetSound("ovni"));
		SetForDeletion(true);
	}

	m_soundDelay -= deltaTimeP;
	if(m_soundDelay <= 0.0f)
	{
		PlaySound(GameManager::GetInstance().GetSound("ovni"));
		m_soundDelay = 0.275f;
	}
}

void Ovni::OnCollisionEvent(const Actor& otherActorP)
{
	GameState::GetInstance().AddScore(100);
	Death();
}

void Ovni::Death()
{
	StopSound(GameManager::GetInstance().GetSound("ovni"));
	PlaySound(GameManager::GetInstance().GetSound("alienDeath"));

	Vector2 explosionPosition = { m_position.x + m_size.x / 2, m_position.y + m_size.y / 2 };
	std::shared_ptr<ParticlesEffect> explosion = std::make_shared<ParticlesEffect>(explosionPosition, GameManager::GetInstance().GetTexture("explosionA"));
	GameManager::GetInstance().AddActor(explosion);

	m_markedForDeletion = true;
}
