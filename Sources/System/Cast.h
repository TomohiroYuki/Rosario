#pragma once
#include "CoreMinimal.h"
template <typename From,typename To>
struct CastImpl
{
	F_INLINE static To*  Cast(From* instance)
	{
		return (To*)instance;
	}
};

template <typename From, typename To>
F_INLINE To* Cast(From* instance)
{
	return CastImpl<From, To>::Cast(instance);
}