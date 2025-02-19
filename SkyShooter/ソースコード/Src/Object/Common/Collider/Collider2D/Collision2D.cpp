
#include "Collider2D.h"
#include "Collision2D.h"

using namespace std;

Collision2D::Collision2D(void)
{
    colTypeCollision_.emplace(
        COLLISION2D_TYPE::BOX_XZ, 
        [&](const Collider2D& colBox, const Collider2D& colB)
        { return IsCollisionBoxXZ(colBox, colB); }
    );
    colTypeCollision_.emplace(
        COLLISION2D_TYPE::BOX_ZY,
        [&](const Collider2D& colBox, const Collider2D& colB)
        { return IsCollisionBoxZY(colBox, colB); }
    );
    colTypeCollision_.emplace(
        COLLISION2D_TYPE::BOX_XY,
        [&](const Collider2D& colBox, const Collider2D& colB)
        { return IsCollisionBoxXY(colBox, colB); }
    );
}

Collision2D::~Collision2D(void)
{
    colTypeCollision_.clear();
}

const bool Collision2D::IsHitCollider(const Collider2D& colA, const Collider2D& colB)
{
    if (!colA.IsAbleCollider() || !colB.IsAbleCollider())
    {
        return false;
    }

    if (IsColliderType_Circle(colA.GetColliderType()) 
        && IsColliderType_Circle(colB.GetColliderType()))
    {
        //‰~“¯Žm‚Ì”»’è
        return CollisionCircle(colA, colB);
    }

    bool isColABox = IsColliderType_Box(colA.GetColliderType());
    bool isColBBox = IsColliderType_Box(colB.GetColliderType());

    if (isColABox || isColBBox)
    {
        //•½–Êã‚Ì‹éŒ`‚Æ‚Ì”»’è
        return isColABox ? IsCollisionBox(colA, colB) : IsCollisionBox(colB, colA);
    }

    return false;
}

const bool Collision2D::CollisionCircle(const Collider2D& colA, const Collider2D& colB)
{
    const auto& circleA = colA.GetColliderData();
    const auto& circleB = colB.GetColliderData();

    float radiusLen = circleA.radius + circleB.radius;

    Vector2f colAPos = { circleA.colCenterPos.x, circleA.colCenterPos.z };
    Vector2f colBPos = { circleB.colCenterPos.x, circleB.colCenterPos.z };
    
    Vector2f vec = colAPos - colBPos;

    float len = vec.Magnitude();

    return radiusLen > len;
}

const bool Collision2D::IsCollisionBox(const Collider2D& colBox, const Collider2D& col)
{
    const COLLISION2D_TYPE colType = colBox.GetColliderType();

    if (!colTypeCollision_.contains(colType))
    {
        return false;
    }

    return colTypeCollision_.at(colType)(colBox, col);
}

const bool Collision2D::IsColliderType_Circle(const COLLISION2D_TYPE colType)
{
    return COLLISION2D_TYPE::CIRCLE == colType;
}

const bool Collision2D::IsColliderType_Box(const COLLISION2D_TYPE colType)
{
    return
        COLLISION2D_TYPE::BOX_XZ == colType
        || COLLISION2D_TYPE::BOX_ZY == colType
        || COLLISION2D_TYPE::BOX_XY == colType;
}

const bool Collision2D::IsCollisionBoxXY(const Collider2D& colBox, const Collider2D& col)
{
    const auto& colBoxData = colBox.GetColliderData();
    const auto& colCircle = col.GetColliderData();

    const Vector2f boxCenter = { colBoxData.colCenterPos.x, colBoxData.colCenterPos.y };
    const Vector2f circleCenter = { colCircle.colCenterPos.x, colCircle.colCenterPos.y };

    return CollisionBox(boxCenter, circleCenter, colBoxData, colCircle);
}

const bool Collision2D::IsCollisionBoxXZ(const Collider2D& colBox, const Collider2D& col)
{
    const auto& colBoxData = colBox.GetColliderData();
    const auto& colCircle = col.GetColliderData();

    const Vector2f boxCenter = { colBoxData.colCenterPos.x, colBoxData.colCenterPos.z };
    const Vector2f circleCenter = { colCircle.colCenterPos.x, colCircle.colCenterPos.z };

    return CollisionBox(boxCenter, circleCenter, colBoxData, colCircle);
}

const bool Collision2D::IsCollisionBoxZY(const Collider2D& colBox, const Collider2D& col)
{
    const auto& colBoxData = colBox.GetColliderData();
    const auto& colCircle = col.GetColliderData();

    const Vector2f boxCenter = { colBoxData.colCenterPos.z, colBoxData.colCenterPos.y };
    const Vector2f circleCenter = { colCircle.colCenterPos.z, colCircle.colCenterPos.y };

    return CollisionBox(boxCenter, circleCenter, colBoxData, colCircle);
}

const bool Collision2D::CollisionBox(const Vector2f& boxCenterPos, const Vector2f& circleCenterPos, const ColliderData_2D& boxData, const ColliderData_2D& circleData)
{
    const Vector2f boxCenter = boxCenterPos;
    const Vector2f circleCenter = circleCenterPos;

    const float radiusPow = circleData.radius * circleData.radius;

    float left = (boxCenter - boxData.size).x;
    float right = (boxCenter + boxData.size).x;
    float top = (boxCenter + boxData.size).y;
    float bottom = (boxCenter - boxData.size).y;

    bool ret = false;

    if (
        circleCenter.x > left - circleData.radius
        && circleCenter.x < right + circleData.radius
        && circleCenter.y < top + circleData.radius
        && circleCenter.y > bottom - circleData.radius
        )
    {
        //‹éŒ`‚Ì”ÍˆÍ“à
        ret = true;

        
        if (circleCenter.x <= left)
        {
            //‰~‚Ì’†S‚ª‹éŒ`‚Ì¶‚Ì”ÍˆÍ“à
            if (circleCenter.y < top)
            {
                Vector2f vec = Vector2f(left, top) - circleCenter;
                if (vec.Magnitude() > radiusPow)
                {
                    return false;
                }
            }
            else
            {
                if (circleCenter.y > bottom)
                {
                    Vector2f vec = Vector2f(left, bottom) - circleCenter;
                    if (vec.Magnitude() > radiusPow)
                    {
                        return false;
                    }
                }
            }
        }
        else if (circleCenter.x > right)
        {
            if (circleCenter.y < top)
            {
                Vector2f vec = Vector2f(right, top) - circleCenter;
                if (vec.Magnitude() > radiusPow)
                {
                    return false;
                }
            }
            else
            {
                if (circleCenter.y > bottom)
                {
                    Vector2f vec = Vector2f(right, bottom) - circleCenter;
                    if (vec.Magnitude() > radiusPow)
                    {
                        return false;
                    }
                }
            }
        }
    }

    return ret;
}
