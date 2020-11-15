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

	//��ǰ�ƶ�������ֱ�������������ߵ�ĩβ
	while (first != last && !wrapped_pred(*first))
		++first;//��������������δ��ĩβ

	if (first == last)
		return first;//�Ƚ������������

	//���������ʼ��δfirst
	ForwardIterator result = first;

	++first;
	while (first != last) {				//���������δ���յ�
		if (!wrapped_pred(*first)) {	//�����ǰ��������ֵ��������
			*result = *first;			//�򣬵�ǰ��������ֵ����result
			++result;					//resultָ����һ��λ��
		}
		++first;						//��ǰָ��++
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

//δд��




} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust

