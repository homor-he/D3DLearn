#ifndef __NODE_H__
#define __NODE_H__

#pragma once

#include "Graphic.h"

class Mesh;

class Node
{
	friend class Model;
public:
	Node(int id, const string & name, vector<Mesh*> meshPtrs, const DirectX::XMMATRIX & transform);
	void SetAppliedTransform(DirectX::FXMMATRIX transform);
	void Submit(size_t channels, DirectX::FXMMATRIX transform);
private:
	void AddChild(unique_ptr<Node> pChild);
private:
	string m_name;
	int m_id;
	vector<unique_ptr<Node>> m_childPtrs;
	vector<Mesh*> m_meshPtrs;
	DirectX::XMFLOAT4X4 m_transform;
	DirectX::XMFLOAT4X4 m_appliedTransform;
};


#endif // !__NODE_H__
