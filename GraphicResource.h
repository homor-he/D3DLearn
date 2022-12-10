#ifndef __GRAPHICRESOURCE_H__
#define __GRAPHICRESOURCE_H__

#pragma once

#include "Graphic.h"

class GraphicResource
{
	friend class Window;
public:
	static ID3D11DeviceContext* GetContext(Graphic& gfx);
	static ID3D11Device* GetDevice(Graphic& gfx);
	static DxgiInfoManager& GetInfoManager(Graphic& gfx);
};

#endif // !__GRAPHICRESOURCE_H__
