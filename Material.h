#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#pragma once

#include "Graphic.h"
#include <filesystem>
#include <vector>
#include "Vertex.h"
#include "Technique.h"

using namespace std;
using namespace experimental::filesystem;

struct aiMaterial;
struct aiMesh;

namespace Bind
{
	class VertexBuffer;
	class IndexBuffer;
}

class Material
{
public:
	Material(Graphic & gfx, aiMaterial & material, path & modelPath);
	VertexRela::VertexBuffer ExtractVertices(aiMesh & mesh);
	vector<unsigned short> ExtractIndices(aiMesh & mesh);
	shared_ptr<Bind::VertexBuffer> MakeVertexBindable(Graphic & gfx, aiMesh & mesh, float scale = 1.0f);
	shared_ptr<Bind::IndexBuffer> MakeIndexBindable(Graphic & gfx, aiMesh & mesh);
	vector<Technique> GetTechniques();
private:
	string MakeMaterialTag(aiMesh & mesh);
private:
	VertexRela::VertexLayout m_vtLayout;
	vector<Technique> m_techinques;
	string m_modelPath;
	string m_name;
};

#endif // !__MATERIAL_H__

