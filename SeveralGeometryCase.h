#ifndef __SEVERALGEOMETRYCASE_H__
#define __SEVERALGEOMETRYCASE_H__

#pragma once

#include "Vertex.h"
#include "BindableCommon.h"
#include "Cuboid.h"
#include "Sphere.h"

namespace DrawTest
{
	//实现绘制2个3d物体并设置不同纹理
	class SeveralGeometryCase 
	{
	public:
		void Init(Graphic & gfx);
		void Draw(Graphic & gfx, float dt);
	private:
		shared_ptr<Cuboid> m_cube;
		shared_ptr<Bind::TransformCbuf> m_cubeTransform;

		shared_ptr<Sphere> m_sphere;
		shared_ptr<Bind::TransformCbuf> m_sphereTransform;
	};
}

#endif // !__SEVERALGEOMETRYCASE_H__
