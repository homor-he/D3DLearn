#ifndef __WINDOW_H__
#define __WINDOW_H__

#pragma once

#include "CmnHead.h"
#include "Graphic.h"
#include "Mouse.h"
#include "Keyboard.h"

class Window
{
	
public:
	Window(HINSTANCE hInstance, string szWndCaption, int nWidth,int nHight);
	virtual ~Window();

	bool	  InitWindow();
	HINSTANCE Inst()const;
	HWND      Wnd()const;
	float     AspectRatio()const;    //��߱�
	Graphic*  GetGraphic()const;

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

	Mouse     mMouse;
	Keyboard  mKeyboard;
private:
	HINSTANCE mInst;				//�ô�����Ĵ��ڹ������ڵ�ģ��ʵ���ľ��
	HWND      mWnd;
	string	  mWndCaption;

	Graphic*  mGraphic;
	
};


#endif // !__WINDOW_H__
