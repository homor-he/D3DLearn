#ifndef __BINDSDIC_H__
#define __BINDSDIC_H__

#pragma once

#include "Bindable.h"
//#include <memory>
#include <unordered_map>

namespace Bind
{
	class BindsDic
	{
	public:
		template<class T, typename ... Params>
		static shared_ptr<T> Resolve(Graphic & gfx, Params && ...p)
		{
			static_assert(is_base_of<Bindable, T>::value, "Can only resolve classes derived from Bindable");
			return Get().Resolve_<T>(gfx, forward<Params>(p)...);
		}
	private:
		static BindsDic & Get()
		{
			static BindsDic bindsDic;
			return bindsDic;
		}
		template<class T, typename ... Params>
		shared_ptr<T> Resolve_(Graphic & gfx, Params && ...p)
		{
			const auto key = T::GenerateUID(forward<Params>(p)...);
			auto itor = binds.find(key);
			if (itor == binds.end())
			{
				auto bind = make_shared<T>(gfx, forward<Params>(p)...);
				binds[key] = bind;
				return bind;
			}
			else
			{
				return static_pointer_cast<T>(itor->second);
			}
		}
	private:
		unordered_map<string, shared_ptr<Bindable>> binds;
	};
}

#endif // !__BINDSDIC_H__
