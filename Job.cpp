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
		//�������vtbuffer��idbuffer��topology�󶨵�shader
		m_pDrawable->Bind(gfx);
		//step�������ɫ������ͼ���йز��ʵĴ���
		m_pStep->Bind(gfx);
		gfx.DrawIndex(m_pDrawable->GetIndexCount());
	}
}