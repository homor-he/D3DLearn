#include "LayoutDic.h"

namespace Dcb
{
	CookedLayout LayoutDic::Resolve(RawLayout && layout)
	{
		string sig = layout.GetSignature();
		unordered_map<string, shared_ptr<LayoutElement>> & map = Get().m_map;
		auto itor = map.find(sig);
		if (itor != map.end())
		{
			layout.ClearRoot();
			return CookedLayout(itor->second);
		}
		//ÃÌº”layoutµΩmap÷–
		auto result = map.insert(make_pair<string, shared_ptr<LayoutElement>>(move(sig), layout.DeliverRoot()));
		return CookedLayout(result.first->second);
	}

	LayoutDic & LayoutDic::Get()
	{
		static LayoutDic layoutDic;
		return layoutDic;
	}
}