#ifndef _POINT2D_H_
#define _POINT2D_H_

#include "PhysicsDll.h"
#include <string>

namespace Engine
{

	struct PHYS_API Point2d
	{
		float x = 0.f;
		float y = 0.f;
	
		static const Point2d Zero;

		Point2d(float x = 0.f, float y = 0.f);

		Point2d operator-();

		Point2d operator+(Point2d other);
		Point2d operator-(Point2d other);

		Point2d operator/(float scalar );
		Point2d operator*(float scalar);
		Point2d scale(Point2d other);

		std::string toString();
	};
}
#endif // _POINT2D_H_