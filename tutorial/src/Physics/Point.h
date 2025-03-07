#ifndef POINT_H
#define POINT_H

#include <iostream>

using namespace std;

class Point {
public:
    float X, Y;

public:
    Point(float X = 0, float Y = 0) : X(X), Y(Y) {}

    inline Point operator+(const Point &p2) const {
        return Point(X + p2.X, Y + p2.Y);
    }

    inline Point& operator+=(const Point &p2) {
        X += p2.X;
        Y += p2.Y;
        return *this;
    }

    inline Point operator-(const Point &p2) const {
        return Point(X - p2.X, Y - p2.Y);
    }

    inline Point& operator-=(const Point &p2) {
        X -= p2.X;
        Y -= p2.Y;
        return *this;
    }

    inline Point operator*(const float scalar) const {
        return Point(X * scalar, Y * scalar);
    }

    void Log(const string &msg = "") const {
        cout << msg << "(X, Y) = (" << X << ", " << Y << ")" << "\n";
    }
};

#endif // POINT_H
