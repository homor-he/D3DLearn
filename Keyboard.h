#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#pragma once

#include <queue>
#include <bitset>

using namespace std;

class Keyboard
{
public:
	class Event
	{
	public:
		enum class Type
		{
			Default,
			Press,
			Release,
		};
	private:
		Type m_type;
		unsigned char m_code;
	public:
		Event(Type type, unsigned char code) : m_type(type), m_code(code)
		{}

		bool IsPressed()
		{
			return m_type == Type::Press;
		}

		bool IsReleased()
		{
			return m_type == Type::Release;
		}

		unsigned char GetCode()
		{
			return m_code;
		}

		Type GetType()
		{
			return m_type;
		}
	};

public:
	void OnKeyPressed(unsigned char keyCode);
	void OnKeyReleased(unsigned char keyCode);
	bool ReadKey(Event & event);
	bool ReadCode(char & code);
	void OnChar(char keyCode);
	template<typename T>
	void RemoveBuffer(queue<T> & buff);
private:
	static const unsigned int m_nkey = 256u;
	static const int bufferSize = 16;
	bitset<m_nkey> m_keyStates;
	queue<Event> m_events;
	//详情见键盘与ascii码对照表 https://www.runoob.com/note/29592
	queue<char> m_codes;
};


#endif // !__KEYBOARD_H__

template<typename T>
inline void Keyboard::RemoveBuffer(queue<T>& buff)
{
	while (buff.size() > bufferSize)
	{
		buff.pop();
	}
}
