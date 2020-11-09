#pragma  once
#include <thrust/detail/config.h>

namespace thrust
{
namespace system
{
namespace detail
{
namespace sequential
{
__thrust_exec_check_disable__ 
template<typename BidirectionalIterator1, typename  BidirectionalIterator2>
__host__ __device__
BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result)
{
	while (first!=last)
	{
		--last;
		--result;
		*result = *last;//��ǰ���������Χ����ֵ
	}
}
return result;//����result��λ�� 

} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust
