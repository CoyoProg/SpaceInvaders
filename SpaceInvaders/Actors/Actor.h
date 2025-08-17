//// 
//// The Actor Component logic comes from one of my previous projects
//// https://github.com/CoyoProg/Ogre3D_Warhammer40k/blob/main/Warhammer40k/Actors.h
////



#pragma once
#include "raylib.h"
#include "../Interfaces/IUpdatable.h"

#include <memory>

class CollisionBoxComponent;

class Actor : public IUpdatable
{
public:
	Actor();
	~Actor();

	virtual void Draw();
	virtual void Update(float deltaTimeP) override;

	Vector2 GetPosition() const { return m_position; }
	Vector2 GetSize() const { return m_size; }
	const CollisionBoxComponent* GetCollisionBoxComponent() const; // Note: We could use a template to get any component, but we only need to access CollisionBoxComponent
	virtual void OnCollisionEvent(const Actor& otherActorP) {};

	void SetPosition(float xP, float yP) { m_position = { xP, yP }; }
	void SetSize(Vector2 sizeP) { m_size = sizeP; }
	void SetColor(Color colorP) { m_color = colorP; }

	bool MarkForDeletion() const { return m_markedForDeletion; }

protected:
	std::unique_ptr<CollisionBoxComponent> m_CollisionBoxComponent;

	Color m_color{ RED };
	Vector2 m_position{ 0, 0 };
	Vector2 m_size{ 50, 50 };

	bool m_markedForDeletion = false;
};

