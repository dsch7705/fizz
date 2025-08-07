#pragma once

#include <ostream>


struct DVec2 
{
    double x;
    double y;

    void scale(double factor);
    static DVec2 scale(const DVec2& vec, double factor);

    void add(const struct DVec2& other);
    // Adds the second vector to the first
    static DVec2 add(const DVec2& first, const DVec2& second);

    void sub(const DVec2& other);
    // Subtracts the second vector from the first
    static DVec2 sub(const DVec2& first, const DVec2& second);

    double mag() const;
    static double mag(const DVec2& vec);

    void normalize();
    static DVec2 normalize(const DVec2& vec);

    void operator+=(const DVec2& other)
    {
        add(other);
    }
    void operator-=(const DVec2& other)
    {
        sub(other);
    }
    void operator*=(double factor)
    {
        scale(factor);
    }

    DVec2 operator+(const DVec2& other) const
    {
        return add(*this, other);
    }
    DVec2 operator-(const DVec2& other) const
    {
        return sub(*this, other);
    }
    DVec2 operator-(void) const
    {
        return scale(*this, -1);
    }
    DVec2 operator*(double factor) const
    {
        return scale(*this, factor);
    }
};

std::ostream& operator<<(std::ostream& os, const DVec2& vec);