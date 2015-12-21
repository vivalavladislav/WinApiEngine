#ifndef _SHAPES_H_
#define _SHAPES_H_


#include "PhysicsDll.h"
#include "Point2d.h"


namespace Engine
{
	/*
		Designed to encapsulate properties of different colliders like AABB, Circle or Mesh Collider
	*/
	struct PHYS_API AABB 
	{
		Point2d Size;
	};

	struct PHYS_API Circle
	{

	};
}


#endif // _SHAPES_H_