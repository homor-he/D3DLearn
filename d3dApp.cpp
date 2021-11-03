//***************************************************************************************
// d3dApp.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "d3dApp.h"
#include <WindowsX.h>
#include <sstream>

D3DApp * gd3dApp = NULL;

D3DApp::D3DApp()
{
	// Get a pointer to the application object so we can forward 
	// Windows messages to the object's window procedure through
	// the global window procedure.
	gd3dApp = this;
}

D3DApp::~D3DApp()
{

}

HINSTANCE D3DApp::Inst()const
{
	return mWindow->Inst();
}

Window * D3DApp::GetWindow() const
{
	return mWindow;
}

bool D3DApp::InitWnd(HINSTANCE hInstance, int nWndWidth, int nWndHeight, string szWndCaption)
{
	mWindow = new Window(hInstance, szWndCaption, nWndWidth, nWndHeight);
	if (mWindow != nullptr)
		mWindow->InitWindow();
	return true;
}

int D3DApp::Run()
{
	MSG msg = {0};

	while(msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}
		// Otherwise, do animation/game stuff.
		else
        {	
			mTimer.Tick();
			if( !mWindow->mAppPaused)
			{
				mWindow->CalculateFrameStats();
				Graphic* pGraphic = mWindow->GetGraphic();
				pGraphic->UpdateScene(mTimer.TotalTime());
				pGraphic->DrawScene();
			}
			else
			{
				Sleep(100);
			}
        }
    }

	return (int)msg.wParam;
}





