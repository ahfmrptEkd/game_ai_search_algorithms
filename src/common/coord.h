#ifndef COORD_H
#define COORD_H

struct Coord
{
    int y_;
    int x_;
    Coord(const int y = 0, const int x = 0) : y_(y), x_(x) {}
    
    bool operator<(const Coord& other) const {
        return (y_ < other.y_) || (y_ == other.y_ && x_ < other.x_);
    }
    
    bool operator==(const Coord& other) const {
        return y_ == other.y_ && x_ == other.x_;
    }
};

#endif // COORD_H