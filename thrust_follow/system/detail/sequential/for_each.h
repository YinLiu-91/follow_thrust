#pragma  once

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
	typename UnaryFunction>//UnaryFunctionӦ����һ����������
	__host__ __device__
	InputIterator for_each(sequential::execution_policy<DerivedPolicy>&,
		InputIterator first,
		InputIterator last,
		UnaryFunction f)//
{
	//wrap f
	thrust::detail::wrapped_function<UnaryFunction, void>wrapped_f(f);
	for (; first != last; ++first)//�����õ���for��������while
	{
		wrapped_f(*first);
	}
	return first;


}

template<typename DerivedPolicy,
	typename InputIterator,
	typename Size,
	typename UnaryFunction>//UnaryFunctionӦ����һ����������
	__host__ __device__
	InputIterator for_each_n(sequential::execution_policy<DerivedPolicy>&,
		InputIterator first,
		Size n,
		UnaryFunction f)
	__host__ __device__
	InputIterator for_each_n(sequential::execution_policy<DerivedPolicy>&, InputIterator first, Size n, UnaryFunction f)

{
	//wrap f
	thrust::detail::wrapped_function<UnaryFunction, void>
		wrpped_f(f);
	for (Size i = 0; i != n; i++) {//n������for_each
		wrapped_f(*first);
		++first;
	}
	return first;

}






} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust
