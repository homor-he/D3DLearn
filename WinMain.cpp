
#include "d3dApp.h"

int _stdcall WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    D3DApp theApp;
	theApp.InitWnd(hInstance, 625, 500, "myWindow");
	theApp.Run();
	return 0;
}