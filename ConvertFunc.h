#ifndef __CONVERTFUNC_H__
#define __CONVERTFUNC_H__

#pragma once

#include <DirectXMath.h>

DirectX::XMMATRIX ScaleTranslation(DirectX::XMMATRIX matrix, float scale);

DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M);
#endif // !__CONVERTFUNC_H__
