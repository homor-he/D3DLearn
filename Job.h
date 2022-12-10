#ifndef __JOB_H__
#define __JOB_H__

#pragma once

class Step;
class Drawable;
class Graphic;

namespace Rgph
{
	class Job
	{
	public:
		Job(Step* pStep, Drawable * pDrawable);
		void Execute(Graphic & gfx);
	private:
		Drawable * m_pDrawable;
		Step * m_pStep;
	};
}

#endif // !__JOB_H__
