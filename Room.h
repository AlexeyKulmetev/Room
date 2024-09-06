#ifndef ROOM_H
#define ROOM_H

#include <iostream>

class TGoods {
protected:
    float _price;
    static float _extra;

public:
    TGoods(float price = 0) : _price{price} {}

    void SetPrice(float p);
    
    static void SetExtra(float extra);

    float FullPrice() const;
};


class TRect {
protected:
    float _x1;
    float _x2;

public:
    TRect(float x1 = 0, float x2 = 0) : _x1{x1}, _x2{x2} {}

    void SetSize(float x1, float x2);

    float Area() const;

    TRect& operator = (const TRect&) = default; // explicitly allowed to create bitwise assignment

    friend std::ostream& operator << (std::ostream& out, const TRect& rect);

    friend std::istream& operator >> (std::istream in, TRect& rect);
};


class TBox: public TRect {
protected:
    float _x0;

public:
    TBox(float x0 = 0, float x1 = 0, float x2 = 0) : TRect(x1, x2), _x0{x0} {}

    void SetSize(float x0);

    void SetSize(float x0, float x1, float x2);

    float Area() const; // surface area calculating method

    TRect GetRect(int i, int j); // parallelepiped edge selection method

    TRect operator () (int i, int j); // parallelepiped edge selection operator

    friend std::ostream& operator << (std::ostream& out, const TBox& box);

    friend std::istream& operator >> (std::istream& in, TBox& box);
};




#endif