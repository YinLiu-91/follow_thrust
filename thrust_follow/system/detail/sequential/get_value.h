#pragma once

#include <thrust/detail/config.h>
#include <thrust/system/detail/sequential/execution_policy.h>
#include <thrust/detail/raw_pointer_cast.h>

namespace thrust
{
namespace system
{
namespace detail
{
namespace sequential
{

template<typename DerivedPolicy,typename Pointer>
__host__ __device__
typename thrust::iterator_value<Pointer>::typpe//·µ»ØÖµ
get_value(sequential::execution_policy< DerivedPolicy>&, Pointer ptr) {
	return *thrust::raw_pointer_cast(ptr);
}
} // end sequential
} // end detail
} // end system
} // end thrust

