//// 
//// The Actor Component logic comes from one of my previous projects
//// https://github.com/CoyoProg/Ogre3D_Warhammer40k/blob/main/Warhammer40k/Actors.h
////

#pragma once
#include "raylib.h"
#include "../Interfaces/IUpdatable.h"
#include "../Interfaces/IDrawable.h"

#include <memory>

class CollisionBoxComponent;

enum class ActorAffiliation
{
	Player,
	Enemy,
	Neutral,

	Default,
};

/*
 * @brief Actor is the base class for all game actors. It implements the IUpdatable and IDrawable interfaces.
 */
class Actor : public IUpdatable, public IDrawable
{
public:
	Actor(ActorAffiliation ownerP = ActorAffiliation::Player, Color colorP = RED);
	~Actor();

	virtual void Draw() override;
	virtual void Update(float deltaTimeP) override;

	Vector2 GetPosition() const { return m_position; }
	Vector2 GetSize() const { return m_size; }
	const CollisionBoxComponent* GetCollisionBoxComponent() const; // Note: We could use a template to get any component, but we only need to access CollisionBoxComponent

	// Check if the actor collides with another actor
	bool CollidesWith(const Actor& otherActorP) const;
	// Do an advanced collision check with the other actor, for complex shapes
	virtual bool AdvancedCollidesWith(const Actor& otherActorP) const { return true; };
	// Check if the actor needs an advanced collision check
	bool NeedsAdvancedCollisionCheck() const { return m_hasComplexCollision; }
	virtual void OnCollisionEvent(const Actor& otherActorP) {};

	ActorAffiliation GetActorAffiliation() const { return m_actorAffiliation; }

	void SetPosition(Vector2 positionP);
	void SetSize(Vector2 sizeP);
	void SetColor(Color colorP) { m_color = colorP; }
	void SetForDeletion(bool markedForDeletionP = true) { m_markedForDeletion = markedForDeletionP; }

	bool IsMarkedForDeletion() const { return m_markedForDeletion; }

protected:
	std::unique_ptr<CollisionBoxComponent> m_CollisionBoxComponent;

	Color m_color{ RED };
	Vector2 m_position{ 0, 0 };
	Vector2 m_size{ 50, 50 };

	bool m_markedForDeletion = false;
	bool m_hasComplexCollision = false;

	ActorAffiliation m_actorAffiliation = ActorAffiliation::Default;
};

