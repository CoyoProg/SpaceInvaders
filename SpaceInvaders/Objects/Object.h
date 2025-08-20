#pragma once
#include "../Interfaces/IUpdatable.h"

/*
 * @brief Object is an abstract base class for all game objects that can be updated but doesn't have a visual representation.
 */
class Object : public IUpdatable
{
public:
	Object() = default;
	virtual ~Object() = 0 {};

	virtual void Update(float deltaSecP) override = 0;
};

