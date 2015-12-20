#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Point2d.h"
#include <set>
#include <memory>
#include <algorithm>
#include <assert.h>

namespace Engine
{
	template< class TChildren >
	struct Object
	{
		typedef std::shared_ptr< TChildren > ChildPtr;
		typedef Object<TChildren>* ParentPtr;
		typedef std::set< ChildPtr > Children;

		void addChild(ChildPtr child);
		void removeChild(ChildPtr child);

		Children getChildren();
		ParentPtr getParent();

		Point2d getWorldPosition(Point2d localPosition = Point2d::Zero );

		Point2d getLocalAxises();
		void setLocalAxises( Point2d newAxises );

	public:
		// GEOMETRIC PROPERTIES
		Point2d Position;

	protected:
		virtual void onChildAdded(ChildPtr){}
		virtual void onChildRemoved(ChildPtr ){}


	private:
		Point2d _localAxises = Point2d(1.f, 1.f);

	private:
		// TREE STRUCTURE
		ParentPtr _parent = nullptr;
		Children _children;
	};


	template < class TChild > 
	void Object< TChild >::addChild(ChildPtr child)
	{
		// checking pointer validity, self assignment -> 
		// already being a child of object -> removing from parent -> adding to container

		assert(child.get() != nullptr || child.get() == this);

		if (_children.find(child) != _children.end())
		{
			return;
		}

		if ( child->getParent() != nullptr )
		{
			child->getParent()->removeChild( child );
		}

		_children.insert(child);
		child->_parent = this;
		onChildAdded( child );
	};

	
	template< class TChild >
	typename Object<TChild>::ParentPtr Object<TChild>::getParent()
	{
		return _parent;
	};

	template< class TChild >
	void Object< TChild >::removeChild(ChildPtr child)
	{
		// pointer validity, self assignment, being a child of this object -> removing
		assert(child.get() != nullptr || child.get() == this);

		auto found = _children.find(child);
		if (found == _children.end())
		{
			return;
		}

		_children.erase(found);
		child->_parent = nullptr;

		onChildRemoved( child );
	};


	template <class TChild > 
	typename Object<TChild>::Children  Object<TChild>::getChildren()
	{
		return _children;
	};

	
	template< class TChild >
	Point2d Object<TChild>::getWorldPosition(Point2d offset)
	{
		auto result = Position + offset;

		ParentPtr parent = _parent;
		while (parent != nullptr)
		{
			result = parent->Position + result.scale( parent->getLocalAxises() );
			parent = parent->getParent();
		}
		return result;
	}
	
	template< class TChild >
	Point2d Object<TChild>::getLocalAxises()
	{
		return _localAxises;
	}

	template< class TChild >
	void Object<TChild>::setLocalAxises(Point2d newAxises)
	{
		_localAxises.x = sign(newAxises.x);
		_localAxises.y = sign( newAxises.y );
	}
};

#endif // _OBJECT_H_