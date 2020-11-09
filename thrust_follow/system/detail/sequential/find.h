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
	typename Predicate>
	__host__ __device__
{
	InputIterator find_if(execution_policy< DerivedPolicy>&,
	InputIterator first,
	InputIterator last,
	Predicate pred)
{
	//wrap pred
	thrust::detail::wrapped_function<Predicate,bool>
		wrapped_pred(pred);

	while (first != last) {
		if (wrapped_pred(*first))return first;
		++first;
	}

	return first;//此时返回末尾的下一个位置

}

} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust
