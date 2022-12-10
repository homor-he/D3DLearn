#ifndef __SEVERALGEOMETRYCASE_H__
#define __SEVERALGEOMETRYCASE_H__

#pragma once

#include "Vertex.h"
#include "BindableCommon.h"
#include "Cuboid.h"
#include "Sphere.h"

namespace DrawTest
{
	//ʵ�ֻ���2��3d���岢���ò�ͬ����
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
