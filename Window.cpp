#include "Window.h"
#include "d3dApp.h"
#include <WindowsX.h>
#include <sstream>
#include "CmnFunc.h"

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
	DestroyWindow(mWnd);

}

bool Window::InitWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = HandleMsgSetup;
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
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mInst, this);
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

LRESULT CALLBACK
Window::HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		//获取window指针
		CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		//用pwnd替换hwnd的窗口相关用户数据
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		//用HandleMsgReal替换hwnd原来的回调
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&HandleMsgReal));
		return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsgReal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			mAppPaused = true;
			//mTimer.Stop();
		}
		else
		{
			mAppPaused = false;
			//mTimer.Start();
		}
		return 0;

		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		mWndWidth = LOWORD(lParam);
		mWndHeight = HIWORD(lParam);
		if (mGraphic->mDevice.Get())
		{
			if (wParam == SIZE_MINIMIZED)
			{
				mAppPaused = true;
				mMinimized = true;
				mMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				mAppPaused = false;
				mMinimized = false;
				mMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (mMinimized)
				{
					mAppPaused = false;
					mMinimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if (mMaximized)
				{
					mAppPaused = false;
					mMaximized = false;
					OnResize();
				}
				else if (mResizing)
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
					OnResize();
				}
			}
		}
		return 0;

		/************* BAR MESSAGES ****************/
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		mAppPaused = true;
		mResizing = true;
		//mTimer.Stop();
		return 0;
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		mAppPaused = false;
		mResizing = false;
		//mTimer.Start();
		OnResize();
		return 0;
		/************* BAR MESSAGES ****************/

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

		/************* MOUSE MESSAGES ****************/
	case WM_LBUTTONDOWN:
		mMouse.OnLeftPressed(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		mMouse.OnRightPressed(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
		mMouse.OnLeftReleased(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		mMouse.OnRightReleased(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
	{
		mMouse.OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		//WriteLog("%s-%d, 屏幕坐标(%d,%d)",__FILE__,__LINE__, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	}
	case WM_MOUSEHWHEEL:
		mMouse.OnWheelDelta(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), GET_WHEEL_DELTA_WPARAM(wParam));
		return 0;
		/************* MOUSE MESSAGES ****************/
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

