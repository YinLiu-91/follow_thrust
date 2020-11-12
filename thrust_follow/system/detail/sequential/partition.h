#pragma once
#include <thrust/detail/config.h>
#include <thrust/pair.h>
#include <thrust/detail/temporary_array.h>
#include <thrust/detail/function.h>
#include <thrust/system/detail/sequential/execution_policy.h>

namespace thrust
{
namespace detail
{


// XXX WAR an unfortunate circular #inclusion problem
template<typename, typename> class temporary_array;//????干嘛使的？？


} // end detail



namespace system
{
namespace detail
{
namespace sequential
{
__thrust_exec_check_disable__
template<typename ForwardIterator1,
	typename ForwardIterator2>
	__host__ __device__
	void iter_swap(ForwardIterator1 iter1, ForwardIterator2 iter2) {

	using namespace thrust::detail;
	typedef typename thrust::iterator_value< ForwardIterator1>::type T;
	T temp = *iter1;
	*iter1 = *iter2;
	*iter2 = temp;
}//进行值的交换
__thrust_exec_check_disable__
template<typename DerivedPolicy,
	typename ForwardIterator,
	typename Predicate>
	__host__ __device__
	ForwardIterator partition(sequential::execution_policy< DerivedPolicy>&,
		ForwardIterator first,
		ForwardIterator last,
		Predicate pred) {
	if (first == last)return first;

	//wrap pred
	thrust::detail::wrapped_function<Predicate, bool>
		wrapped_pred(pred);
	while (wrapped_pred(*first)) {
		if (++first == last)
			return first;
	}//特殊情况
	ForwardIterator next = first;
	while (++next!=last)
	{
		if (wrapped_pred(*next)) {
			//
			::thrust::system::sequential::iter_swap(first, next);//交换其值
			++first;//啥意思？？？
		}

	}
	return first;

}//函数结束
__thrust_exec_check_disable__
template<typename DerivedPolicy,
	typename ForwardIterator,
	typename InputIterator,
	typename Predicate>
	__host__ __device__
	ForwardIterator partition(sequential::execution_policy<DerivedPolicy>&,
		ForwardIterator first,
		ForwardIterator last,
		InputIterator stencil_first,
		Predicate pred)
{
	if (first == last)return last;

	//wrapped pred
	thrust::detail::wrapped_function<Predicate, bool>
		wrapped_pred(pred);
	while (wrapped_pred(*stencil_first)) {
		++stencil_first;
		if (++first == last) {
			return first;
		}
	}//特例

	ForwardIterator next = first;

	//advance stencil to next element as well
	++stencil_first;
	while (++next != last) {
		if (wrapped_pred(*stencil_first)) {
			::thrust::system::detail::sequential::iter_swap(first, next);
			++first;
		}
		++stencil_first;
	}
	return first;

}//函数结束


__thrust_exec_check_disable__
template<typename DerivedPolicy,
	typename ForwardIterator,
	typename Predicate>
	__host__ __device__
	ForwardIterator stable_partition(sequential::execution_policy<DerivedPolicy>& exec,
		ForwardIterator first,
		ForwardIterator last,
		Predicate pred)
{
	//wrap pred
	thrust::detail::wrapped_function<Predicate, bool>wrapped_pred(pred);
	typedef typename thrust::iterator_value< ForwardIterator>::type T;
	typedef thrust::detail::temporary_array<T, DerivedPolicy>TempRange;
	typedef typename TempRange::iterator TempIterator;
	TempRange temp(exec, first, last);
	for (TempIterator iter=temp.begin();iter!=tem.end();++iter)
	{
		if (wrapped_pred(*iter)) {
			*first = *iter;
			++first;
		}
	}

	ForwardIterator middle = first;

	for (TempIterator iter = temp.begin(); iter != temp.end(); ++iter)
	{
		if (!wrapped_pred(*iter))
		{
			*first = *iter;
			++first;
		}
	}

	return middle;
}//原文中204行

} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust