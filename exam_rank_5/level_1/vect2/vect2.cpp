#include "vect2.hpp"

vect2::vect2() : _x(0), _y(0)
{
	
}

vect2::vect2(int x, int y) : _x(x), _y(y)
{
	
}

vect2::vect2(const vect2& other) : _x(other._x), _y(other._y)
{
	
}

vect2&	vect2::operator=(const vect2& other)
{
	if (this != &other)
	{
		_x = other._x;
		_y = other._y;
	}
	return (*this);
}

vect2::~vect2()
{
	
}

int		vect2::operator[](int index) const
{
	return (index == 0) ? _x : _y;
}

int&	vect2::operator[](int index)
{
	return (index == 0) ? _x : _y;
}

vect2&	vect2::operator++()
{
	_x++;
	_y++;
	return (*this);
}

vect2	vect2::operator++(int)
{
	vect2 tmp(*this);
	_x++;
	_y++;
	return (tmp);
}

vect2&	vect2::operator--()
{
	_x--;
	_y--;
	return (*this);
}

vect2	vect2::operator--(int)
{
	vect2 tmp(*this);
	_x--;
	_y--;
	return (tmp);
}

vect2& vect2::operator*=(int other)
{
	_x *= other;
	_y *= other;
	return (*this);
}

vect2 vect2::operator*(int other) const
{
	vect2 tmp(*this);
	tmp._x = _x * other;
	tmp._y = _y * other;
	return (tmp);
}
		
vect2& vect2::operator+=(const vect2& other)
{
	_x += other._x;
	_y += other._y;
	return (*this);
}

vect2 vect2::operator+(const vect2& other) const
{
	vect2 tmp(*this);
	tmp._x = _x + other._x;
	tmp._y = _y + other._y;
	return (tmp);
}
		
vect2& vect2::operator-=(const vect2& other)
{
	_x -= other._x;
	_y -= other._y;
	return (*this);
}

vect2 vect2::operator-(const vect2& other) const
{
	vect2 tmp(*this);
	tmp._x = _x - other._x;
	tmp._y = _y - other._y;
	return (tmp);
}

vect2 vect2::operator-() const
{
	vect2 tmp(*this);
	tmp._x = -_x;
	tmp._y = -_y;
	return (tmp);
}

bool vect2::operator!=(const vect2& other) const
{
	return (_x == other._x && _y == other._y) ? false : true;
}

bool vect2::operator==(const vect2& other) const
{
	return (_x == other._x && _y == other._y) ? true : false;
}

vect2 operator*(int num, const vect2& to_mult)
{
	vect2 temp(to_mult);
	temp *= num;
	return temp;
}

std::ostream& operator<<(std::ostream& os, const vect2& toPrint)
{
	os << "{" << toPrint[0] << ", " << toPrint[1] << "}";
	return os;
}