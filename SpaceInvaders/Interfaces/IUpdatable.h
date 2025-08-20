#pragma once

/*
 * @brief Interface for all classes that can be updated every frame
 */
class IUpdatable
{
public:
	virtual ~IUpdatable() = default;
	virtual void Update(float deltaTime) = 0;
};

