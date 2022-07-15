#include <math.h>

#include "Wall.h"

Wall::Wall(int x1, int y1, int x2, int y2) : Line(x1, y1, x2, y2, m_thickness ) {}

Wall::Wall( const Point& p1, const Point& p2 ) : Line( p1, p2, m_thickness ) { }

float Wall::distanceFrom( const Point& p ) const
{
    glm::vec2 wallDir = GetDirection();
    glm::vec2 wallNormal = glm::normalize( glm::vec2{ wallDir.y, -wallDir.x } );
    glm::vec2 a = p - m_firstPoint;
    float dot = glm::dot( a, wallNormal );
    Point m{ p - wallNormal * dot };
    return m.DistanceTo(p);
}

float Wall::GetAngle() const
{
    return m_angle; 
}

Wall::~Wall()
{
    
}

