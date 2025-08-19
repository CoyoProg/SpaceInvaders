#pragma once
#include "../Interfaces/IUpdatable.h"

class Object : public IUpdatable
{
public:
	Object() = default;
	virtual ~Object() = 0 {};

	virtual void Update(float deltaSecP) override = 0;
};

