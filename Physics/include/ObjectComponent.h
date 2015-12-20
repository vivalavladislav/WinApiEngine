#ifndef _OBJECT_COMPONENT_H_
#define _OBJECT_COMPONENT_H_

#include "PhysicsDll.h"
#include <memory>

namespace Engine
{
	struct World;
	struct WorldObject;

	struct PHYS_API IComponent
	{
		typedef std::weak_ptr< WorldObject > ObjPtr;

		IComponent(ObjPtr obj);
		virtual ~IComponent();

		virtual void onAddedToWorld(World *) = 0;
		virtual void onRemovedFromWorld(World*) = 0;

	protected:
		ObjPtr Object;
	};


	struct PHYS_API UpdatableComponent : public IComponent
	{
		UpdatableComponent(ObjPtr object);
		~UpdatableComponent();

		virtual void onAddedToWorld(World*) override;
		virtual void onRemovedFromWorld(World*) override;

		virtual void update( float dt ) = 0;

	private:
		FuncPtr _updateCallback;
	};

	struct PHYS_API MoveUp : public UpdatableComponent
	{
		MoveUp( ObjPtr );

		virtual void update(float dt);
	};
}

#endif // _OBJECT_COMPONENT_H_