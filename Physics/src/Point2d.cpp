#include "../include/Point2d.h"
#include <float.h>
#include <math.h>
#include <iostream>


namespace Engine
{

	const Point2d Point2d::Zero = Point2d(0.f, 0.f);

	Point2d::Point2d(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Point2d Point2d::operator-()
	{
		return Point2d(-x, -y);
	}

	Point2d Point2d::operator+(Point2d other)
	{
		return Point2d(x + other.x, y + other.y);
	}

	Point2d Point2d::operator-(Point2d other)
	{
		return Point2d(x - other.x, y - other.y);
	}

	Point2d Point2d::scale(Point2d other)
	{
		return Point2d(x*other.x, y*other.y);
	}

	Point2d Point2d::operator/(float scalar)
	{
		if (abs(scalar) < FLT_EPSILON)
		{
			std::cout << "dividing by zero. skipping" << std::endl;
			return *this;
		}
		return Point2d(x / scalar, y / scalar);
	}

	Point2d Point2d::operator*(float scalar)
	{
		return Point2d(x * scalar, y * scalar);
	}


	std::string Point2d::toString()
	{
		std::string res = "x = ";
		res += std::to_string(x);
		res += ", y = ";
		res += std::to_string(y);
		return res;
	}
	
}