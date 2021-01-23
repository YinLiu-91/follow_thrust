#pragma once

#include <thrust/detail/config.h>
#include <thrust/system/cuda/memory_resource.h>
#include <thrust/memory.h>
#include <thrust/detail/type_traits.h>
#include <thrust/mr/allocator.h>
#include <ostream>

namespace thrust
{
namespace cuda_cub {
/*! Allocates an area of memory available to Thrust's <tt>cuda</tt> system.
 *  \param n Number of bytes to allocate.
 *  \return A <tt>cuda::pointer<void></tt> pointing to the beginning of the newly
 *          allocated memory. A null <tt>cuda::pointer<void></tt> is returned if
 *          an error occurs.
 *  \note The <tt>cuda::pointer<void></tt> returned by this function must be
 *        deallocated with \p cuda::free.
 *  \see cuda::free
 *  \see std::malloc
 */
inline __host__ __device__ pointer<void> malloc(std::size_t n);

/*! Allocates a typed area of memory available to Thrust's <tt>cuda</tt> system.
 *  \param n Number of elements to allocate.
 *  \return A <tt>cuda::pointer<T></tt> pointing to the beginning of the newly
 *          allocated elements. A null <tt>cuda::pointer<T></tt> is returned if
 *          an error occurs.
 *  \note The <tt>cuda::pointer<T></tt> returned by this function must be
 *        deallocated with \p cuda::free.
 *  \see cuda::free
 *  \see std::malloc
 */
template <typename T>
inline __host__ __device__ pointer<T> malloc(std::size_t n);

/*! Deallocates an area of memory previously allocated by <tt>cuda::malloc</tt>.
 *  \param ptr A <tt>cuda::pointer<void></tt> pointing to the beginning of an area
 *         of memory previously allocated with <tt>cuda::malloc</tt>.
 *  \see cuda::malloc
 *  \see std::free
 */
inline __host__ __device__ void free(pointer<void> ptr);

/*! \p cuda::allocator is the default allocator used by the \p cuda system's containers such as
 *  <tt>cuda::vector</tt> if no user-specified allocator is provided. \p cuda::allocator allocates
 *  (deallocates) storage with \p cuda::malloc (\p cuda::free).
 */
template<typename T>
using allocator = thrust::mr::stateless_resource_allocator<T, system::cuda::memory_resource>;

}    // namespace cuda_cub

namespace system {
namespace cuda {
using thrust::cuda_cub::malloc;
using thrust::cuda_cub::free;
using thrust::cuda_cub::allocator;
} // namespace cuda
} // namespace system
namespace cuda {
using thrust::cuda_cub::malloc;
using thrust::cuda_cub::free;
using thrust::cuda_cub::allocator;
}    // end cuda

} // end namespace thrust

#include <thrust/system/cuda/detail/memory.inl>