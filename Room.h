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
    int  *_CarcassInd{nullptr};
    int _MaxRect;
    int _CarcassNumber{ 0 }; 
    TErr _ErrorState{TErr::OK};
    // Element for links whin incorrectly accessed by index
    static Rect _BadRect;

public:
    Room(int maxrect = 10) {
        if (maxrect < 1) maxrect = 1;
        _MaxRect = maxrect;
        _CarcassList = new Rect [_MaxRect];
        if (!_CarcassList) {
            _ErrorState = TErr::BAD_ALLOC;
            return;
        }
        _CarcassInd = new int [_MaxRect];
        if (!_CarcassInd) {
            _ErrorState = TErr::BAD_ALLOC;
            delete[] _CarcassList;
            return;
        }
    }

    Room(Room&& room)
    {
        Move(room);
    }

    ~Room() {
        if (_CarcassList) delete[] _CarcassList;
        if (_CarcassInd) delete[] _CarcassInd;
    }

    TErr ErrorState() const {
        return _ErrorState;
    }

    void ClearErrorState() {
        _ErrorState = TErr::OK;
    }

    // adding a partition
    Room& operator += (const Rect& rect) {
        if (_CarcassNumber < _MaxRect) {
            _CarcassList[_CarcassNumber] = rect;
            _CarcassInd[_CarcassNumber++] = +1;
        }
        else
            _ErrorState = TErr::BUFFER_OVERFLOW;
        return *this;
    }

    // adding a opening
    Room& operator -= (const Rect& rect) {
        if (_CarcassNumber < _MaxRect) {
            _CarcassList[_CarcassNumber] = rect;
            _CarcassInd[_CarcassNumber++] = -1;
        }
        else
            _ErrorState = TErr::BUFFER_OVERFLOW;
        return *this;
    }

    // adding a partition group
    Room& operator += (const Rects rects) {
        if (_CarcassNumber < _MaxRect) {
            _CarcassList[_CarcassNumber] = Rect(rects);
            _CarcassInd[_CarcassNumber++] = rects._count;
        }
        else
            _ErrorState = TErr::BUFFER_OVERFLOW;
        return *this;
    }
    
    float GetCost() const; // returns full price of repair works

    Rect& operator [] (const int& i) { // access to element by index
        if (i >=0 && i < _CarcassNumber)
            return _CarcassList[i];
        else
            return _BadRect;
    }

    void Copy(const Room& room)
    {
        *(static_cast<Box*>(this)) = *(static_cast<Box*>(const_cast<Room*>(&room)));
        *(static_cast<Goods*>(this)) = *(static_cast<Goods*>(const_cast<Room*>(&room)));
        _MaxRect = room._MaxRect;
        _CarcassNumber = room._CarcassNumber;
        _CarcassList = new Rect [_MaxRect];
        _CarcassInd = new int [_MaxRect];
        for (int i = 0; i < _CarcassNumber; ++i) {
            _CarcassList[i] = room._CarcassList[i];
            _CarcassInd[i] = room._CarcassInd[i];
        }
        std::cout << "Copy" << std::endl;
    }

    Room(const Room& room)
    {
        Copy(room);
    }

    Room& operator = (const Room& room)
    {
        if (_CarcassList) delete[] _CarcassList;
        if (_CarcassInd) delete[] _CarcassInd;
        Copy(room);
        std::cout << " = &" << std::endl;
        return *this;
    }

    Room& operator = (Room&& room)
    {
        if (_CarcassList) delete[] _CarcassList;
        if (_CarcassInd) delete[] _CarcassInd;
        Copy(room);
        std::cout << " = &&" << std::endl;
        return *this;
    }

    friend std::ostream& operator << (std::ostream out, const Room& room);

    void Move(Room& room)
    {
        *(static_cast<Box*>(this)) = *(static_cast<Box*>(&room)); 
        *(static_cast<Goods*>(this)) = *(static_cast<Goods*>(&room));
        _MaxRect = room._MaxRect;
        _CarcassNumber = room._CarcassNumber;
        _CarcassList = room._CarcassList;
        room._CarcassList = nullptr;
        _CarcassInd = room._CarcassInd;
        room._CarcassInd = nullptr;
        std::cout << "Move" << std::endl;
    }
};


#endif