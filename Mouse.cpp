#include "Mouse.h"

Mouse::Mouse()
{
	leftIsPressed = false;
	rightIsPressed = false;
	x = 0;
	y = 0;
	totalWheelDelta = 0;
}

Mouse::~Mouse()
{
}

void Mouse::OnMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;
	eventBuffer.push(Event(Event::Type::Move, *this));
	RemoveBuffer();
}

void Mouse::OnMouseLeave()
{
	eventBuffer.push(Event(Event::Type::Leave, *this));
	RemoveBuffer();
}

void Mouse::OnMouseEnter()
{
	eventBuffer.push(Event(Event::Type::Enter, *this));
	RemoveBuffer();
}

void Mouse::OnLeftPressed(int x, int y)
{
	leftIsPressed = true;
	eventBuffer.push(Event(Event::Type::LPress, *this));
	RemoveBuffer();
}

void Mouse::OnLeftReleased(int x, int y)
{
	leftIsPressed = false;
	eventBuffer.push(Event(Event::Type::LRelease, *this));
	RemoveBuffer();
}

void Mouse::OnRightPressed(int x, int y)
{
	rightIsPressed = true;
	eventBuffer.push(Event(Event::Type::RPress, *this));
	RemoveBuffer();
}

void Mouse::OnRightReleased(int x, int y)
{
	rightIsPressed = false;
	eventBuffer.push(Event(Event::Type::RRelease, *this));
	RemoveBuffer();
}

void Mouse::OnWheelUp(int x, int y)
{
	eventBuffer.push(Event(Event::Type::WheelUp, *this));
	RemoveBuffer();
}

void Mouse::OnWheelDown(int x, int y)
{
	eventBuffer.push(Event(Event::Type::WheelDown, *this));
	RemoveBuffer();
}

void Mouse::OnWheelDelta(int x, int y, int delta)
{
	totalWheelDelta += delta;
	//if (totalWheelDelta >= WHEEL_DELTA)
	//{
	//	//OnWheelUp(x, y);
	//	totalWheelDelta -= WHEEL_DELTA;
	//}
	//else if (totalWheelDelta <= -WHEEL_DELTA)
	//{
	//	//OnWheelDown(x, y);
	//	totalWheelDelta += WHEEL_DELTA;
	//}
}

bool Mouse::ReadEventBuffer(Event & sEvent)
{
	if (eventBuffer.size() > 0)
	{
		sEvent = eventBuffer.front();
		//memcpy_s(&sEvent, sizeof(Event), &eventBuffer.front(), sizeof(Event));
		eventBuffer.pop();
		return true;
	}
	return false;
}

void Mouse::RemoveBuffer()
{
	while (eventBuffer.size() > bufferSize)
	{
		eventBuffer.pop();
	}
}
