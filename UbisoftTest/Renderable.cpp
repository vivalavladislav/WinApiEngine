#include "stdafx.h"
#include "Renderable.h"

using namespace Engine;
using namespace std;


namespace Render
{


	Renderable::Renderable(COLORREF color, WorldObjectPtr worldObj)
		: WorldObj(worldObj), Color(color)
	{
	}

	Renderable::~Renderable()
	{
	}


	Line::Line(Point2d offset, COLORREF color, WorldObjectPtr worldObj)
		: Renderable(color, worldObj), _offset(offset), _height(8)
	{
	}

	Point2d Line::getOffset()
	{
		return _offset;
	}

	float Line::getHeight()
	{
		return _height;
	}

	void Line::render(HDC hdc)
	{
		auto pen = CreatePen(PS_SOLID, _height, Color);

		SelectObject(hdc, pen);

		auto start = WorldObj->getWorldPosition();
		auto end = WorldObj->getWorldPosition(_offset);

		MoveToEx(hdc, start.x, start.y, nullptr);
		LineTo(hdc, end.x, end.y);

		DeleteObject(pen);
	}


	Ellipse::Ellipse(Point2d size, COLORREF color, WorldObjectPtr worldObj)
		: Renderable(color, worldObj), _size(size)
	{
	}

	void Ellipse::render(HDC hdc)
	{
		auto pen = CreatePen(PS_SOLID, 8, Color);
		SelectObject(hdc, pen);

		auto bottom = WorldObj->getWorldPosition( -_size / 2.f);
		auto top = WorldObj->getWorldPosition(_size / 2.f);

		::Ellipse( hdc, bottom.x, bottom.y, top.x, top.y );

		DeleteObject(pen);
	}


	Text::Text(wstring text, COLORREF color, WorldObjectPtr worldObj)
		:Renderable(color, worldObj), _text( text)
	{
	}

	void Text::render(HDC hdc )
	{
		auto pen = CreatePen(PS_SOLID, 8, Color);
		SelectObject(hdc, pen);

		auto pos = WorldObj->getWorldPosition();
		TextOut(hdc, pos.x, pos.y, _text.c_str(), _text.length() );

		DeleteObject( pen );
	}
}