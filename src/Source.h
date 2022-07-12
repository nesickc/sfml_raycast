#pragma once
#include "proj_data.h"
#include "Beam.h"

class Source
{
public:
    Source(int x, int y, int beamCount);

    /// default constructor with default number of rays
    Source();

    /// default destructor (no data needs to be explicitly released so it can remain empty)
    ~Source();

    /// draw the beams that the source holds
    void Draw(sf::RenderWindow &window) const;

    /// move the source to the destination
    void Move(Point destination);

    /// ckeck beams' collisions 
    void CheckWalls(const std::vector<Wall> &walls);

    /// set new number of beams
    void SetBeamCount(int count);

    /// set new number of reflections for the beams
    void SetReflectionsNumber( int number );

    /// sets the color for all the rays
    void SetBeamColor( sf::Color& color );
private:

public:
    static constexpr int DEFAULT_BEAM_COUNT = 720;

private:
    Point m_point;
    int m_beamCount = 0;
    std::vector<Beam> m_beams;
};