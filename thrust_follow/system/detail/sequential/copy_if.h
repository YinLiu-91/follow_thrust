#progma once

namespace thrust
{
namespace system
{
namespace detail
{
namespace sequential
{
__thrust_exec_check_disable__ 
template <typename DerivedPolicy,
typename InputIterator1,
typename InputIterator2,
typename OutputIterator,
typename Predicate>
__host__ __device__
OutputIterator copy_if(sequential::execution_policy<DerivedPolicy>&,
	InputIterator1 first,
	InputIterator1 last,
	InputIterator2 stencil,
	OutputIterator result,
	Predicate pred)
{
	thrust::detail::wrapped_function<Predicate, bool>wrapped_pred(pred);//????

	while (first != last)
	{
		if (wrapped_pred(*stencil)) {
			*result = *first;
			++result;
		}//pred表示判断？？
		++first;
		++stencil;
	}

	return result;//返回迭代器// 

}


} // end namespace sequential
} // end namespace detail
} // end namespace system
} // end namespace thrust