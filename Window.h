#ifndef __WINDOW_H__
#define __WINDOW_H__

#pragma once

#include "CmnHead.h"
#include "Graphic.h"
#include "Mouse.h"

class Window
{
public:
	Window(HINSTANCE hInstance, string szWndCaption, int nWidth,int nHight);
	virtual ~Window();

	bool	  InitWindow();
	HINSTANCE Inst()const;
	HWND      Wnd()const;
	float     AspectRatio()const;    //宽高比
	Graphic*  GetGraphic()const;
	void CalculateFrameStats();

	virtual void OnResize();
public:
	static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgReal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	int		  mWndWidth;
	int		  mWndHeight;

	bool      mAppPaused;
	bool      mMinimized;
	bool      mMaximized;
	bool      mResizing;
private:
	HINSTANCE mInst;				//该窗口类的窗口过程所在的模块实例的句柄
	HWND      mWnd;
	string	  mWndCaption;

	Graphic*  mGraphic;
	Mouse     mMouse;
};


#endif // !__WINDOW_H__
