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
	//���캯��
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


	//�������캯��
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
	//��������������Alloc��vector_baseʱ
	//�������캯��
	template<typename T,typename Alloc>
	template<typename OtherT,typename OtherAlloc>
	vector_base<T, Alloc>
		::vector_base(const vector_base<OtherT, OtherAlloc>& v) :m_storage(), m_size(0) {
		range_init(v.begin(), v.end());
	}


	//��������������Alloc��vector_baseʱ
	//������ֵ����

	template<typename T, typename Alloc>
	template<typename OtherT, typename OtherAlloc>
	vector_base<T, Alloc>&
		vector_base<T, Alloc>
		::operator=(const vector_base<OtherT, OtherAlloc>& v)
	{
		assign(v.begin(), v.end());

		return *this;
	} // end vector_base::operator=()

	//����׼����vector������vector_base
	template<typename T,typename Alloc>
	template<typename OtherT,typename OtherAlloc>
	vector_base<T, Alloc>::vector_base(const std::vector<OtherT, OtherAlloc>& v):m_storage(),m_size(0) {
		range_init(v.begin(), v.end());
	}

	//������׼����vector��ֵ��vector_base
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
		//2020.10.30��ʼ
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
			typedef thrust::detail::is_integral<InputIterator> Integer;//����ʲô������
			init_dispatch(first, last, Integer());
		}

		template<typename T,typename Alloc>
		template<typename InputIterator>
		vector_base<T,Alloc>
			::vector_base(InputIterator first, InputIterator last, const Alloc& alloc):m_storage(alloc),m_size(0)
		{
			//��� InputIterator�����ͣ�����������Σ�
			//������Ҫ���˽���Ϊ��size_type,value_type)
			typedef thrust::detail::is_integral<InputIterator> Integer;
			init_dispatch(first, last, Integer());
		}

		template<typename T,typename Alloc>
		void vector_base<T,Alloc>
			::resize(size_type new_size)
		{
			if (new_size < size())
			{
				iterator new_end = begin();//advance����new_end()+=new_size();
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
			//������ô���ģ�����
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
		}//����const���͵�

		template<typename T,typename Alloc>
		typename vector_base<T,Alloc>::iterator
			vector_base<T, Alloc>::begin(void) {
			return m_storage.begin();
		}

		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::const_iterator
			vector_base<T, Alloc>::begin(void) const{
			return m_storage.begin();
		}//����const begin()

		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::const_iterator
			vector_base<T, Alloc>::cbegin(void) const {
			return m_storage.begin();
		}//����const begin()


		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::reverse_iterator
			vector_base<T, Alloc>::rbegin(void) {
			return reverse_iterator(end());//rbegin��ʵ��
		}

		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::const_reverse_iterator
			vector_base<T, Alloc>::rbegin(void) const {
			return const_reverse_iterator(end());
		}//����const rbegin()

		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::const_reverse_iterator
			vector_base<T, Alloc>::crbegin(void) const {
			return rbegin();
		}//����const crbegin()


		template<typename T,typename Alloc>
		typename vector_base<T,Alloc>::iterator
			vector_base<T,Alloc>
			::end(void)
		{
			
			iterator result = begin();
			thrust::advance ( result, size());//
			return result;
		}//Ϊʲôû��end?

		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::const_iterator
			vector_base<T, Alloc>
			::end(void)const
		{

			iterator result = begin();
			thrust::advance(result, size());//
			return result;
		}


		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::const_iterator
			vector_base<T, Alloc>
			::cend(void)const
		{
			return end();
		}


		template<typename T,typename Alloc>
		typename vector_base<T,Alloc>::reverse_iterator 
			vector_base<T,Alloc>
			::rend(void) {
			return const_reverse_iterator(begin());
		}
		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::const_reverse_iterator
			vector_base<T, Alloc>
			::rend(void)const {
			return const_reverse_iterator(begin());
		}

		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::const_reverse_iterator
			vector_base<T, Alloc>
			::crend(void)const {
			return rend();//���������
		}

		template<typename T,typename Alloc>
		typename vector_base<T,Alloc>::reference
			vector_base<T,Alloc>
			::front(void)
		{
			return *begin();
		}

		template<typename T,typename Alloc>
		typename vector_base<T,Alloc>::const_reference
			vector_base<T,Alloc>::
			back(void)const
		{
			const_iterator ptr_to_back = end();
			--ptr_to_back
				return *ptr_to_back;
		}


		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::reference
			vector_base<T, Alloc>::
			back(void)
		{
			iterator ptr_to_back = end();
			--ptr_to_back;
			return *ptr_to_back;
		}

		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::pointer
			vector_base<T, Alloc>::
			data(void)
		{
			return pointer(&front());
		}


		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::const_pointer
			vector_base<T, Alloc>::
			data(void)const
		{
			return const_pointer(&front());
		}


		typename<typename T,typename Alloc>
			vector_base<T,Alloc>
			::~vector_base(void) {
			if (!empty())
				m_storage.destroy(begin(), end());//m_storage��һ��ԭʼ�ڴ棿��
		}

		template<typename T,typename Alloc>
		void vector_base<T,Alloc>
			::clear(void) {
			erase(begin(), end());
		}

		template<typename T,typename Alloc>
		bool vector_base<T,Alloc>
			::empty(void)const {
			return size() == 0;
		}

		template<typename T,typename Alloc>
		void vector_base<T,Alloc>
			::push_back(const value_type& x) {
			insert(end(), x);
		}

		template<typename T,typename Alloc>
		void vector_base<T, Alloc>::pop_back(void)
		{
			iterator e = end();
			iterator ptr_to_back = e;
			--ptr_to_back;
			m_storage.destroy(ptr_to_back, e);
			--m_size;
		}//�ɲ����Լ�Щ

		template<typename T, typename Alloc>
		typename vector_base<T,Alloc>::iterator 
			vector_base<T, Alloc>::erase(iterator pos)
		{
			iterator end = pos;
			++end;
			return erase(pos, end);
		}//����ɾ��һ�������ɾ��

		template<typename T, typename Alloc>
		typename vector_base<T, Alloc>::iterator
			vector_base<T,Alloc>::
			erase(iterator first, iterator last)
		{
			//����last��end()��Ԫ��˳��ǰ�Ƶ����Ϊfirstλ�ô�
			iterator i = thrust::detail::overlapped_copy(last, end(), first);

			//����i�����һ��Ԫ��
			m_storage.destroy(i, end());

			//������С
			m_size -= (last - first);

			//����ָ��ɾ��Ԫ�صĵ�һ��λ��
			return first;
		}


		template<typename T,typename Alloc>
		void vector_base<T, Alloc>
			::swap(vector_base& v) {
			thrust::swap(m_storage, v.m_storage);
			thrust::swap(m_size, v.m_size);
		}

		template<typename T,typename Alloc>
		void vector_base<T,Alloc>::
			assign(size_type n, const T& x)
		{
			fill_assign(n, x);
		}

		template<typename T,typename Alloc>
		template<typename InputIterator>
		void vector_base<T,Alloc>:: 
			assign(InputIterator first, InputIterator last) {

			//�����Ѿ�����(n,x)��ʽ��assign
			//Ϊ�����������ԣ��Ե���������:
			typedef typename thrust::detail::is_integral<InputIterator> integral;
			assign_dispatch(first, last, integral());
		}

		template<typename T,typename Alloc>
		typename vector_base<T,Alloc>::allocator_type
			vector_base<T,Alloc>
			::get_allocator(void)const {
			return m_storage.get_allocator();
		}

		template<typename T,typename Alloc>
		typename vector_base<T,Alloc>::iterator
			vector_base<T,Alloc>::
			insert(iterator position, const T& x) {
			size_type index = thrust::distance(begin(), position);
			insert(position, 1, x);

			iterator result = begin();
			thrust::advance(result, index);
			return result;
		}


		template<typename T,typename Alloc>
		void vector_base<T,Alloc>
			::insert(iterator pisition, size_type n, const T& x)
		{
			fill_insert(position, n, x);
		}

		template<typename T,typename Alloc>
		template<typename InputIterator>
		void vector_base<T,Alloc>
			::insert(iterator position, InputIterator first, InputIterator last)
		{
			//Ϊ����������һ���Ĳ�ͬ(position,n,x)
			typedef typename thrust::detail::is_integral<InputIterator> integral;
			insert_dispatch(position, first, last, integral());
		}


		template<typename T,typename Alloc>
		template<typename InputIterator>
		void vector_base<T,Alloc>
			::assign_dispatch(InputIterator first, InputIterator last, false_type)
		{
			range_assign(first, last);
		}

		template<typename T,typename Alloc>
		template<typename Integral>
		void vector_base<T,Alloc>:: 
			assign_dispatch(Integral n, Integral x, true_type)
		{
			fill_assign(n, x);
		}

		template<typename T,typename Alloc>
		template<typename InputIterator>
		void  vector_base<T,Alloc>:: 
			insert_dispatch(iterator position, InputIterator first, InputIterator last, false_type)
		{
			copy_insert(position, first, last);

		}

		template<typename T, typename Alloc>
		template<typename integral>
		void  vector_base<T, Alloc>::
			insert_dispatch(iterator position, integral n, integral x, true_type)
		{
			fill_insert(position, first, last);

		}

		template<typename T,typename Alloc>
		template<typename ForwardIterator>
		void vector_base<T,Alloc>
			::copy_insert(iterator position, InputIterator first, InputIterator last)
		{
			if (first != last) {
				//�ж�����Ԫ�ؽ��ᱻ����
				const size_type num_new_elements = thrust::distance(first, last);
				if (capacity() - size() >= num_new_elements) {
					//�����г���Ŀռ�����
					//�ж������ڵ�Ԫ�ؽ��ᱻ���

					const size_type num_displaced_elements = end() - position;
					iterator old_end = end();
					if (num_displaced_elements > num_new_elements) {
						//���첢���� n��Ԫ��
					}
				}
			}
		}





	

	


	


	
	
}//detail
}//thrust