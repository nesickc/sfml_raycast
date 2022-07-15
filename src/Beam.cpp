#include <math.h>
#include <iostream>
#include <algorithm>
#include "Beam.h"

int Beam::m_maxReflectionDepth = 0;

Beam::Beam( int x, int y, float angle, int reflectionDepth ) :
    Line( x, y, angle, 2 ),
    m_reflectionDepth( reflectionDepth )
{
}

Beam::Beam( const Point& point, float angle, int reflectionDepth ) :
    Line( (int)point.x, (int)point.y, angle, 2 ),
    m_reflectionDepth( reflectionDepth )
{
}

void Beam::CheckCollision( const std::vector<Wall>& walls )
{
    //setting second point to be outside the screen
    if ( m_secondPoint.x < WINDOW_WIDTH && m_secondPoint.x > 0 && m_secondPoint.y < WINDOW_HEIGHT && m_secondPoint.y > 0 )
        SetLength( WINDOW_WIDTH + WINDOW_HEIGHT );

    const Wall* closestWall = nullptr;
    // for every wall look for intersections.
    for ( auto& wall : walls )
    {
        if ( m_wallToIgnore && &wall == m_wallToIgnore )
            continue;

        Point intercept = FindIntersection( wall );

        if ( intercept.x == INF )
            continue;

        if ( Contains( intercept ) && wall.Contains( intercept ) )
        {
            SetLength( m_firstPoint.DistanceTo( intercept ) );
            closestWall = &wall;
        }
    }
    if ( closestWall && Reflect( *closestWall ) )
    {
        m_reflectedBeam->CheckCollision( walls );
    }
}

uint32_t Beam::ReflectionDepth()
{
    return m_reflectionDepth;
}

void Beam::Draw( sf::RenderWindow& window ) const
{
    Line::Draw( window );
    if ( m_reflectedBeam )
        m_reflectedBeam->Draw( window );
}

void Beam::Move( const Point& destination )
{
    Line::Move( destination );
    m_reflectedBeam = nullptr;
}

void Beam::SetMaxReflectionDepth( uint32_t newDepth )
{
    m_maxReflectionDepth = newDepth;
}

Beam::~Beam()
{
}


bool Beam::Reflect( const Wall& wall )
{
    if ( m_reflectionDepth > m_maxReflectionDepth )
        return false;

    glm::vec2 wallDir = wall.GetDirection();
    glm::vec2 wallNormal = glm::vec2( wallDir.y, -wallDir.x);
    glm::vec2 beamDir = GetDirection();
    float dot = glm::dot( beamDir, wallNormal );
    glm::vec2 beamOrientation = GetDirection() - 2.f * dot * wallNormal;
    float newAngle = atan( beamOrientation.y / beamOrientation.x ) * 180.0f / (float)M_PI + 180.0f * (beamOrientation.x < 0);

    m_reflectedBeam = std::make_shared<Beam>( m_secondPoint, newAngle, m_reflectionDepth + 1 );
    m_reflectedBeam->setWallToIgnore( wall );
    sf::Color newColor = GetColor();
    newColor.a *= 0.8f;
    m_reflectedBeam->SetColor( newColor );

    return true;
}

void Beam::setWallToIgnore( const Wall& wall )
{
    m_wallToIgnore = &wall;
}
