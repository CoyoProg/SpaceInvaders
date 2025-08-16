#pragma once
#include "raylib.h"

class Actor
{
public:
	Actor() = default;

	virtual void Draw();
	virtual void Update(float deltaTimeP) {};

	Vector2 GetPosition() const { return m_position; }
	Vector2 GetSize() const { return m_size; }

	void SetPosition(float xP, float yP) { m_position = { xP, yP }; }
	void SetSize(Vector2 sizeP) { m_size = sizeP; }
	void SetColor(Color colorP) { m_color = colorP; }

	bool MarkForDeletion() const { return m_markedForDeletion; }

protected:
	Color m_color{ RED };
	Vector2 m_position{ 0, 0 };
	Vector2 m_size{ 50, 50 };

	bool m_markedForDeletion = false;
};

