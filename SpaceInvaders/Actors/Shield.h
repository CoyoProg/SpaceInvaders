////
//// Some source code for the Shield construction and destruction
//// https://ronjeffries.com/articles/020-invaders/i-20/
////

#pragma once
#include "Actor.h"
#include <vector>

class Shield : public Actor
{
public:
    Shield(Vector2 positionP, float widthP, float heightP);

    virtual void Draw() override;
    virtual bool AdvancedCollidesWith(const Actor& otherActorP) const override;
    virtual void OnCollisionEvent(const Actor& otherActorP);

	void ExplodeCell(int x, int y);

    void ResizePatternToCells();
    void RemoveExtraCells();
    void UpdateTrianglesCells();

public:
    float width, height;   
	float cellWidth, cellHeight;    
    std::vector<std::vector<int>> cells; // if 0, the the cell is empty

    RenderTexture2D renderTex;

private:
    Vector2 ConvertWorldToGrid(const Vector2& worldPos) const;
	mutable Vector2 m_lastHitPosition{ 0, 0 };
};

