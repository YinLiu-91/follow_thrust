#pragma once
#include <thrust/detail/config.h>
#include <thrust/detail/function.h>
#include <thrust/system/detail/sequential/execution_policy.h>

namespace thrust
{
namespace system
{
namespace detail
{
namespace sequential
{
__thrust_exec_check_disable__
template<typename DerivedPolicy,
	typename InputIterator,
	typename OutputType,
	typename BinaryFunction>
	__host__ __device__
	OutputType reduce(sequential::execution_policy<DerivedPolicy>&,
		InputIterator begin,
		InputIterator end,
		OutputType init,
		BinaryFunction binary_op)
{
	//wrap binary_op
	thrust::detail::wrapped_function<BinaryFunction,
		OutputType>wrapped_binary_op(binary_op);

	//initialize the result
	OutputType result = init;
	while (begin != end) {//如果begin!=end
		result = wrapped_binary_op(result, *begin);//
		++begin;//这里是让begin一直++
	}
	return result;
}

} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust