#ifndef _PHYSICS_DLL_H_
#define _PHYSICS_DLL_H_

#include <functional>
#include <memory>

#ifdef PHYSICS_EXPORTS
#define PHYS_API __declspec(dllexport)
#else
#define PHYS_API __declspec(dllimport)
#endif

template <class TNumber >
TNumber sign(TNumber value )
{
	return value >= 0.f ? 1.f : -1.f;
};

typedef std::function< void(float) > UpdateF;
typedef std::shared_ptr< UpdateF > FuncPtr;


#endif // _PHYSICS_DLL_H_