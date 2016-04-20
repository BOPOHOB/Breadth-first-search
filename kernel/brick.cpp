#include "brick.h"

#include <cassert>

Brick::Brick()
{

}

Brick::Brick(const Size& s, const Point& p, const Stars& star) :
    p(p), s(s), star(star) {}


Brick::~Brick()
{

}

void Brick::move(const Point& d)
{
    p += d;
}

void Brick::moveLeft()
{
    p.rx() -= 1;
}

void Brick::moveRight()
{
    p.rx() += 1;
}

void Brick::moveBottom()
{
    p.ry() += 1;
}

void Brick::moveTop()
{
    p.ry() -= 1;
}
