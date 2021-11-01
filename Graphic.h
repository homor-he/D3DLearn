#ifndef __GRAPHIC_H__
#define __GRAPHIC_H__

#pragma once

#include "CmnHead.h"
#include <d3d11.h>

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

class Graphic
{
public:
	Graphic(HWND hWnd, int nWndWidth, int nWndHeight);
	virtual ~Graphic();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();
public:
	HWND      mWnd;
	int		  mWndWidth;
	int		  mWndHeight;

	ID3D11Device* md3dDevice;
	ID3D11DeviceContext* md3dImmediateContext;
	IDXGISwapChain* mSwapChain;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mScreenViewport;

	bool	  mEnable4xMsaa;		//ÊÇ·ñ¿ªÆô¿¹¾â³Ý
	UINT	  m4xMsaaQuality;
};


#endif // !__GRAPHIC_H__
