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
	typename ForwardIterator,
	typename Predicate>
	__host__ __device__
	ForwardIterator remove_if(sequential::execution_policy<DerivedPolicy>&,
		ForwardIterator first,
		ForwardIterator last,
		Predicate pred)
{
	//wrap pred
	thrust::detail::wrapped_function<Predicate, bool>
		wrapped_pred(pred);

	//往前移动迭代器直到满足条件或者到末尾
	while (first != last && !wrapped_pred(*first))
		++first;//不满足条件或者未到末尾

	if (first == last)
		return first;//先将特殊情况做好

	//结果迭代器始终未first
	ForwardIterator result = first;

	++first;
	while (first != last) {				//如果迭代器未到终点
		if (!wrapped_pred(*first)) {	//如果当前迭代器的值满足条件
			*result = *first;			//则，当前迭代器的值副歌result
			++result;					//result指向下一个位置
		}
		++first;						//当前指针++
	}
	return result;

}

__thrust_exec_check_disable__
template<typename DerivedPolicy,
	typename ForwardIterator,
	typename InputIterator,
	typename Predicate>
	__host__ __device__
	ForwardIterator remove_if(sequential::execution_policy<DerivedPolicy>&,
		ForwardIterator first,
		ForwardIterator last,
		InputIterator stencil,
		Predicate pred)
{
	//wrap pred
	thrust::detail::wrapped_function<
		predicate, bool>wrapped_pred(pred);

	//advance iterators until wrapped_pre(*stencil)is true or we reach the end of input
	while (first != last && !wrapped_pred(*stencil)) {
		++first;
		++stencil;
	}
	if (first == last)return first;
	++first;
	++stencil;//
	while (first!=last)
	{
		if (wrapped_pred(*stencil)) {
			*result = *first;
			++result;
		}

		++first;
		++stencil;

	}
	return result;
}

//未写完




} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust

