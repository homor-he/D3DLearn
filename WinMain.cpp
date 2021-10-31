
#include "d3dApp.h"

int _stdcall WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    D3DApp theApp(hInstance);

    theApp.Init();
    theApp.Run();
	return 0;
}