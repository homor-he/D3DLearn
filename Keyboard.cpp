#include "Keyboard.h"

void Keyboard::OnKeyPressed(unsigned char keyCode)
{
	m_keyStates[keyCode] = true;
	m_events.push(Event(Event::Type::Press, keyCode));
	RemoveBuffer(m_events);
}

void Keyboard::OnKeyReleased(unsigned char keyCode)
{
	m_keyStates[keyCode] = false;
	m_events.push(Event(Event::Type::Release, keyCode));
	RemoveBuffer(m_events);
}

bool Keyboard::ReadKey(Event & event)
{
	if (m_events.size() > 0u)
	{
		event =  m_events.front();
		m_events.pop();
		return true;
	}
	return false;
}

bool Keyboard::ReadCode(char & code)
{
	if (m_codes.size() > 0u)
	{
		code = m_codes.front();
		m_codes.pop();
		return true;
	}
	return false;
}

void Keyboard::OnChar(char keyCode)
{
	m_codes.push(keyCode);
	RemoveBuffer(m_codes);
}

