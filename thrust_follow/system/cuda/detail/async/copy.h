#pragma once
#include <thrust/detail/config.h>
#include <thrust/detail/cpp14_required.h>

#if THRUST_CPP_DIALECT >= 2014

#if THRUST_DEVICE_COMPILER == THRUST_DEVICE_COMPILER_NVCC

#include <thrust/system/cuda/config.h>

#include <thrust/system/cuda/detail/async/customization.h>
#include <thrust/system/cuda/detail/async/transform.h>
#include <thrust/system/cuda/detail/cross_system.h>
#include <thrust/system/cuda/future.h>
#include <thrust/iterator/iterator_traits.h>
#include <thrust/type_traits/logical_metafunctions.h>
#include <thrust/detail/static_assert.h>
#include <thrust/type_traits/is_trivially_relocatable.h>
#include <thrust/type_traits/is_contiguous_iterator.h>
#include <thrust/distance.h>
#include <thrust/advance.h>
#include <thrust/uninitialized_copy.h>

#include <type_traits>

namespace thrust
{

namespace system {
namespace cuda {
namespace detail
{
template<typename FromPolicy,typename Topolicy,
	typename ForwardIt,typename OutputIt,typename Size>
	auto async_copy_n(
		FromPolicy& from_exec,
		Topolicy& to_exec,
		ForwardIt first,
		Size n,
		OutputIt output
		//使用了尾后推断？？
		//\c is_trivially_relocatable, an extensible type trait indicating
		//* whether a type can be bitwise copied(e.g.via \c memcpy).
	)-> typename std::enable_if<is_indirectly_trivially_relocatable_to< ForwardIt, OutputIt>::value, unique_eager_event>::type {
	using T = typename iterator_traits< ForwardIt>::value_type;//萃取迭代器类型来定义T的类型

	auto const device_alloc = get_async_device_allocator(select_device_system(from_exec, to_exec));
	using pointer = typename thrust::detail::allocator_traits<decltype(device_alloc)>::template rebind_traitts<void>::pointer;//???

	unique_eager_event e;

	//Set up stream with dependences
	//依据相互依赖关系创建流

	cudaStream_t const user_raw_stream = thrust::cuda_cub::stream(select_device_system(from_exec, to_exec));

	if (thrust::cuda_cub::default_stream() != user_raw_stream) {
		e = make_dependent_event(
			std::tuple_cat(
				std::make_tuple(
					unique_stream(nonowning, user_raw_stream)
				),
				extract_dependencies(std::move(thrust::detail::derived_cast(from_exec)),
				extract_dependencies(std::move(thrust::detail::derived_cast(to_exec))
					)
				)
			);
	}


}









} // cuda_cub

} // end namespace thrust

#endif // THRUST_DEVICE_COMPILER == THRUST_DEVICE_COMPILER_NVCC

#endif