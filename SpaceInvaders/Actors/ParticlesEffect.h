#pragma once
#include "Actor.h"
#include "../Components/SpriteAnimationComponent.h"

/*
 * @brief ParticlesEffect is spawned in the world and dies after a short time.
 */
class ParticlesEffect : public Actor
{
public:
	ParticlesEffect(Vector2 positionP, Texture2D textureP, float scaleP = 0.5f);

	void Draw() override;
	void Update(float deltaTimeP) override;

private:
	Texture2D m_texture{};
	float m_scale{ 0.5f };
	float m_lifeTime{ 0.5f };
};

