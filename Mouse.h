#ifndef __MOUSE_H__
#define __MOUSE_H__

#pragma once

#include <queue>

#define WHEEL_DELTA 120

class Mouse
{
	struct WheelDelta
	{
		int x, y;
	};
	class Event
	{
	public:
		enum Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		int x;
		int y;
	public:
		Event(Type enType, const Mouse & parent)
		{
			type = enType;
			leftIsPressed = parent.leftIsPressed;
			rightIsPressed = parent.rightIsPressed;
			x = parent.x;
			y = parent.y;
		}
		Type GetType() const
		{
			return type;
		}
		int GetXPos() const
		{
			return x;
		}
		int GetYPos() const
		{
			return y;
		}
		std::pair<int, int> GetAxis() const
		{
			return{ x,y };
		}
		bool LeftIsPressed() const
		{
			return leftIsPressed;
		}
		bool RightIsPressed() const
		{
			return rightIsPressed;
		}
	};

public:
	Mouse();
	~Mouse();

	void OnMouseMove(int x, int y);
	void OnMouseLeave();
	void OnMouseEnter();
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnWheelDelta(int x, int y, int delta);
	bool ReadEventBuffer(Event & sEvent);
	void RemoveBuffer();
private:
	static const int bufferSize = 16;
	bool leftIsPressed;
	bool rightIsPressed;
	int x;
	int y;
	int totalWheelDelta;
	std::queue<Event> eventBuffer;
	std::queue<WheelDelta> wheelDeltaBuff;
};

#endif // !__MOUSE_H__
