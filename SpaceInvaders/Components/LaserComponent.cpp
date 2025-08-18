#include "LaserComponent.h"
#include "../Actors/LaserProjectile.h"
#include "../Core/GameManager.h"

void LaserComponent::Shoot(int directionP, Vector2 positionP, ActorOwner ownerP)
{
	// TO DO:
	// OBSERVER PATTERN TO CHECK WHEN A LASER GOES SLEEPING
	// REUSE SLEEPING LASERS INSTEAD OF CREATING NEW ONES

	auto laser = std::make_shared<LaserProjectile>(directionP, positionP, ownerP);
	GameManager::GetInstance().AddActor(laser);
}
