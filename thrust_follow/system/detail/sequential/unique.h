#pragma  once
#include <thrust/detail/config.h>
#include <thrust/system/detail/sequential/execution_policy.h>
#include <thrust/iterator/iterator_traits.h>
#include <thrust/pair.h>

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
	typename BinaryPredicate>
	__host__ __device__
	//输入数据必须是业已有序的
	OutputIterator unique_copy(sequential::execution_policy<DerivedPolicy>&,
		InputIterator first,
		InputIterator last,
		OutputIterator output,
		BinaryPredicate binary_pred)
{
	typedef typename thrust::detail::iterator_traits<InputIterator>::value_type;

	if (first != last) {			//如果元素数目不为0，那么
		T prev = *first;			//将第一个值赋予prev
		for (++first; first != last; ++first) {	//++first
			T temp = *first;		//将此时的*first赋予temp
			if (!binary_pred(prev, temp)) {//如果prev和temp不相等，则
				*output = prev;				//此时此元素满足unique，将其值赋予output，并
				++output;					//将output迭代器加1，并	
				prev = temp;				//将temp的值赋予prev（之前），以便进行下一次迭代
			}
		}
		*output = prev;						//由于在循环中*output最终被赋值的是if之后的prev，不是经过prev=temp赋值之后的，所以最后还要赋值，并
		++output;							//将output加1
	}
	return output;
}

template<typename DerivedPolicy,
	typename ForwardIterator,
	typename BinaryPredicate>
	__host__ __device__
	ForwardIterator unique(sequential::execution_policy<DerivedPolicy>& exec,
		ForwardIterator first,
		ForwardIterator last,
		BinaryPredicate binary_pred)
{
	// sequential unique_copy permits in-situ operation
	return sequential::unique_copy(exec, first, last, first, binary_pred);
} // end unique()

} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust
