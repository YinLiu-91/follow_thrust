#pragma once
#pragma once

#include <thrust/detail/config.h>
#include <thrust/system/detail/sequential/execution_policy.h>
#include <thrust/iterator/iterator_traits.h>
#include <thrust/detail/type_traits.h>
#include <thrust/detail/type_traits/function_traits.h>
#include <thrust/detail/type_traits/iterator/is_output_iterator.h>
#include <thrust/detail/function.h>

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
	typename OutputIterator,
	typename BinaryFunction>
	__host__ __device__
	OutputIterator inclusive_scan(sequential::execution_policy<DerivedPolicy>&,
		InputIterator first,
		InputIterator last,
		OutputIterator result,
		BinaryFunction binary_op)
{
	using namespace thrust::detail;

	//
	using ValueType = typename thrust::iterator_value<InputIterator>::type;

	//wrap binary_op
	wrapped_function<BinaryFunction, ValueType>wrapped_binary_op(binary_op);

	if (first != last) {
		ValueType sum = *first;
		*result = *first;
		for (++first, ++result; first != last; ++first, ++result)
			*result = sum = wrapped_binary_op(sum, *first);//将二元结合值值赋予result迭代器和sum
	}
	return result;//返回迭代器
}

__thrust_exec_check_disable__
template<typename DerivedPolicy,
	typename InputIterator,
	typename OutputIterator,
	typename InitialValueType,
	typename BinaryFunction>
	__host__ __device__
	OutputIterator exclusive_scan(sequential::execution_policy<DerivedPolicy>&,
		InputIterator first,
		InputIterator last,
		OutputIterator result,
		InitialValueType init,//多了一个initial value
		BinaryFunction binary_op)
{
	using namespace thrust::detail;

	using ValeType = InitialValueType;

	if (first != last) {
		ValeType tmp = *first;		//有临时变量可以便于就地扫描
		ValeType sum = init;		//这里与上一个函数不同

		*result = sum;
		sum = binary_op(sum, tmp);
		for (++first, ++result; first != last; ++first, ++result) {
			tmp = *first;
			*result = sum;
			sum = binary_op(sum, tmp);
		}
	}
	return result;
}


} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust

