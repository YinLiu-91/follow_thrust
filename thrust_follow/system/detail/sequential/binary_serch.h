#pragma once
#include <thrust/advance.h>
#include <thrust/distance.h>
#include <thrust/iterator/iterator_traits.h>
#include <thrust/detail/function.h>

namespace thrust {

namespace system {

namespace detail {

namespace sequential {

__thrust_exec_check_disable__
template<typename DerivedPolicy,
	typename ForwardIterator,
	typename T,
	typename StrictWeakOrdering>
	__host__ __device__
	ForwardIterator lower_bound(sequential::execution_policy<DerivedPolicy>&, ForwardIterator first, ForwardIterator last, const T& val, StrictWeakOrdering comp)
{
	thrust::detail::wrapped_function<StrictWeakOrdering, bool> wrapper_comp(comp);
	typedef typename thrust::iterator_difference< ForwardIterator>::type difference_type;
	difference_type len = thrust::distance(first, last);
	while (len > 0)
	{
		//应该是比较*middle和val的值大小，这里如果val大于middle为真
 // 比如 len=11;  len>>1=5;  middle=5;   若元素值与下标相等
 // val=6; 则 此if为真，此时first=5，++first=6;len=11-5-1=5;
 // 此时half=2,middle=8, 判断为假，len=2;
 // half=1，判断为假，len=1；
 //half=0  判断为假，len=0，返回first


 // val=4,为类似的判断
		difference_type half = len >> 1;
		ForwardIterator middle = first;

		thrust::advance(middle, half);//加half

		if (wrapper_comp(*middle, val)
		{
			first = middle;
				++first;
				len = len - half - 1;

		}
		else
		{
			len = half;
		}

	}//while
	return first;
}


//未写
__thrust_exec_check_disable__
template<typename DerivedPolicy,
	typename ForwardIterator,
	typename T,
	typename StrictWeakOrdering>
	__host__ __device__
	ForwardIterator upper_bound(sequential::execution_policy<DerivedPolicy>&,
		ForwardIterator first,
		ForwardIterator last,
		const T& val,
		StrictWeakOrdering comp)
{
	// wrap comp
	thrust::detail::wrapped_function<
		StrictWeakOrdering,
		bool
	> wrapped_comp(comp);

	typedef typename thrust::iterator_difference<ForwardIterator>::type difference_type;

	difference_type len = thrust::distance(first, last);

	while (len > 0)
	{
		difference_type half = len >> 1;
		ForwardIterator middle = first;

		thrust::advance(middle, half);

		if (wrapped_comp(val, *middle))
		{
			len = half;
		}
		else
		{
			first = middle;
			++first;
			len = len - half - 1;
		}
	}

	return first;
}

__thrust_exec_check_disable__
template<typename DerivedPolicy,
	typename ForwardIterator,
	typename T,
	typename StrictWeakOrdering>
	__host__ __device__
	bool binary_search(sequential::execution_policy<DerivedPolicy>& exec,
		ForwardIterator first,
		ForwardIterator last,
		const T& val,
		StrictWeakOrdering comp)
{
	ForwardIterator iter = sequential::lower_bound(exec, first, last, val, comp);

	// wrap comp
	thrust::detail::wrapped_function<
		StrictWeakOrdering,
		bool
	> wrapped_comp(comp);

	//这里iter！=last说明命中目标，并且*iter的值不大于val的值
	return iter != last && !wrapped_comp(val, *iter);
}




} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust
