#include "Window.h"
#include "d3dApp.h"
#include <WindowsX.h>
#include <sstream>

LRESULT CALLBACK
WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (gd3dApp == nullptr) 
		return 0;
	Window * gWindow = gd3dApp->GetWindow();
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			gWindow->mAppPaused = true;
			//mTimer.Stop();
		}
		else
		{
			gWindow->mAppPaused = false;
			//mTimer.Start();
		}
		return 0;

		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		gWindow->mWndWidth = LOWORD(lParam);
		gWindow->mWndHeight = HIWORD(lParam);
		if (gWindow->GetGraphic()->md3dDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				gWindow->mAppPaused = true;
				gWindow->mMinimized = true;
				gWindow->mMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				gWindow->mAppPaused = false;
				gWindow->mMinimized = false;
				gWindow->mMaximized = true;
				gWindow->OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (gWindow->mMinimized)
				{
					gWindow->mAppPaused = false;
					gWindow->mMinimized = false;
					gWindow->OnResize();
				}

				// Restoring from maximized state?
				else if (gWindow->mMaximized)
				{
					gWindow->mAppPaused = false;
					gWindow->mMaximized = false;
					gWindow->OnResize();
				}
				else if (gWindow->mResizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					gWindow->OnResize();
				}
			}
		}
		return 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		gWindow->mAppPaused = true;
		gWindow->mResizing = true;
		//mTimer.Stop();
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		gWindow->mAppPaused = false;
		gWindow->mResizing = false;
		//mTimer.Start();
		gWindow->OnResize();
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		gWindow->OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		gWindow->OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		gWindow->OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


Window::Window(HINSTANCE hInstance, string szWndCaption, int nWidth, int nHight)
{
	mInst = hInstance;
	mWnd = nullptr;
	mWndCaption = szWndCaption;
	mWndWidth = nWidth;
	mWndHeight = nHight;
	mAppPaused = false;
	mMinimized = false;
	mMaximized = false;
	mResizing = false;

	mGraphic = nullptr;
}

Window::~Window()
{
	if (mGraphic != nullptr)
	{
		delete mGraphic;
		mGraphic = nullptr;
	}
}

bool Window::InitWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "Window";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, "RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, mWndWidth, mWndHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	mWnd = CreateWindow(wc.lpszClassName, mWndCaption.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mInst, 0);
	if (!mWnd)
	{
		MessageBox(0, "CreateWindow Failed.", 0, 0);
		return false;
	}

	mGraphic = new Graphic(mWnd, mWndWidth, mWndHeight);
	mGraphic->Init();

	ShowWindow(mWnd, SW_SHOW);
	UpdateWindow(mWnd);

	return true;
}

HINSTANCE Window::Inst() const
{
	return mInst;
}

HWND Window::Wnd() const
{
	return mWnd;
}

float Window::AspectRatio() const
{
	return static_cast<float>(mWndWidth) / mWndHeight;
}

Graphic * Window::GetGraphic() const
{
	return mGraphic;
}

void Window::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	//if( (mTimer.TotalTime() - timeElapsed) >= 1.0f )
	//{
	//	float fps = (float)frameCnt; // fps = frameCnt / 1
	//	float mspf = 1000.0f / fps;

	//	std::wostringstream outs;   
	//	outs.precision(6);
	//	outs << mMainWndCaption << L"    "
	//		 << L"FPS: " << fps << L"    " 
	//		 << L"Frame Time: " << mspf << L" (ms)";
	//	SetWindowText(mhMainWnd, outs.str().c_str());
	//	
	//	// Reset for next average.
	//	frameCnt = 0;
	//	timeElapsed += 1.0f;
	//}
}

void Window::OnResize()
{
	mGraphic->OnResize();
}

void Window::OnMouseDown(WPARAM btnState, int x, int y)
{
}

void Window::OnMouseUp(WPARAM btnState, int x, int y)
{
}

void Window::OnMouseMove(WPARAM btnState, int x, int y)
{
	ostringstream outs;
	outs << "ÆÁÄ»×ø±ê (" << x << "," << y <<")";
	SetWindowText(mWnd, outs.str().c_str());
}

