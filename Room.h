#ifndef ROOM_H
#define ROOM_H

#include <iostream>

class Goods {
protected:
    float _price;
    static float _extra;

public:
    Goods(float price = 0) : _price{price} {}

    void SetPrice(float p);
    
    static void SetExtra(float extra);

    float FullPrice() const;
};


class Rect {
protected:
    float _x1;
    float _x2;

public:
    Rect(float x1 = 0, float x2 = 0) : _x1{x1}, _x2{x2} {}

    void SetSize(float x1, float x2);

    float Area() const;

    Rect& operator = (const Rect&) = default; // explicitly allowed to create bitwise assignment

    friend std::ostream& operator << (std::ostream& out, const Rect& rect);

    friend std::istream& operator >> (std::istream in, Rect& rect);
};


class Box: public Rect {
protected:
    float _x0;

public:
    Box(float x0 = 0, float x1 = 0, float x2 = 0) : Rect(x1, x2), _x0{x0} {}

    void SetSize(float x0);

    void SetSize(float x0, float x1, float x2);

    float Area() const; // surface area calculating method

    Rect GetRect(int i, int j); // parallelepiped edge selection method

    Rect operator () (int i, int j); // parallelepiped edge selection operator

    friend std::ostream& operator << (std::ostream& out, const Box& box);

    friend std::istream& operator >> (std::istream& in, Box& box);
};

    enum class TErr {OK, BUFFER_OVERFLOW, BAD_ALLOC};

class Room : public Box, public Goods {
public:
    struct Rects: Rect {
        int _count;
        // Copy constructor "Rect(const Rect& other)"" will be implicitly created
        Rects(const Rect& rect, int count) : Rect(rect), _count{count} {} 
    };

private:
    Rect *_CarcassList{nullptr};
    int  *_CarcassInd;
    int MaxRect;
    int CarcassNumber;
    TErr ErrorState{TErr::OK};
    // Element for links whin incorrectly accessed by index
    static Rect BadRect;

public:
    Room(int maxrect = 10) : Box(), Goods() {
        if (maxrect < 1) maxrect = 1;
        MaxRect = maxrect;
        _CarcassList = new Rect[MaxRect];
        _CarcassInd = new int[MaxRect];
    }

    ~Room() {
        if (_CarcassList) delete[] _CarcassList;
        if (_CarcassInd) delete[] _CarcassInd;
    }
};


#endif