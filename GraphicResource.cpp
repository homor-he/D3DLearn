#include "GraphicResource.h"

ID3D11DeviceContext * GraphicResource::GetContext(Graphic & gfx)
{
	return gfx.mDeviceContext.Get();
}

ID3D11Device * GraphicResource::GetDevice(Graphic & gfx)
{
	return gfx.mDevice.Get();
}

DxgiInfoManager & GraphicResource::GetInfoManager(Graphic & gfx)
{
	return gfx.infoManager;
}
