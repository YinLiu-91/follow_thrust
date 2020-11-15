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
			*result = sum = wrapped_binary_op(sum, *first);//����Ԫ���ֵֵ����result��������sum
	}
	return result;//���ص�����
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
		InitialValueType init,//����һ��initial value
		BinaryFunction binary_op)
{
	using namespace thrust::detail;

	using ValeType = InitialValueType;

	if (first != last) {
		ValeType tmp = *first;		//����ʱ�������Ա��ھ͵�ɨ��
		ValeType sum = init;		//��������һ��������ͬ

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

