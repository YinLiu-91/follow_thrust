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
		//Ӧ���ǱȽ�*middle��val��ֵ��С���������val����middleΪ��
 // ���� len=11;  len>>1=5;  middle=5;   ��Ԫ��ֵ���±����
 // val=6; �� ��ifΪ�棬��ʱfirst=5��++first=6;len=11-5-1=5;
 // ��ʱhalf=2,middle=8, �ж�Ϊ�٣�len=2;
 // half=1���ж�Ϊ�٣�len=1��
 //half=0  �ж�Ϊ�٣�len=0������first


 // val=4,Ϊ���Ƶ��ж�
		difference_type half = len >> 1;
		ForwardIterator middle = first;

		thrust::advance(middle, half);//��half

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


//δд
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

	//����iter��=last˵������Ŀ�꣬����*iter��ֵ������val��ֵ
	return iter != last && !wrapped_comp(val, *iter);
}




} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust
