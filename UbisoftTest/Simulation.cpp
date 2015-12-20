#include "stdafx.h"
#include "UbisoftTest.h"
#include "ObjectComponent.h"
#include "Rigidbody.h"

#include <chrono>


using namespace Engine;
using namespace Render;
using namespace std;

Simulation::Simulation( Point2d windowSize )
// 20 pixels is one meter
: _world(PhysicProperties{ 1/20.f })
{
	_world.Position = Point2d( 60, windowSize.y - 100 );
	// swap y axis to make a standart look
	_world.setLocalAxises(Point2d(1, -1));

	// creating objects to be tracked by engine
	auto yAxisEngine = make_shared< WorldObject >();
	_world.addChild( yAxisEngine );

	auto xAxisEngine = make_shared< WorldObject >();
	_world.addChild( xAxisEngine );

	auto startEllipse = make_shared< WorldObject >();
	startEllipse->Position = Point2d( 0, 500 );
	_world.addChild(startEllipse);


	auto rigidbody = unique_ptr< Rigidbody >(new Rigidbody(startEllipse, 1.f));
	startEllipse->addComponent( move(rigidbody) );

	// creating renderable axises
	auto yLineRender = make_shared< Line >(Point2d(0, windowSize.y - 110), RGB(34, 87, 21), yAxisEngine);
	_renderables.push_back( yLineRender );

	auto xLineRender = make_shared< Line >(Point2d(windowSize.x - 130, 0), RGB(19, 28, 89), xAxisEngine);
	_renderables.push_back(xLineRender);

	auto startRenderer = make_shared< Render::Ellipse >(Point2d(40, 40), RGB(160, 18, 21), startEllipse );
	_renderables.push_back(startRenderer);


	// helper struct for rendering grid properties 
	// creates line renderer and a text to display physical meter properties
	struct Notch
	{
		Notch(Simulation* simulation, WorldObjectPtr parent, Point2d lineLength, wstring text )
		{
			NotchObj = make_shared< WorldObject >();
			parent->addChild(NotchObj);

			LineRend = make_shared< Line >(lineLength, RGB(0, 0, 0), NotchObj);
			simulation->_renderables.push_back(LineRend);

			TextObj = make_shared< WorldObject >();
			NotchObj->addChild(TextObj);

			auto textRendr = make_shared< Text >(text.c_str(), RGB(0, 0, 0), TextObj);
			simulation->_renderables.push_back(textRendr);
		}

		WorldObjectPtr NotchObj;
		WorldObjectPtr TextObj;
		std::shared_ptr<Line> LineRend;
	};

	// y notches
	auto props = _world.getPhysicProperties();
	int yNumber = yLineRender->getOffset().y * props.MeterToPixel;
	for (auto i = 1; i < yNumber; i += 20)
	{
		auto notchLength = 20;
		wstring text = to_wstring(i) + L"m";
		auto notch = Notch( this, yAxisEngine, Point2d(notchLength, 0), text );

		notch.NotchObj->Position = Point2d(-notchLength / 2, props.MeterToPixel * i);
		notch.TextObj->Position = Point2d(-notchLength - 10, notch.LineRend->getHeight());
	}

	// x notches
	int xNumber = xLineRender->getOffset().x * props.MeterToPixel;
	for (auto i = 1; i < xNumber ; i += 15)
	{
		auto notchLength = 20;
		wstring text = to_wstring(i) + L"m";
		auto notch = Notch(this, xAxisEngine, Point2d(0, notchLength), text);

		notch.NotchObj->Position = Point2d(props.MeterToPixel * i, -notchLength / 2.f);
		notch.TextObj->Position = Point2d(-notchLength - 10, notch.LineRend->getHeight());
	}
	
}

void Simulation::run( HWND window, std::shared_ptr<bool> run )
{
	auto lastUpdate = chrono::system_clock::now();
	auto updateTime = 1 / 30.0; // 60 fps

	while ( *run.get() )
	{
		chrono::duration<double> delta = chrono::system_clock::now() - lastUpdate;
		if (delta.count() >= updateTime)
		{
			InvalidateRect(window, nullptr, true);
			lastUpdate = chrono::system_clock::now();
		}
		_world.getUpdater().update();
	}
}
std::vector<RenderablePtr> Simulation::getRender()
{
	return _renderables;
}

World& Simulation::getWorld()
{
	return _world;
}