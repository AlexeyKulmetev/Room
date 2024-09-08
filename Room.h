#ifndef ROOM_H
#define ROOM_H

#include <iostream>

class Goods {
protected:
    float _price;
    static float _extra;

public:
    Goods(float price = 0) : _price{price} {}

    void SetPrice(float price) { _price = price; }
    
    static void SetExtra(float extra) { _extra = extra; }

    float GoodsPrice() const { return _price * (1 + _extra); }
};


class Rect {
protected:
    float _x1; // length
    float _x2; // width

public:
    Rect(float x1 = 0, float x2 = 0) : _x1{x1}, _x2{x2} {}

    void SetSize(float x1, float x2) 
    { 
        _x1 = x1 < 0 ? 0 : x1;
        _x2 = x2 < 0 ? 0 : x2;
    }

    float Area() const { return _x1 * _x2; } // calculating area

    Rect& operator = (const Rect&) = default; // explicitly allowed to create bitwise assignment

    friend std::ostream& operator << (std::ostream& out, const Rect& rect);

    friend std::istream& operator >> (std::istream in, Rect& rect);
};


class Box: public Rect {
protected:
    float _x0; // height

public:
    Box(float x0 = 0, float x1 = 0, float x2 = 0) : Rect(x1, x2), _x0{x0} {}

    void SetSize(float x0) { _x0 = x0; }

    void SetSize(float x0, float x1, float x2) 
    {
        Rect::SetSize(x1, x2);
        _x0 = x0;
    }

    float Area() const // surface area calculating method
    {
        return 2 * _x1 * _x2 + 2 * _x0 * (_x1 + _x2);
    }

    Rect GetRect(int i, int j) // parallelepiped edge selection method
    {
        float tmp[3] = {_x0, _x1, _x2};
        return Rect(tmp[i], tmp[j]);
    }

    Rect operator () (int i, int j) // parallelepiped edge selection operator
    {
        return Rect(i, j);
    }

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
        if (!_CarcassList) 
        { 
            ErrorState = TErr::BAD_ALLOC; 
            return;
        }
        _CarcassInd = new int[MaxRect];
        if (!_CarcassInd) 
        { 
            delete[] _CarcassList; 
            ErrorState = TErr::BAD_ALLOC;
            return;    
        }
    }

    Room(Room&& room); // move constructor

    ~Room() {
        if (_CarcassList) delete[] _CarcassList;
        if (_CarcassInd) delete[] _CarcassInd;
    }

    TErr ErrorState() const {
        return ErrorState;
    }

    void ClearErrorState() {
        ErrorState = TErr::OK;
    }

    Room& operator += (const Rect& rect);

    Room& operator -= (const Rect& rect);

    Room& operator += (const Rects rects);
    
    float GetCost(); // returns full price of repair works

    Rect& operator [] (const int& i); // access to element by index

    void Copy(const Room& room);

    Room(const Room& room);

    Room& operator = (const Room& room);

    Room& operator = (Room&& room);

    void Move(Room& room); // move data
};


#endif