#pragma  once
#include <thrust/detail/config.h>
#include <thrust/system/detail/sequential/execution_policy.h>
#include <thrust/detail/copy.h>
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
	typename InputIterator1,
	typename InputIterator2,
	typename OutputIterator,
	typename StrictWeakOrdering>
	__host__ __device__
	OutputIterator set_difference(sequential::execution_policy<DerivedPolicy>& exec,
		InputIterator1 first1,
		InputIterator1 last1,
		InputIterator2 first2,
		InputIterator2 last2,
		OutputIterator result,
		StrictWeakOrdering comp)
{
	//wrap comp
	thrust::detail::wrapped_function< StrictWeakOrdering, bool>wrapped_comp(comp);
	while (first1 != last1 && first2 != last2) {
		if (wrapped_comp(*first1, *first2))//如果first1严格小于first2的对象
		{
			*result = *first1;	//那么将*first1的值result
			++first1;			//并++first1和
			++result;			//++result
		}//end if
		else if (wrapped_comp(*first2, *first1))//否则，first2严格小于first1的对象
		{
			++first2;							//++first2
		}
		else//或者两者相等
		{
			++first1;//两者都前进
			++first2;
		}
	}
	return thrust::copy(exec, first1, last1, result);
}

__thrust_exec_check_disable__
template<typename DerivedPolicy,
	typename InputIterator1,
	typename InputIterator2,
	typename OutputIterator,
	typename StrictWeakOrdering>
	__host__ __device__
	OutputIterator set_intersection(sequential::execution_policy<DerivedPolicy>&,
		InputIterator1 first1,
		InputIterator1 last1,
		InputIterator2 first2,
		InputIterator2 last2,
		OutputIterator result,
		StrictWeakOrdering comp)
{
	//wrap comp
	thrust::detail::wrapped_function< StrictWeakOrdering, bool> wrapped_comp(comp);
	while (first1 != last1 && first2 != last2) {
		if (wrapped_comp(*first1, *first2))//*first1<*first2,亦或
		{
			++first1;
		}
		else if (wrapped_comp(*first2, *first1))//*first1,>*first2
		{
			++first2;
		}
		else//否则
		{
			*result = *first1;
			++first1;
			++first2;
			++result;
		}

	}

	return result;

}
__thrust_exec_check_disable__
template<typename DerivedPolicy,
	typename InputIterator1,
	typename InputIterator2,
	typename OutputIterator,
	typename StrictWeakOrdering>
	__host__ __device__
	OutputIterator set_symmetric_difference(sequential::execution_policy<DerivedPolicy>& exec,
		InputIterator1 first1,
		InputIterator1 last1,
		InputIterator2 first2,
		InputIterator2 last2,
		OutputIterator result,
		StrictWeakOrdering comp)
{
	//wrap comp
	thrust::detail::wrapped_function< StrictWeakOrdering, bool>
		wrapped_comp(comp);
	while (first1 != last1 && first2 != last2) {
		if (wrapped_comp(*first1, *first2))
		{
			*result = *first1;
			++first1;
			++result;
		}
		else if (*first2, *first1) {
			*result = *first2;
			++first2;
			++first1;
		}
		else {
			++first1;
			++first2;
		}
	}
	return thrust::copy(exec, first2, last2, thrust::copy(exec, first1, last1, result));
}

//目的：将set联合在一起
__thrust_exec_check_disable__
template<typename DerivedPolicy,
	typename InputIterator1,
	typename InputIterator2,
	typename OutputIterator,
	typename StrictWeakOrdering>
	__host__ __device__
	
	OutputIterator set_union(sequential::execution_policy<DerivedPolicy>& exec,
		InputIterator1 first1,
		InputIterator1 last1,
		InputIterator2 first2,
		InputIterator2 last2,
		OutputIterator result,
		StrictWeakOrdering comp)
{
	// wrap comp
	thrust::detail::wrapped_function<
		StrictWeakOrdering,
		bool
	> wrapped_comp(comp);

	while (first1 != last1 && first2 != last2)
	{
		if (wrapped_comp(*first1, *first2))//若*first1<*first2
		{
			*result = *first1;
			++first1;
		} // end if
		else if (wrapped_comp(*first2, *first1))//若
		{
			*result = *first2;
			++first2;
		} // end else if
		else//否则，相等，则只需要取一个值即可（因为是set）
		{
			*result = *first1;
			++first1;
			++first2;
		} // end else

		++result;
	} // end while

	return thrust::copy(exec, first2, last2, thrust::copy(exec, first1, last1, result));
} // end set_union()


} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust
