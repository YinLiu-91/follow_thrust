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

	typedef typename thrust::iterator_value< RandomAccessIterator>::type value_type;//�õ����Ͳ�typedef
	if (first == last)return;
	//warp comp
	thrust::detail::wrapped_function< StrictWeakOrdering, bool>wrapped_comp(comp);
	for (RandomAccessIterator i = first + 1; i != last; ++i) {
		value_type tmp = *i;//�����ò���ֵ��tmp
		//wrapped_comp�Ƚϵ�һ�����Ƿ�С�ڵڶ�����С��Ϊ��
		if (wrapped_comp(tmp, *first)) {//firstʼ���ǵ�ǰɨ�赽����������С��ֵ
			//tmp ��ĿǰΪֹ��С��ֵ
			sequential::copy_backward(first, i, i + 1);//�����ǽ���--i;--(i+1);*(i+1)=*i;�ǽ�first��ֵ����ڶ���Ԫ�أ����copy_backward)

			*first = tmp;//����Сֵ��first
		}
		else {
			//tmp���ǵ�ǰɨ����������С�ģ����Ա������Ƿ���ڵ�һ��
			//ǰ��ҵ������Ϊʲô���ಽ�����Ĳ��ң����������ӶȾ���O(N^2)???ʵ��ΪO(N*log(N))�ȽϺã���
			auto size_vec = i - first;
			std::vector<value_type> vec(size_vec);
			for (int i = 0; i < size_vec)
			{
				vec[i] = *(first + i);
			}
			auto it = std::search(vec.begin(), vec.end(), *k);

			RandomAccessIterator j = i;
			RandomAccessIterator k = i - 1;
			while (wrapped_comp(tmp, *k)) {	//tmp<*k,��tmp�Ƿ�С����ǰһ����
				*j = *k;					//��ʱ��С��*k����ô��ǰһ������������j��������ֵ
				j = k;						//j��ǰŲһ��
				--k;						//kҲ��ǰŲһ��
			}								//���������ж�tmp�Ƿ�С����ǰ��Ĳ��֣�
			*j = tmp;

		}
	}			//ÿ����һ�ֵ�������i֮ǰ�����ݶ�������ģ�Ȼ��ֻ�ǵ�ǰֵ���������бȽϣ���ν��������
				//�����ǰ׺������ĺ�׺
}


} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust