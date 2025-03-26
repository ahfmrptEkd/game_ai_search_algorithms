#ifndef COORD_H
#define COORD_H

struct Coord
{
    int y_;
    int x_;
    Coord(const int y = 0, const int x = 0) : y_(y), x_(x) {}
};

#endif // COORD_H