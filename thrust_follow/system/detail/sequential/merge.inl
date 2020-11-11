#include <thrust/detail/config.h>
#include <thrust/system/detail/sequential/merge.h>
#include <thrust/detail/copy.h>
#include <thrust/iterator/iterator_traits.h>
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
	OutputIterator merge(sequential::execution_policy<DerivedPolicy>& exec,
		InputIterator1 first1,
		InputIterator1 last1,
		InputIterator2 first2,
		InputIterator2 last2,
		OutputIterator result,
		StrictWeakOrdering comp)
{
	//wrap comp
	thrust::detail::wrapped_function<StrictWeakOrdering, bool>
		wrapped_comp(comp);
	while (first1 != last1 && first2 != last2) {
		if (wrapped_comp(*first2, *first1)) {
			//若*first2严格小于*first1，则
			*result = *first2;//将first2的值赋予result迭代器
			++first2;//并将first2所指位置加1
		}
		else {
			*result = *first1;
			++first1;
		}
		++result;//resutl相当于输出序列，其结果是两个序列的归并
	}//end while
	return thrust::copy(exec, first1, last2, thrust::copy(exec, first1, last1, result));///返回的是什么意思？？？
}

__thrust_exec_check_disable__
template<typename DerivedPolicy,
	typename InputIterator1,
	typename InputIterator2,
	typename InputIterator3,
	typename InputIterator4,
	typename OutputIterator1,
	typename OutputIterator2,
	typename StrictWeakOrdering>
	__host__ __device__
	thrust::pair<OutputIterator1,OutputIterator2>
	merge_by_key(sequential::execution_policy<DerivedPolicy>& exec,
		InputIterator1 keys_first1,
		InputIterator1 keys_last1,
		InputIterator2 keys_first2,
		InputIterator2 keys_last2,
		InputIterator3 values_first1,
		InputIterator4 values_first2,
		OutputIterator1 keys_result,
		OutputIterator2 values_result,
		StrictWeakOrdering comp)
{
	//wrap comp
	thrust::detail::wrapped_function<
		StrictWeakOrdering, bool>
		wrapped_comp(comp);
	while (keys_first1 != keys_last1 && keys_first2 != keys_last2) {
		if (!wrapped_comp(*keys_last2, *keys_first1)) {
			//*keys_first1<=*keys_first2
			*keys_result = *keys_first1;
			*values_result = *values_first1;
			++keys_first1;
			++values_first1;
		}
		else
		{
			//*keyw_first1>*keys_first2
			*keys_result = *keys_first2;
			*values_result = *values_first2;
			++keys_first2;
			++values_first2;
		}
		++keys_result;
		++values_result;
	}
	while (keys_first1!=keys_last1)//此时归并的另一路是相等的迭代器，即无内容,
	{
		*keys_result = *keys_first1;
		*values_result = *values_first1;
		++keys_first1;
		++values_first1;
		++keys_result;
		++values_result;

	}

	while (keys_first2 != keys_last2) {//此时归并的另一路是相等的迭代器，即无内容
		*keys_result = *keys_first2;
		*values_result = *values_first2;
		++keys_first2;
		++values_first2;
		++keys_result;
		++values_result;
	}
	return thrust::make_pair(keys_result, values_result);
}




} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust