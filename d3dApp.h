//***************************************************************************************
// d3dApp.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Simple Direct3D demo application class.  
// Make sure you link: d3d11.lib d3dx11d.lib D3DCompiler.lib D3DX11EffectsD.lib 
//                     dxerr.lib dxgi.lib dxguid.lib.
// Link d3dx11.lib and D3DX11Effects.lib for release mode builds instead
//   of d3dx11d.lib and D3DX11EffectsD.lib.
//***************************************************************************************

#ifndef D3DAPP_H
#define D3DAPP_H

#pragma once

#include "windows.h"
#include "Window.h"
#include "GameTimer.h"
#include "Camera.h"
#include "ShadowRenderGraph.h"

class D3DApp
{
public:
	D3DApp();
	virtual ~D3DApp();
	
	HINSTANCE Inst()const;
	shared_ptr<Window> GetWindow()const;
	int Run();
	void CalculateFrameStats();
	void HandleInput();
	void DoFrame(float dt);
public:
	virtual bool InitWnd(HINSTANCE hInstance, int nWndWidth, int nWndHeight, string szWndCaption = "");
protected:
	GameTimer mTimer;
	shared_ptr<Window> mWindow;
	shared_ptr<Camera> mCamera;
	shared_ptr<Rgph::ShadowRenderGraph> mRenderGraph;
	DWORD mSave = false;
};
extern D3DApp* gd3dApp;
#endif // D3DAPP_H