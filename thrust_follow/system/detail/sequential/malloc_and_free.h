#progma once
namespace thrust
{
namespace system
{
namespace detail
{
namespace sequential
{


template<typename DerivedPolicy>
inline __host__ __device__
void *malloc(execution_policy<DerivedPolicy> &, std::size_t n)
{
	return std::malloc(n);//thrust的malloc调用的是std::malloc
}
template<typename DerivedPolicy, typename Pointer>
inline __host__ __device__
void free(sequential::execution_policy<DerivedPolicy>&, Pointer ptr)
{
	std::free(thrust::raw_pointer_cast(ptr));
} // end mallc()

} // end sequential
} // end detail
} // end system
} // end thrust