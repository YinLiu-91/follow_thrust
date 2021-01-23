#progma once 
#include <thrust/detail/config.h>
#include <thrust/detail/cpp14_required.h>

#if THRUST_CPP_DIALECT >= 2014

#if THRUST_DEVICE_COMPILER == THRUST_DEVICE_COMPILER_NVCC

#include <thrust/system/cuda/config.h>

#include <thrust/system/cuda/detail/async/customization.h>
#include <thrust/system/cuda/detail/parallel_for.h>
#include <thrust/system/cuda/future.h>
#include <thrust/iterator/iterator_traits.h>
#include <thrust/distance.h>

#include <type_traits>

namespace thrust
{

namespace system {
namespace cuda {
namespace detail
{
template<typename ForwardIt, typename UnaryFunction>
struct async_for_each_fn {

	ForwardIt first;
	UnaryFunction f;
	__host__ __device__ 
		async_for_each_fn(ForwardIt&& first_,UnaryFunction &&f_):first(std::move(first_)),f(std::move(f_)){}//构造函数
	template<typename Index>
	__host__ __device__ 
		void operator()(Index idx) {//函数对象
		f(thrust::raw_reference_cast(first[idx]));
	}
};

template<
	typename DerivedPolicy,
	typename ForwardIt,
	typename Size,
	typename UnaryFunction
>
auto async_for_each_n(
	execution_policy<DerivedPolicy>& policy,
	ForwardIt                        first,
	Size                             n,
	UnaryFunction                    func
) ->unique_eager_event//尾置返回类型
{
	unique_eager_event e;

	//set up stream with dependencies
	//这里不懂
	cudaStream_t const user_raw_stream = thrust::cuda_cub::stream(policy);
	if (thrust::cuda_cub::default_stream() != user_raw_stream)
	{
		e = make_dependent_event(
			std::tuple_cat(
				std::make_tuple(
					unique_stream(nonowning, user_raw_stream)
				)
				, extract_dependencies(
					std::move(thrust::detail::derived_cast(policy))
				)
			)
		);
	}
	else
	{
		e = make_dependent_event(
			extract_dependencies(
				std::move(thrust::detail::derived_cast(policy))
			)
		);
	}

	//Run for each
	async_for_each_fn<ForwardIt, UnaryFunction> wrapped(std::move(first), std::move(func));

	thrust::cuda_cub::throw_on_error(
		thrust::cuda_cub::__parallel_for::parallel_for(
			n, std::move(wrapped), e.stream().native_handle()
		)
		, "after for_each launch"
	);

	return e;



}








}
}
}
} // namespace system::cuda::detail


namespace cuda_cub
{

// ADL entry point.
template <
	typename DerivedPolicy
	, typename ForwardIt, typename Sentinel, typename UnaryFunction
>
auto async_for_each(
	execution_policy<DerivedPolicy>& policy,
	ForwardIt                        first,
	Sentinel                         last,
	UnaryFunction&& func
)
THRUST_RETURNS(
	thrust::system::cuda::detail::async_for_each_n(
		policy, first, distance(first, last), THRUST_FWD(func)
	)
);

} // cuda_cub

} // end namespace thrust

#endif // THRUST_DEVICE_COMPILER == THRUST_DEVICE_COMPILER_NVCC

#endif