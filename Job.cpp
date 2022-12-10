#include "Job.h"
#include "Drawable.h"
#include "Graphic.h"

namespace Rgph
{
	Job::Job(Step * pStep, Drawable * pDrawable):
		m_pDrawable(pDrawable), m_pStep(pStep)
	{
	}

	void Job::Execute(Graphic & gfx) 
	{
		//将物体的vtbuffer和idbuffer、topology绑定到shader
		m_pDrawable->Bind(gfx);
		//step里包含着色器、贴图等有关材质的处理
		m_pStep->Bind(gfx);
		gfx.DrawIndex(m_pDrawable->GetIndexCount());
	}
}