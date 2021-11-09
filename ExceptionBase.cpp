#include "ExceptionBase.h"
#include <sstream>

ExceptionBase::ExceptionBase(int line, const char * file) 
	:line(line),file(file)
{
}

const char * ExceptionBase::what() const
{
	ostringstream sOut;
	sOut << GetType() << endl << GetOriginString();
	whatBuffer = sOut.str();
	return whatBuffer.c_str();
}

const char * ExceptionBase::GetType() const
{
	return "ExceptionBase";
}

int ExceptionBase::GetLine() const
{
	return line;
}

const string & ExceptionBase::GetFile() const
{
	return file;
}

string ExceptionBase::GetOriginString() const
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}
