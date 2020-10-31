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

	/*template<typename T,typename Alloc>
	vector_base<T, Alloc>::vector_base(size_type n, const value_type& value) : m_storage(), m_size(0) { fill_init(n, value); }*/

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
	template<typename T,typename Alloc>
	vector_base<T, Alloc>::vector_base(vector_base&& v) : m_storage(copy_allocator_t(), v.m_storage), m_size(0) {
		*this = std:move(v);
	}
#endif

	template<typename T,typename Alloc>
	vector_base<T, Alloc>& vector_base<T, Alloc>::operator =(const vector_base& v) {
		if (this != &v) {
			m_storage.destroy_on_allocator_mismatch(v.m_storage, begin(), end());
			m_storage.deallocate_on_allocator_mismatch(v.m_storage);

			m_storage.propagate_allocator(v.m_storage);

			assign(v.begin(), v.end());
			
		}

		return *this;
	}


#if THRUST_CPP_DIALECT >=2011
	template<typename T,typename Alloc>
	vector_base<T, Alloc>& vector_base<T, Alloc>::operator=(vector_base&& v) {
		m_storage.destroy(begin(), end());
		m_storage = std::move(v.m_storage);
		m_size = std::move(v.m_size);

		v.m_storage = contiguous_storage<T, Alloc>(copy_allocator_t(), m_storage);
		v.m_size = 0;
		return *this;
	}

#endif
	//存在其他类型与Alloc的vector_base时
	//拷贝构造函数
	template<typename T,typename Alloc>
	template<typename OtherT,typename OtherAlloc>
	vector_base<T, Alloc>
		::vector_base(const vector_base<OtherT, OtherAlloc>& v) :m_storage(), m_size(0) {
		range_init(v.begin(), v.end());
	}


	//存在其他类型与Alloc的vector_base时
	//拷贝赋值函数

	template<typename T, typename Alloc>
	template<typename OtherT, typename OtherAlloc>
	vector_base<T, Alloc>&
		vector_base<T, Alloc>
		::operator=(const vector_base<OtherT, OtherAlloc>& v)
	{
		assign(v.begin(), v.end());

		return *this;
	} // end vector_base::operator=()

	//将标准容器vector拷贝给vector_base
	template<typename T,typename Alloc>
	template<typename OtherT,typename OtherAlloc>
	vector_base<T, Alloc>::vector_base(const std::vector<OtherT, OtherAlloc>& v):m_storage(),m_size(0) {
		range_init(v.begin(), v.end());
	}

	//将将标准容器vector赋值给vector_base
	template<typename T,typename Alloc>
	template<typename OtherT,typename OtherAlloc>
		vector_base<T, Alloc>& vector_base<T, Alloc>::operator =(const std::vector<OtherT, OtherAlloc>& v) {
			assign(v.begin(), v.end());
			return *this;
	}


		template<typename T,typename Alloc>
		template<typename IteratorIntegralType>//???
		void vector_base<T,Alloc>
			::init_dispatch(IteratorIntegralType begin, IteratorIntegralType value, true_type) {
			fill_init(n, value);
		}

		template<typename T,typename Alloc>
		void vector_base<T,Alloc>
			::default_init(size_type n) {
			if (n > 0) {
				m_storage.allocate(n);
				m_size = n;

				m_storage.default_construct_n(begin(), size());
			}
		}

		template<typename T,typename Alloc>
		void vector_base<T,Alloc>
			::fill_init(size_type n, const T& x)
		{
			if (n > 0) {
				m_storage.allocate(n);
				m_size = n;

				m_storage.uninitialized_fill_n(begin(), size(), x);
			}
		}//end vector_base::fill_init

		template<typename T,typename Alloc>
		template<typename InputIterator>
		void vector_base<T,Alloc>
			::init_dispatch(InputIterator first, InputIterator last, false_type)
		{
			range_init(first, last);
		}

		//2020.10.29
		//2020.10.30开始
		template<typename T,typename Alloc>
		template<typename InputIterator>
		void vector_base<T,Alloc>::range_init(InputIterator first, InputIterator last) {
			range_init(first, last, typename thrust::iterator_traversal<InputIterator>::type());
		}

		template<typename T,typename Alloc>
		template<typename InputIterator>
		void vector_base<T,Alloc>
			::range_init(InputIterator first, InputIterator last, thrust::incrementable_traversal_tag) {
			for (; first != last; ++first)
				push_back(*first);
		}

		template<typename T,typename Alloc>
		template<typename FowardIterator>
		void vector_base<T, Alloc>::range_init(FowardIterator first, FowardIterator last, thrust::random_access_traversal_tag)
		{
			size_type new_size = thrust::distance(first, last);
			allocate_and_copy(new_size, first, last, m_storage);
			m_size = new_size;
		}


		template<typename T,typename Alloc>
		template<typename InputIterator>
		vector_base<T,Alloc>
			::vector_base(InputIterator first, InputIterator last) :m_storage(), m_size(0) {
			typedef thrust::detail::is_integral<InputIterator> Integer;//这是什么？？？
			init_dispatch(first, last, Integer());
		}

		template<typename T,typename Alloc>
		template<typename InputIterator>
		vector_base<T,Alloc>
			::vector_base(InputIterator first, InputIterator last, const Alloc& alloc):m_storage(alloc),m_size(0)
		{
			//检查 InputIterator的类型：如果他是整形，
			//我们需要将此解释为（size_type,value_type)
			typedef thrust::detail::is_integral<InputIterator> Integer;
			init_dispatch(first, last, Integer());
		}

		template<typename T,typename Alloc>
		void vector_base<T,Alloc>
			::resize(size_type new_size)
		{
			if (new_size < size())
			{
				iterator new_end = begin();//advance中有new_end()+=new_size();
				thrust::advance(new_end, new_size);
				erase(new_end, end());
			} 
			else
			{
				append(new_size, -size());
			}
		}

		template<typename T,typename Alloc>
		void vector_base<T,Alloc>
			::resize(size_type new_size, const value_type& x)
		{
			if (new_size < size())
			{
				iterator new_end = begin();
				thrust::advance(new_end, new_size);
				erase(new_end, end());
			}
			else
			{
				insert(end(), new_size - size());
			}
		}

		template<typename T,typename Alloc>
		typename vector_base<T,Alloc>::size_type
			vector_base<T, Alloc>::size(void)const {
			return m_size;
		}

		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::size_type
			vector_base<T, Alloc>::max_size(void)const {
			return m_storage.max_size();
		}

		template<typename T,typename Alloc>
		void vector_base<T,Alloc>
			::reserve(size_type n)
		{
			if (n > capacity())
			{
				allocate_and_copy(n, begin(), end(), m_storage);
			}
		}


		template<typename T,typename Alloc>
		typename vector_base<T,Alloc>::size_type 
			vector_base<T,Alloc>::
			capacity(void)const {
			return m_storage.size();
		}

		template<typename T,typename Alloc>
		void vector_base<T,Alloc>::
		shrink_to_fit(void) {
			//这里怎么做的？？？
			vector_base(*this).swap(*this);
		}

		template<typename T,typename Alloc>
		typename vector_base<T,Alloc>::reference
			vector_base<T, Alloc>::operator[](const size_type n) {
			return m_storage[n];
		}

		template<typename T,typename Alloc>
		typename vector_base<T,Alloc>::const_reference
			vector_base<T, Alloc>::operator[](const size_type n)const {
			return m_storage[n];
		}//返回const类型的

		template<typename T,typename Alloc>
		typename vector_base<T,Alloc>::iterator
			vector_base<T, Alloc>::begin(void) {
			return m_storage.begin();
		}

		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::const_iterator
			vector_base<T, Alloc>::begin(void) const{
			return m_storage.begin();
		}//返回const begin()

		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::const_iterator
			vector_base<T, Alloc>::cbegin(void) const {
			return m_storage.begin();
		}//返回const begin()


		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::reverse_iterator
			vector_base<T, Alloc>::rbegin(void) {
			return reverse_iterator(end());//rbegin的实现
		}

		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::const_reverse_iterator
			vector_base<T, Alloc>::rbegin(void) const {
			return const_reverse_iterator(end());
		}//返回const rbegin()

		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::const_reverse_iterator
			vector_base<T, Alloc>::crbegin(void) const {
			return rbegin();
		}//返回const crbegin()


		template<typename T,typename Alloc>
		typename vector_base<T,Alloc>::iterator
			vector_base<T,Alloc>
			::end(void)
		{
			
			iterator result = begin();
			thrust::advance ( result, size());//
			return result;
		}//为什么没有end?





	

	


	


	
	
}//detail
}//thrust