#pragma once

#include <SFML/Graphics.hpp>
#include "Point.h"
#include "Line.h"

class Wall : public Line
{
private:
    static const int m_thickness = 3;
public:
    Wall( int x1, int y1, int x2, int y2 );

    Wall( const Point& p1, const Point& p2 );

    float distanceFrom( const Point& p ) const;

    float GetAngle() const; 

    ~Wall();

private:

};