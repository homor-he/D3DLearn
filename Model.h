#ifndef __MODEL_H__
#define __MODEL_H__

#pragma once

#include "Graphic.h"
#include <memory>
#include "Node.h"
#include "Mesh.h"

using namespace std;

//class Node;
//class Mesh;
struct aiNode;

class Model
{
public:
	Model(Graphic & gfx, const string & pathString, float scale = 1.0f);
	void SetRootTransform(DirectX::FXMMATRIX tf);
	void Submit(size_t channelFilter);
	void LinkTechniques(Rgph::RenderGraph & renderGraph);
private:
	unique_ptr<Node> ParseNode(int & nextId, const aiNode & node, float scale);
private:
	unique_ptr<Node> m_pRoot;
	vector<unique_ptr<Mesh>> m_drawableList;
};


#endif // !__MODEL_H__
