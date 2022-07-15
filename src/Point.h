#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm.hpp>

/// basic point class with distance added
class Point : public glm::vec2
{
public:
    inline float DistanceTo(const Point& other) const
    {
        return glm::length(*this - other);
    }
    inline static float DistanceBetween(const Point& first, const Point& second)
    {      
        return glm::length( first - second );
    }
    Point(float x_, float y_) : glm::vec2(x_, y_) { }

    Point( const sf::Vector2i& vec  ) : glm::vec2( vec.x, vec.y ) { }
    Point( const glm::vec2& vec ) : glm::vec2( vec ) { }

    Point() : glm::vec2(0, 0) { }
};
