#pragma once

#include "proj_data.h"
#include "Line.h"
#include "Wall.h"
#include <optional>

class Beam : public Line
{
public:
    Beam(int x, int y, float angle, int reflectionDepth = 1);

    void CheckCollision(const std::vector<Wall>& walls);

    uint32_t ReflectionDepth();

    void Draw( sf::RenderWindow& window ) const override;

    void Move( Point& destination ) override;

    static void SetMaxReflectionDepth( uint32_t newDepth );

    ~Beam();

private:
    bool Reflect( const Wall& wall);

    void setWallToIgnore( const Wall& wall );

private:
    std::shared_ptr<Beam> m_reflectedBeam = nullptr;
    const Wall* m_wallToIgnore;
    int m_reflectionDepth;
    static int m_maxReflectionDepth;
};

