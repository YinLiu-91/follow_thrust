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
	//�������ݱ�����ҵ�������
	OutputIterator unique_copy(sequential::execution_policy<DerivedPolicy>&,
		InputIterator first,
		InputIterator last,
		OutputIterator output,
		BinaryPredicate binary_pred)
{
	typedef typename thrust::detail::iterator_traits<InputIterator>::value_type;

	if (first != last) {			//���Ԫ����Ŀ��Ϊ0����ô
		T prev = *first;			//����һ��ֵ����prev
		for (++first; first != last; ++first) {	//++first
			T temp = *first;		//����ʱ��*first����temp
			if (!binary_pred(prev, temp)) {//���prev��temp����ȣ���
				*output = prev;				//��ʱ��Ԫ������unique������ֵ����output����
				++output;					//��output��������1����	
				prev = temp;				//��temp��ֵ����prev��֮ǰ�����Ա������һ�ε���
			}
		}
		*output = prev;						//������ѭ����*output���ձ���ֵ����if֮���prev�����Ǿ���prev=temp��ֵ֮��ģ��������Ҫ��ֵ����
		++output;							//��output��1
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
