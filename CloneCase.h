#ifndef __CLONECASE_H__
#define __CLONECASE_H__

#pragma once

#include "Vertex.h"
#include "BindableCommon.h"
#include "Cuboid.h"
#include "Sphere.h"

namespace DrawTest
{
	//实现绘制2个以上3d物体及设置纹理和旋转
	class CloneCase
	{
	public:
		void Init(Graphic & gfx);
		void Draw(Graphic & gfx, float dt);
	public:
		vector<shared_ptr<Cuboid>> m_cubeList;
		vector<shared_ptr<Bind::TransformCbuf>> m_cubeTransformList;

		shared_ptr<Sphere> m_sphere;
		shared_ptr<Bind::TransformCbuf> m_sphereTransform;
	};
}


#endif // !__CLONECASE_H__
