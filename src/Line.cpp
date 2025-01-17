#include "Line.h"

#define RAD M_PI / 180.0


Line::Line( int x1, int y1, int x2, int y2, int thickness ) :
    m_firstPoint( x1, y1 ),
    m_secondPoint( x2, y2 ),
    m_thickness( thickness ),
    m_rect( sf::Vector2f( glm::distance( glm::vec2{ x1, y1 }, glm::vec2{ x2, y2 } ), thickness ) )
{
    UpdateAngle();
    InitRectangle(sf::Color(255, 255, 255, 250));
}

Line::Line(int x1, int y1, float angle, int thickness)
    : m_firstPoint(x1, y1), m_angle(angle), m_thickness(thickness),
      m_rect(sf::Vector2f(sqrt(pow(WINDOW_WIDTH, 2) + pow(WINDOW_HEIGHT, 2)), thickness))
{
    InitRectangle(sf::Color(255, 255, 255, 50));
    CalculateEndPoint();
}

Line::Line(const Point& p1, const Point& p2, int thickness)
    : m_firstPoint(p1), m_secondPoint(p2),
    m_thickness(thickness),
    m_rect( sf::Vector2f( p1.DistanceTo(p2), thickness ) )
{
    UpdateAngle();
    InitRectangle(sf::Color::White);
}

void Line::Move( const Point& destination)
{
    m_secondPoint += destination - m_firstPoint;
    m_firstPoint = destination;
    m_rect.setPosition( m_firstPoint.x, m_firstPoint.y );
}

void Line::InitRectangle(sf::Color color)
{
    m_rect.setFillColor(color);
    m_rect.setOrigin(0, m_thickness / 2);
    m_rect.setPosition(m_firstPoint.x, m_firstPoint.y);
    m_rect.setRotation(m_angle);
}

void Line::Draw(sf::RenderWindow& window) const
{
    window.draw(m_rect);
}

void Line::UpdateAngle()
{
    // updating the angle between positive x axis and the line
    // Angle is in degrees, not radians
    m_angle = atan((m_secondPoint.y - m_firstPoint.y) / (m_secondPoint.x - m_firstPoint.x)) * 180 / M_PI + 180 * (m_secondPoint.x < m_firstPoint.x); // adding 180 degrees if the second point is in II or III quarters
    m_rect.setRotation(m_angle);
}

void Line::SetLength(float length)
{
    m_secondPoint.x = m_firstPoint.x + cos(m_angle * RAD) * length;
    m_secondPoint.y = m_firstPoint.y + sin(m_angle * RAD) * length;
    m_rect.setSize(sf::Vector2f(length, m_thickness));
}

void Line::SetEndPoint( const Point& point)
{
    m_secondPoint = point;
    UpdateAngle();
    SetLength(Point::DistanceBetween(m_firstPoint, m_secondPoint));
}

void Line::SetColor( const sf::Color& color )
{
    m_rect.setFillColor( color );
}

sf::Color Line::GetColor() const
{
    return m_rect.getFillColor();
}

void Line::CalculateEndPoint()
{
    float length = sqrt(pow(m_rect.getSize().x, 2) + pow(m_rect.getSize().y, 2));
    m_secondPoint.x = m_firstPoint.x + cos(m_angle * RAD) * length;
    m_secondPoint.y = m_firstPoint.y + sin(m_angle * RAD) * length;
}

bool Line::Contains(const Point& point) const
{
    static const float epsilon = 0.01;
    // checking if the poin is inside the line's rectangle
    if ( ( point.x >= m_firstPoint.x  && point.x <= m_secondPoint.x ||
           point.x >= m_secondPoint.x && point.x <= m_firstPoint.x ) &&
         ( point.y >= m_firstPoint.y  && point.y <= m_secondPoint.y  ||
           point.y >  m_secondPoint.y && point.y <  m_firstPoint.y ) )
    {
        if ( m_secondPoint.y == m_firstPoint.y )
        {
            if (point.y == m_secondPoint.y)
                return true;
            return false;
        }
        if ( m_secondPoint.x == m_firstPoint.x )
        {
            if (point.x == m_secondPoint.x)
                return true;
            return false;
        }
        // checking if the point is on the line
        if(abs( (point.y - m_firstPoint.y) / (m_secondPoint.y - m_firstPoint.y) - 
                (point.x - m_firstPoint.x) / (m_secondPoint.x - m_firstPoint.x) ) < epsilon)
            return true;
    }
    return false;
}

glm::vec2 Line::GetDirection() const
{
    auto vec = m_secondPoint - m_firstPoint;
    return vec / glm::length(vec);
}

Point Line::FindIntersection( const Line& l2 ) const
{
    Point intercept( INF, INF );

    KBCoefficients kbThis = FindKBCoeffs();
    float k1 = kbThis.k;
    float b1 = kbThis.b;

    KBCoefficients kbOther = l2.FindKBCoeffs();
    float k2 = kbOther.k;
    float b2 = kbOther.b;

    // point of interception is found as the solution of the system
    // {y = k1 * x + b1
    // {y = k2 * x + b2
    // 
    // or k1 * x + b1 = k2 * x + b2
    // continue: k1 * x - k2 * x = b2 - b1
    // x = (b2 - b1) / (k1 - k2); y = k1 * (b2 - b1) / (k1 - k2) - b1
    // If some of the lines are vertical, things get more complicated in some sense.
    // if this line is vertical then x coordinate of interception is obviously the x coordinate of this line
    // the y coordinate then is found from y = k2 * x1 + b2,  where x1 - x coordinate of this line.
    // 
    // If lines are parallel then there's no or infinite number of interceptions,
    // which means both of these cases are irrelevant 

    if ( k1 == k2 )
        return intercept;

    if ( k1 != INF )
    {
        if ( k2 == INF )
        {
            intercept.x = l2.m_firstPoint.x;
            intercept.y = k1 * intercept.x + b1;
        }
        else
        {
            intercept.x = (b2 - b1) / (k1 - k2);
            intercept.y = k1 * intercept.x + b1;
        }       
    }
    else
    {
        intercept.x = m_firstPoint.x;
        intercept.y = k2 * intercept.x + b2;
    }

    return intercept;
}

Line::KBCoefficients Line::FindKBCoeffs() const
{
    float k = 0;
    float b = 0;
    if ( (m_secondPoint.x - m_firstPoint.x) == 0 ) // if the line is vertical set k to be inf
    {
        k = INF;
        b = -INF;
    }
    else
    {
        // line equation from points is (y-y1)/(y1-y2) = (x-x1)/(x1-x2) 
        // or, which is basically the same (y-y2)/(y2-y1) = (x-x2)/(x2-x1). 
        // That means order of points is irrelevant. You can check it on desmos.com for example.
        // https://www.desmos.com/calculator/woj2nfulgf
        // Choose the first one: (y-y1)/(y1-y2) = (x-x1)/(x1-x2) 
        // therefore y = (x-x1)(y1-y2)/(x1-x2) + y1
        // expand: y = x(y1-y2)/(x1-x2) - x1(y1-y2)/(x1-x2) + y1
        // equation is y = kx+b therefore k = (y1-y2)/(x1-x2); b = y1 - x1(y1-y2)/(x1-x2).

        k = (m_secondPoint.y - m_firstPoint.y) / (m_secondPoint.x - m_firstPoint.x);
        b = m_firstPoint.y - k * m_firstPoint.x;
    }
    return Line::KBCoefficients{ k, b };
}


Line::~Line()
{
}