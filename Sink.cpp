#include "Sink.h"

namespace Rgph
{
	const string & Sink::GetRegisteredName()
	{
		return m_registeredName;
	}

	const string & Sink::GetPassName()
	{
		return m_passName;
	}

	const string & Sink::GetOutputName()
	{
		return m_outputName;
	}

	void Sink::SetTarget(string passName, string outputName)
	{
		{
			if (passName.empty())
			{
				OutPutError("%s:%d\nEmpty pass name", __FILE__, __LINE__);
				return;
			}

			bool nameCharsValid = true;
			for (char c : passName)
			{
				nameCharsValid &= (isalnum(c) || c == '_');
			}

			if (passName != "$" && (!nameCharsValid || isdigit(passName[0])))
			{
				OutPutError("%s:%d\nInvalid pass name", __FILE__, __LINE__);
				return;
			}
			m_passName = passName;
		}
	
		{
			if (outputName.empty())
			{
				OutPutError("%s:%d\nEmpty output name", __FILE__, __LINE__);
				return;
			}

			bool nameCharsValid = true;
			for (char c : outputName)
			{
				nameCharsValid &= (isalnum(c) || c == '_');
			}

			if (!nameCharsValid || isdigit(outputName[0]))
			{
				OutPutError("%s:%d\nInvalid pass name", __FILE__, __LINE__);
				return;
			}
			m_outputName = outputName;
		}
	}

	Sink::Sink(string registeredName):m_registeredName(registeredName)
	{
	}
}