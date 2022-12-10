#ifndef __MESH_H__
#define __MESH_H__

#pragma once

#include "Drawable.h"

//class Material;

class Mesh : public Drawable
{
public:
	Mesh(Graphic & gfx, Material & mat, aiMesh & mesh, float scale = 1.0f);
	DirectX::XMMATRIX GetTransform() override;
	void Submit(size_t channel, DirectX::FXMMATRIX transform);
private:
	DirectX::XMFLOAT4X4 m_transform;
};

#endif // !__MESH_H__
