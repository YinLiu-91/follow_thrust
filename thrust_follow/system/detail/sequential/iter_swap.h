#pragma  once
#include <thrust/detail/config.h>
#include <thrust/system/detail/sequential/execution_policy.h>
#include <thrust/detail/raw_pointer_cast.h>
#include <thrust/detail/swap.h>

namespace thrust
{
namespace system
{
namespace detail
{
namespace sequential
{
template<typename DerivedPolicy, typename Pointer1, typename Pointer2>
__host__ __device__
void iter_swap(sequential::execution_policy<DerivedPolicy>&, Pointer1 a, Pointer2 b) {
	using thrust::swap;
	swap(*thrust::raw_pointer_cast(a), *thrust::raw_pointer_cast(b));
}
} // end sequential
} // end detail
} // end system
} // end thrust