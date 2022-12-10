#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#pragma once

#include "Graphic.h"
#include <DirectXMath.h>
#include <memory>
#include "Technique.h"

class Material;
struct aiMesh;

namespace Rgph
{
	class RenderGraph;
}

namespace Bind
{
	class VertexBuffer;
	class IndexBuffer;
	class Topology;
	class InputLayout;
}

enum RotaStat
{
	Revolution,  //��ת
	Rotation,	 //��ת
};

enum GeometryType
{
	Common,			//һ��������Ⱦ����
	Solid,			//������λ�ã��ɱ�ʾ���Դ������
};

class Drawable
{
public:
	Drawable() = default;
	Drawable(Graphic & gfx, Material & mat, aiMesh & mesh, float scale = 1.0f);
	virtual ~Drawable();
	virtual DirectX::XMMATRIX GetTransform() = 0;
	void AddTechnique(Technique tech);
	void Bind(Graphic & gfx);
	UINT GetIndexCount();
	void Submit(size_t channelFilter);
	void LinkTechnique(Rgph::RenderGraph &);
protected:
	std::shared_ptr<Bind::VertexBuffer> m_pVtBuffer;
	std::shared_ptr<Bind::IndexBuffer> m_pIdBuffer;
	std::shared_ptr<Bind::Topology> m_pTopology;
	std::vector<Technique> m_techniques;
};

#endif // !__DRAWABLE_H__
