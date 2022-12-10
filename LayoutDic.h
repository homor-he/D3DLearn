#ifndef __LAYOUTDIC_H__
#define __LAYOUTDIC_H__

#pragma once

#include "DynamicConstant.h"
#include <unordered_map>

namespace Dcb
{
	class LayoutDic
	{
	public:
		static CookedLayout Resolve(RawLayout && layout);
	private:
		static LayoutDic& Get();
		unordered_map<string, shared_ptr<LayoutElement>> m_map;
	};
}


#endif // !__LAYOUTDIC_H__

