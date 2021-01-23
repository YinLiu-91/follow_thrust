#pragma once

#include <thrust/iterator/iterator_traits.h>
#include <thrust/iterator/detail/minimum_system.h>
#include <thrust/detail/type_traits.h>
#include <thrust/detail/type_traits/iterator/is_output_iterator.h>
#include <thrust/detail/type_traits/function_traits.h>
#include <thrust/transform.h>
#include <thrust/scatter.h>
#include <thrust/iterator/zip_iterator.h>
#include <limits>

#include <thrust/detail/internal_functional.h>
#include <thrust/scan.h>
#include <thrust/detail/temporary_array.h>

namespace thrust
{
namespace system
{
namespace detail
{
namespace generic
{
namespace detail
{
template<typename ValueType,typename TailFlagType, typename AssociativeOperator>
struct reduce_by_key_functor {
	AssociativeOperator binary_op;

	typedef typename thrust::tuple<ValueType, TailFlagType> result_type;

	__host__ __device__
		reduce_by_key_functor(AssociativeOperator _binary_op) :binary_op(_binary_op) {}
	__host__ __device__
		result_type operator()(result_type a, result_type b) {
		return result_type(thrust::get<1>(b) ? thrust::get<0>(b) : binary_op(thrust::get<0>(a), thrust::get<0>(b)),
			thrust::get<1>(a) | thrust::get<1>(b));
	}
};
} // end namespace detail

template<typename ExecutionPolicy,
	typename InputIterator1,
	typename InputIterator2,
	typename OutputIterator1,
	typename OutputIterator2,
	typename BinaryPredicate,
	typename BinaryFunction>
	__host__ __device__
	thrust::pair<OutputIterator1, OutputIterator2>
	reduce_by_key(thrust::execution_policy<ExecutionPolicy>& exec,
		InputIterator1 keys_first,
		InputIterator1 keys_last,
		InputIterator2 values_first,
		OutputIterator1 keys_output,
		OutputIterator2 values_output,
		BinaryPredicate binary_pred,
		BinaryFunction binary_op)
{
	typedef typename thrust::iterator_traits<InputIterator1>::difference_type difference_type;

	typedef unsigned int FlagType;

	typedef typename thrust::detail::eval_if<
		thrust::detail::has_result_type<BinaryFunction>::value,
		thrust::detail::result_type<BinaryFunction>,
		thrust::detail::eval_if<
		thrust::detail::is_output_iterator<OutputIterator2>::value,
		thrust::iterator_value<InputIterator2>,
		thrust::iterator_value<OutputIterator2>
		>
	>::type ValueType;


	if (keys_first == keys_last)
		return thrust::make_pair(keys_output, values_output);

	//input size
	difference_type n = keys_last - keys_first;

	InputIterator2 values_last = values_first + n;//首先计算出需要用到的第二个序列的长度

	//计算头部标志
	thrust::detail::temporary_array<FlagType, ExecutionPolicy> head_flags(exec, n);//什么事head_flags???
	thrust::transform(exec, keys_first, keys_last - 1, keys_first + 1, head_flags.begin() + 1, thrust::detail::not2(binary_pred));
	tail_flags[0] = 1;

	// 计算尾部flags
	thrust::detail::temporary_array<FlagType, ExecutionPolicy> tail_flags(exec, n); //COPY INSTEAD OF TRANSFORM
	thrust::transform(exec, keys_first, keys_last - 1, keys_first + 1, tail_flags.begin(), thrust::detail::not2(binary_pred));
	tail_flags[n - 1] = 1;

	//通过flag臊面values
	thrust::detail::temporary_array<ValueType, ExecutionPolicy>scanned_values(exec, n);
	thrust::detail::temporary_array<FlagType, ExecutionPolicy> scanned_tail_flags(exec, n);

	thrust::inclusive_scan
		(exec,)



}



} // end namespace generic
} // end namespace detail
} // end namespace system
} // end namespace thrust

