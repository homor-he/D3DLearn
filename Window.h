#ifndef __WINDOW_H__
#define __WINDOW_H__

#pragma once

#include "CmnHead.h"
#include "Graphic.h"

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
	void CalculateFrameStats();

	virtual void OnResize();
	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y); 
	virtual void OnMouseMove(WPARAM btnState, int x, int y);
public:
	int		  mWndWidth;
	int		  mWndHeight;

	bool      mAppPaused;
	bool      mMinimized;
	bool      mMaximized;
	bool      mResizing;
private:
	HINSTANCE mInst;				//�ô�����Ĵ��ڹ������ڵ�ģ��ʵ���ľ��
	HWND      mWnd;
	string	  mWndCaption;

	Graphic*  mGraphic;
};


#endif // !__WINDOW_H__
