#progma once
#include <thrust/detail/config.h>

#include <thrust/iterator/iterator_traits.h>
#include <thrust/detail/function.h>
#include <thrust/system/detail/sequential/copy_backward.h>

namespace thrust
{
namespace system
{
namespace detail
{
namespace sequential
{

__thrust_exec_check_disable__
template<typename RandomAccessIterator,
	typename StrictWeakOrdering>
	__host__ __device__
	void insertion_sort(RandomAccessIterator first,
		RandomAccessIterator last,
		StrictWeakOrdering comp)
{

	typedef typename thrust::iterator_value< RandomAccessIterator>::type value_type;//得到类型并typedef
	if (first == last)return;
	//warp comp
	thrust::detail::wrapped_function< StrictWeakOrdering, bool>wrapped_comp(comp);
	for (RandomAccessIterator i = first + 1; i != last; ++i) {
		value_type tmp = *i;//解引用并赋值给tmp
		//wrapped_comp比较第一个数是否小于第二个，小于为真
		if (wrapped_comp(tmp, *first)) {//first始终是当前扫描到的序列中最小的值
			//tmp 是目前为止最小的值
			sequential::copy_backward(first, i, i + 1);//这里是将：--i;--(i+1);*(i+1)=*i;是将first的值赋予第二个元素（详见copy_backward)

			*first = tmp;//将最小值给first
		}
		else {
			//tmp不是当前扫描序列中最小的，可以避免检查是否等于第一个
			//前面业已有序为什么还亦步亦趋的查找？？这样复杂度就是O(N^2)???实际为O(N*log(N))比较好？？
			auto size_vec = i - first;
			std::vector<value_type> vec(size_vec);
			for (int i = 0; i < size_vec)
			{
				vec[i] = *(first + i);
			}
			auto it = std::search(vec.begin(), vec.end(), *k);

			RandomAccessIterator j = i;
			RandomAccessIterator k = i - 1;
			while (wrapped_comp(tmp, *k)) {	//tmp<*k,看tmp是否小于它前一个数
				*j = *k;					//此时是小于*k，那么将前一个数赋予现在j迭代器的值
				j = k;						//j往前挪一个
				--k;						//k也往前挪一个
			}								//逆向依次判断tmp是否小于它前面的部分，
			*j = tmp;

		}
	}			//每经过一轮迭代，其i之前的数据都是已序的，然后只是当前值和已序序列比较，此谓插入排序
				//有序的前缀，无序的后缀
}


} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust