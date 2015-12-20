#ifndef _WORLD_OBJECT_H_
#define _WORLD_OBJECT_H_

#include "PhysicsDll.h"
#include "Object.h"
#include "ObjectComponent.h"
#include <vector>

namespace Engine
{
	struct World;
	struct PHYS_API WorldObject : public Object< WorldObject >
	{
		typedef std::unique_ptr< IComponent > ComponentPtr;
		
		WorldObject();
		WorldObject(const WorldObject&) = delete;
		WorldObject& operator=(const WorldObject&) = delete;

		void onAddedToWord(World*);
		void onRemovedFromWorld(World*);

		void addComponent( ComponentPtr component );
		void removeComponent( IComponent* component );
		
		World* getWorld();

		std::string Name;

	protected:
		virtual void onChildAdded(std::shared_ptr< WorldObject > );
		virtual void onChildRemoved(std::shared_ptr< WorldObject > );

	private:
		World* _world = nullptr;

		std::vector< ComponentPtr > _components;
	};
	typedef std::shared_ptr< WorldObject > WorldObjectPtr;

}

#endif // WORLD_OBJECT_H