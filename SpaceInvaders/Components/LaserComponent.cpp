#include "LaserComponent.h"
#include "../Actors/LaserProjectile.h"
#include "../Core/GameManager.h"

void LaserComponent::Shoot(int directionP, Vector2 positionP, ActorAffiliation ownerP, int movementSpeedP)
{
	// Check if there is a sleeping laser
	for (auto it = m_lasers.begin(); it != m_lasers.end(); )
	{
		std::shared_ptr<LaserProjectile> laserPtr = it->lock();
		if (!laserPtr)
		{
			it = m_lasers.erase(it);
			continue;
		}

		// Check if the laser is marked for deletion (sleeping) and reuse it
		if (laserPtr->IsMarkedForDeletion())
		{
			laserPtr->SetPosition(positionP);
			laserPtr->SetSize({ 5, 20 });
			laserPtr->SetColor(RED);
			laserPtr->SetForDeletion(false);
			GameManager::GetInstance().AddActor(laserPtr);
			return;
		}

		++it;
	}

	// If no sleeping laser was found, create a new one
	std::shared_ptr<LaserProjectile> laser = std::make_shared<LaserProjectile>(directionP, positionP, ownerP, movementSpeedP);
	m_lasers.push_back(laser);
	GameManager::GetInstance().AddActor(laser);
}

const int LaserComponent::GetLaserCount() const
{
	int count = 0;
	for (const std::weak_ptr<LaserProjectile>& laser : m_lasers)
	{
		std::shared_ptr<LaserProjectile> laserPtr = laser.lock();
		if (!laserPtr) continue;
		if (!laserPtr->IsMarkedForDeletion()) count++;
	}

	return count;
}
