#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include "stdafx.h"
#include "WorldObject.h"
#include <string >


namespace Render
{

	struct Renderable
	{
		Renderable(COLORREF color, Engine::WorldObjectPtr worldObj);
		virtual ~Renderable();

		virtual void render(HDC hdc) = 0;

	protected:
		COLORREF Color;
		Engine::WorldObjectPtr WorldObj;
	};
	typedef std::shared_ptr< Renderable > RenderablePtr;


	struct Line : public Renderable
	{
		Line(Engine::Point2d offset, COLORREF color, Engine::WorldObjectPtr worldObj);

		virtual void render(HDC hdc);
		
		Engine::Point2d getOffset();
		float getHeight();

	private:
		Engine::Point2d _offset;
		float _height;
	};

	struct Ellipse : public Renderable
	{
		Ellipse(Engine::Point2d size, COLORREF color, Engine::WorldObjectPtr worldObj);

		virtual void render(HDC hdc);

	private:
		Engine::Point2d _size;
	};

	struct Text : public Renderable
	{
		Text(std::wstring text, COLORREF color, Engine::WorldObjectPtr worldObj);

		virtual void render(HDC);

	private:
		std::wstring _text;
	};
}

#endif // _RENDERABLE_H_