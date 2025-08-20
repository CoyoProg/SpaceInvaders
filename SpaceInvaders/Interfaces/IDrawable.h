#pragma once

/*
 * @brief Interface for all classes that needs to be draw every frame
 */
class IDrawable
{
public:
	virtual ~IDrawable() = default;
	virtual void Draw() = 0;
};

