/*
 * @Author: Liu Yin
 * @Date: 2020-10-28 12:03:06
 * @LastEditTime: 2020-10-28 20:05:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \follow\thrust_follow\detail\vector_base.inl
 */


#include<thrust/detail/config.h>
#include "thrust_follow/detail/vector_base.h"
#include <thrust/detail/copy.h>
#include <thrust/detail/overlapped_copy.h>
#include <thrust/equal.h>
#include <thrust/distance.h>
#include <thrust/advance.h>
#include <thrust/detail/type_traits.h>
#include <thrust/detail/minmax.h>
#include <thrust/iterator/iterator_traits.h>
#include <thrust/detail/temporary_array.h>


#include <stdexcept>

namespace thrust {
namespace detail{
	//构造函数
	template<typename T,typename Alloc>
	vector_base<T, Alloc>::vector_base(void) :m_storage(), m_size(0) { ; }

	template<typename T, typename Alloc>
	vector_base<T, Alloc>::vector_base(const Alloc &alloc) : m_storage(alloc), m_size(0) { ; }


	template<typename T, typename Alloc>
	vector_base<T, Alloc>::vector_base(size_type n) : m_storage(), m_size(0) { default_init(n); }

	template<typename T, typename Alloc>
	vector_base<T, Alloc>::vector_base(size_type n,const value_type &value) : m_storage(), m_size(0) { default_init(n,value); }

	template<typename T, typename Alloc>
	vector_base<T, Alloc>::vector_base(size_type n, const Alloc&alloc) : m_storage(alloc), m_size(0) { default_init(n, value); }

	template<typename T,typename Alloc>
	vector_base<T, Alloc>::vector_base(size_type n, const value_type& value) : m_storage(), m_size(0) { fill_init(n, value); }

	template<typename T, typename Alloc>
	vector_base<T, Alloc>::vector_base(size_type n, const value_type& value,const Alloc&alloc) : m_storage(alloc), m_size(0) { fill_init(n, value); }


	//拷贝构造函数
	template<typename T,typename Alloc>
	vector_base<T, Alloc>
		::vector_base(const vector_base& v) : m_storage(copy_allocator_t(), v.m_storage), m_size(0) { range_init(v.begin, v.end()); }


	template<typename T, typename Alloc>
	vector_base<T, Alloc>
		::vector_base(const vector_base& v,const Alloc&alloc) : m_storage(alloc), m_size(0) { range_init(v.begin, v.end()); }


#if THRUST_CPP_DIALECT >=2011


#endif




	


	
	
}//detail
}//thrust