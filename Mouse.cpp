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
	eventBuffer.push(Event(Event::Move, *this));
	RemoveBuffer();
}

void Mouse::OnMouseLeave()
{
	eventBuffer.push(Event(Event::Leave, *this));
	RemoveBuffer();
}

void Mouse::OnMouseEnter()
{
	eventBuffer.push(Event(Event::Enter, *this));
	RemoveBuffer();
}

void Mouse::OnLeftPressed(int x, int y)
{
	leftIsPressed = true;
	eventBuffer.push(Event(Event::LPress, *this));
	RemoveBuffer();
}

void Mouse::OnLeftReleased(int x, int y)
{
	leftIsPressed = false;
	eventBuffer.push(Event(Event::LRelease, *this));
	RemoveBuffer();
}

void Mouse::OnRightPressed(int x, int y)
{
	rightIsPressed = true;
	eventBuffer.push(Event(Event::RPress, *this));
	RemoveBuffer();
}

void Mouse::OnRightReleased(int x, int y)
{
	rightIsPressed = false;
	eventBuffer.push(Event(Event::RRelease, *this));
	RemoveBuffer();
}

void Mouse::OnWheelUp(int x, int y)
{
	eventBuffer.push(Event(Event::WheelUp, *this));
	RemoveBuffer();
}

void Mouse::OnWheelDown(int x, int y)
{
	eventBuffer.push(Event(Event::WheelDown, *this));
	RemoveBuffer();
}

void Mouse::OnWheelDelta(int x, int y, int delta)
{
	totalWheelDelta += delta;
	if (totalWheelDelta >= WHEEL_DELTA)
	{
		//OnWheelUp(x, y);
		totalWheelDelta -= WHEEL_DELTA;
	}
	else if (totalWheelDelta <= -WHEEL_DELTA)
	{
		//OnWheelDown(x, y);
		totalWheelDelta += WHEEL_DELTA;
	}
}

bool Mouse::ReadEventBuffer(Event & sEvent)
{
	if (eventBuffer.size() > 0)
	{
		memcpy_s(&sEvent, sizeof(Event), &eventBuffer.front(), sizeof(Event));
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
