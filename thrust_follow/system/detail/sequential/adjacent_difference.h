#pragma once
#include <thrust/detail/config.h>
#include <thrust/iterator/iterator_traits.h>
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
	typename OutputIterator,
	typename BinaryFunction>
__host__ __device__ 
OutputIterator adjacent_difference(sequential::execution_policy<DerivedPolicy>&,
	InputIterator first,
	InputIterator last,
	OutputIterator result,
	BinaryFunction binary_op)
{
	//�õ���������ָ������
	typedef typename thrust::iterator_traits< InputIterator>::value_type InputType;
	if (first == last)return result;
	InputType curr = *first;
	*result = curr;
	while (++first!=last)
	{
		InputType next = *first;//��ȡ��ֵ
		*(++result) = binary_op(next, curr);//���ж�Ԫ�����������������++result
		curr = next;

	}
	return ++result;
}

} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust