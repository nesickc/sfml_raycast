#include <math.h>

#include "Wall.h"

Wall::Wall(int x1, int y1, int x2, int y2) : Line(x1, y1, x2, y2, m_thickness ) {}

Wall::Wall( const Point& p1, const Point& p2 ) : Line( p1, p2, m_thickness ) { }

float Wall::distanceFrom( const Point& p ) const
{
    sf::Vector2f wallDir = GetDirection();
    sf::Vector2f wallNormal = sf::Vector2f( wallDir.y, -wallDir.x );
    sf::Vector2f a = p - m_firstPoint;
    float dot = a.x * wallNormal.x + a.y * wallNormal.y;
    Point m = { p.x - wallNormal.x * dot, p.y - wallNormal.y * dot };
    return m.DistanceTo(p);
}

float Wall::GetAngle() const
{
    return m_angle; 
}

Wall::~Wall()
{
    
}

