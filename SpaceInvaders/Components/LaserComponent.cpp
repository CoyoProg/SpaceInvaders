#include "LaserComponent.h"
#include "../Actors/LaserProjectile.h"
#include "../Core/GameManager.h"
#include <iostream>

void LaserComponent::Shoot(int directionP, Vector2 positionP, ActorAffiliation ownerP, int movementSpeedP)
{
	// TO DO:
	// OBSERVER PATTERN TO CHECK WHEN A LASER GOES SLEEPING
	// REUSE SLEEPING LASERS INSTEAD OF CREATING NEW ONES

	// Check if there is a sleeping laser
	for (auto& laser : m_lasers)
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

	auto laser = std::make_shared<LaserProjectile>(directionP, positionP, ownerP, movementSpeedP);
	m_lasers.push_back(laser);
	GameManager::GetInstance().AddActor(laser);
}

const int LaserComponent::GetLaserCount() const
{
	int count = 0;
	for (auto& laser : m_lasers)
	{
		if (!laser->IsMarkedForDeletion()) count++;
	}

	return count;
}
