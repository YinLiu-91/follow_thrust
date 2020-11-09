#pragma  once 
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

template<typename DerivedPolicy,typename Pointer1,typename Pointer2>
__host__ __device__
void assign_value(sequential::execution_policy<DerivedPolicy>&, Pointer1 dst, Pointer2 src)
{
	*thrust::raw_pointer_cast(dst) = *thrust::raw_pointer_cast(src);//一个强制转型的赋值
}



} // end sequential
} // end detail
} // end system
} // end thrust