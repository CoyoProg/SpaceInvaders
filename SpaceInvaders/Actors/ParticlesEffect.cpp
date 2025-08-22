#include "ParticlesEffect.h"
#include "../Core/GameManager.h"

ParticlesEffect::ParticlesEffect(Vector2 positionP, Texture2D textureP, float scaleP)
{
	m_position = positionP;
	m_texture = textureP;
	m_scale = scaleP;
}

void ParticlesEffect::Draw()
{
	DrawTextureEx(
		m_texture,
		Vector2{ m_position.x - m_texture.width / 2 * m_scale, m_position.y - m_texture.height / 2 * m_scale },
		0.0f,
		m_scale,
		WHITE
	);
}

void ParticlesEffect::Update(float deltaTimeP)
{
	m_lifeTime -= deltaTimeP;
	if (m_lifeTime <= 0.0f)
	{
		SetForDeletion(true);
	}
}
