#include "Room.h"

const float EXTRA_FOR_ALL_OBJECTS = 0.4;

float Goods::_extra = 1.5f;


std::ostream& operator << (std::ostream& out, const Rect& rect) 
{
    out << " (" << rect._x1 << " x " << rect._x2 << ") ";
    return out;
}

std::istream& operator >> (std::istream in, Rect& rect) 
{
    in >> rect._x1 >> rect._x2;
    return in;
}

std::ostream& operator << (std::ostream& out, const Box& box) 
{
    out << " (" << box._x0 << " x " << box._x1 << " x " << box._x2 << ") ";
    return out;
}

std::istream& operator >> (std::istream& in, Box& box) {

    in >> box._x0 >> box._x1 >> box._x2;
    return in;
}

float Room::GetCost() const 
{
    float s = Area();
    for (int i = 0; i < _CarcassNumber; ++i) {
        s += _CarcassList[i].Area() * _CarcassInd[i];
    }
    return s * GoodsPrice();
}

std::ostream& operator << (std::ostream out, const Room& room) 
{
    out << " (" << room._x0 << " x " << room._x1 << " x " << room._x2 << ") " << std::endl;
    for (int i = 0; i < room._CarcassNumber; ++i) {
        out << "[" << i << "]: " << room._CarcassList[i] << " : " << room._CarcassInd[i] << "; ";
        return out;
    }
}