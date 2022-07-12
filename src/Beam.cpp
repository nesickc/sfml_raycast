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

void Beam::CheckCollision( const std::vector<Wall>& walls )
{
    //setting second point to be outside the screen
    if ( m_secondPoint.x < WINDOW_WIDTH && m_secondPoint.x > 0 && m_secondPoint.y < WINDOW_HEIGHT && m_secondPoint.y > 0 )
        SetLength( WINDOW_WIDTH + WINDOW_HEIGHT );

    // calculating coefficients for equation y = kx + b
    // if two points are on one parallel to y line, assign k = INF
    sf::Vector2f kbCoefs = FindKBCoeffs();
    float k_beam = kbCoefs.x;
    float b_beam = kbCoefs.y;

    const Wall* closestWall = nullptr;
    // for every wall look for intersections.
    for ( auto& wall : walls )
    {
        if ( m_wallToIgnore && &wall == m_wallToIgnore )
            continue;
        
        sf::Vector2f wallKBCoefs = wall.FindKBCoeffs();
        float k_wall = wallKBCoefs.x;
        float b_wall = wallKBCoefs.y;

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

void Beam::Move( Point& destination )
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

    sf::Vector2f wallDir = wall.GetDirection();
    sf::Vector2f wallNormal = sf::Vector2f( wallDir.y, -wallDir.x);
    sf::Vector2f beamDir = GetDirection();
    float dot = wallNormal.x * beamDir.x + wallNormal.y * beamDir.y;
    sf::Vector2f beamOrientation = GetDirection() - 2.f * dot * wallNormal;
    float newAngle = atan( beamOrientation.y / beamOrientation.x ) * 180 / M_PI + 180 * (beamOrientation.x < 0);

    m_reflectedBeam = std::make_shared<Beam>( m_secondPoint.x, m_secondPoint.y, newAngle, m_reflectionDepth + 1 );
    m_reflectedBeam->setWallToIgnore( wall );
    sf::Color newColor = GetColor();
    newColor.a *= 0.8;
    m_reflectedBeam->SetColor( newColor );

    return true;
}

void Beam::setWallToIgnore( const Wall& wall )
{
    m_wallToIgnore = &wall;
}
