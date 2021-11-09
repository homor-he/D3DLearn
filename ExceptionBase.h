#ifndef __EXCEPTIONBASE_H__
#define __EXCEPTIONBASE_H__

#pragma once

#include <exception>
#include <string>

using namespace std;

class ExceptionBase : public exception
{
public:
	ExceptionBase(int line, const char* file);
	const char* what() const override;
	virtual const char* GetType() const;
	int GetLine() const;
	const string& GetFile() const;
	string GetOriginString() const;
private:
	int line;
	string file;
protected:
	mutable string whatBuffer;
};

#endif // !__EXCEPTIONBASE_H__
