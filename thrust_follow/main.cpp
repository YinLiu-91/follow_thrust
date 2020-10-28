#include "detail/vector_base.h"
int main(int argc, char** argv) {
	thrust::detail::vector_base<int,std::allocator<int>> i;
	thrust::detail::vector_base<int, std::allocator<int>> i1 = i;
	return 0;
}