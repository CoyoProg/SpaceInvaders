#include "LaserComponent.h"
#include "../Actors/LaserProjectile.h"
#include "../Core/GameManager.h"
#include <iostream>

void LaserComponent::Shoot(int directionP, Vector2 positionP, ActorAffiliation ownerP, int movementSpeedP)
{
	// Check if there is a sleeping laser
	for (const std::shared_ptr<LaserProjectile>& laser : m_lasers)
	{
		if (laser->IsMarkedForDeletion())
		{
			laser->SetPosition(positionP);
			laser->SetSize({ 5, 20 });
			laser->SetColor(RED);
			laser->SetForDeletion(false);
			GameManager::GetInstance().AddActor(laser);
			return;
		}
	}

	// If no sleeping laser was found, create a new one
	std::shared_ptr<LaserProjectile> laser = std::make_shared<LaserProjectile>(directionP, positionP, ownerP, movementSpeedP);
	m_lasers.push_back(laser);
	GameManager::GetInstance().AddActor(laser);
}

const int LaserComponent::GetLaserCount() const
{
	int count = 0;
	for (const std::shared_ptr<LaserProjectile>& laser : m_lasers)
	{
		if (!laser->IsMarkedForDeletion()) count++;
	}

	return count;
}
