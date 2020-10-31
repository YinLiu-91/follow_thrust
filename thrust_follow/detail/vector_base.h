#pragma once
#include <thrust/iterator/detail/normal_iterator.h>
#include <thrust/iterator/reverse_iterator.h>
#include <thrust/iterator/iterator_traits.h>
#include <thrust/detail/type_traits.h>
#include <thrust/detail/config.h>
#include <thrust/detail/contiguous_storage.h>
#include <vector>
namespace thrust {

	namespace detail {
		template<typename T, typename Alloc>
		class vector_base {
		private:
			typedef thrust::detail::contiguous_storage<T, Alloc> storage_type;

		public:
			//typedefs
			typedef typename storage_type::value_type			value_type;
			typedef typename storage_type::pointer				pinter;
			typedef typename storage_type::const_pointer		const_pointer;
			typedef typename storage_type::reference			reference;
			typedef typename storage_type::const_reference		const_reference;
			typedef typename storage_type::size_type			size_type;
			typedef typename storage_type::difference_type		difference_type;
			typedef typename storage_type::allocator_type		allocator_type;

			typedef typename storage_type::iterator				iterator;
			typedef typename storage_type::const_iterator		const_iterator;

			typedef thrust::reverse_iterator					reverse_iterator;
			typedef thrust::reverse_iterator<const_iterator>	const_reverse_iterator;

			/*! 创建了一个空的vector_base
			*/


			vector_base(void);

			/*!此创建一个空的vector_base
			* \param alloc 是此vector_base使用的分配器
			*/
			explicit vector_base(const Alloc& alloc);

			/*!此构造函数创建一个默认构造元素的vector_base
			*  \param n 构造元素的数量
			*/
			explicit vector_base(size_type n);


			/*!创建给定元素数量的vector_base
			* \param n 创建的元素数量
			* \param alloc 此vector_base使用的分配器
			*/
			explicit vector_base(size_type n, const Alloc& alloc);

			/*!此构造函数创建一个有着一个值的vector_base
			* \param n 元素数量
			* \param value 元素的值
			*/
			explicit vector_base(size_type n, const value_type& value);

			/*!此构造函数创建一个有着一个值并且指定分配器的vector_base
			* \param n 元素数量
			* \param value 元素的值
			* \param alloc 指定的分配器
			*/

			explicit vector_base(size_type n, const value_type& value, const Alloc& alloc);


			/*!拷贝构造函数：拷贝一个vector_base
			* \param v 被拷贝的vector_base
			* \param
			*/

			vector_base(const vector_base& v);

			/*!拷贝构造函数：拷贝一个指定分配器vector_base
			* \param v 被拷贝的vector_base
			* \param alloc 指定的分配器
			*/
			vector_base(const vector_base& v, const Alloc& alloc);

#if THRUST_CPP_DIALECT >=2011
			/*!移动构造函数移动另一个vector_base
			* \param v :被移动的vector_base
			*/
			vector_base(vector_base&& v);
#endif

			/*!拷贝赋值运算符将拷贝另一个vector_base 赋值给右边的vector_base
			* \param v 是被拷贝的vector_base
			*/

			vector_base& operator=(const vector_base& v);

#if THRUST_CPP_DIALECT >=2011
			/*!移动拷贝赋值运算符将另一个vector_base 移动拷贝给右边的vector_base
			* \param 是被拷贝的vector_base
			*/

			vector_base& operator=(vector_base&& v);
#endif

			/*!拷贝构造一个不同类型的vector_base
			* \param v 将要被拷贝的vector_base
			*/

			template<typename OtherT, typename OtherAlloc>
			vector_base(const vector_base<OtherT, OtherAlloc>& v);


			template<typename OtherT, typename OtherAlloc>
			vector_base& operator=(const vector_base<OtherT, OtherAlloc>& v);

			/*!拷贝赋值一个不同类型的vector_base
			* \param v 将要被拷贝的vector_base
			*/
			template<typename OtherT, typename OtherAlloc>
			vector_base& operator=(const std::vector<OtherT, OtherAlloc>& v);

			/*!拷贝构造一个不同类型的std::vector
			* \param v 将要被拷贝的vector
			*/

			template<typename OtherT, typename OtherAlloc>
			vector_base(const std::vector<OtherT, OtherAlloc>& v);


			/*!拷贝赋值一个不同类型的std::vector
			* \param v 将要被拷贝的vector
			*/
			/*template<typename OtherT, typename OtherAlloc>
			vector_base& operator=(const std::vector<OtherT, OtherAlloc>& v);*/



			template<typename InputIterator>
			void range_init(InputIterator first, InputIterator last);


			/*!此构造函数源来自于一个范围
			* \param first 此范围的开始
			* \param last 此范围的结束
			*/
			template<typename InputIterator>
			vector_base(InputIterator first, InputIterator last);


			/*!此构造函数源来自于一个带分配器的范围
			* \param first 此范围的开始
			* \param last 此范围的结束
			*/
			template<typename InputIterator>
			vector_base(InputIterator first, InputIterator last, const Alloc& alloc);


			/*!析构函数
			*
			*/
			~vector_base(void);


			/*!\brief 将vector_base resize到确定的数量，多余的默认构造
			* \param new_size 此vector_base应当保有的数量
			* \throw 如果超出最大数量 max_size9 抛出std::length_error
			*/
			void resize(size_type new_size);

			/*!\brief 将vector_base resize到确定的数量,多余的给予值x
			* \param new_size 此vector_base应当保有的数量
			* \throw 如果超出最大数量 max_size9 抛出std::length_error
			* \ x 新的元素应当填充的值
			*/

			void resize(size_type new_size, const value_type& x);

			/*! 返回vector_base含有元素的个数
			* \

			*/
			size_type size(void)const;

			/*! 返回vector_base可以有的最大元素个数
			*
			*/
			size_type max_size(void)const;

			/*! \brief 如果n小于vector_base的大小，则此函数没有影响，
			* 否则，此操作将会增加内存，如果内存请求成功，则容量capacity()大于n，
			* 否则容量不会变化，不论在任何情况下，size()的大小都不变
			* \throw 如果n超过了max_size() 那么会抛出std::length_error
			*/

			void reserve(size_type n);


			/*! 返回reserve()的数量
			*/
			size_type capacity(void)const;

			/*!使得size()和capacity的数量一样
			*
			*/
			void shrink_to_fit(void);

			/*!\brief 得到vector_base的元素的下标
			* \param n 元素下标
			* \return  写入/读取的数据
			*/
			reference operator[](size_type n);

			/*!\brief 得到vector_base的元素的const 下标
			* \param n 元素下标
			* \return  读取的数据
			*/
			const_reference operator[](size_type n)const;


			/*!返回指向vetor_base起始点的迭代器
			* \return mStart
			*/
			iterator begin(void);

			/*!返回指向vetor_base起始点的const迭代器
			* \return mStart
			*/
			const_iterator begin(void)const;

			/*!返回指向vetor_base起始点的const迭代器
			* \return mStart
			*/
			const_iterator cbegin(void)const;

			/*! 返回反向迭代器
			* \return 指向vector_base反向的反向迭代器
			*/
			reverse_iterator rbegin(void);

			/*! 返回const反向迭代器
			* \return 指向vector_base反向的反向迭代器
			*/
			const_reverse_iterator rbegin(void)const;

			/*! 返回const反向迭代器
			* \return 指向vector_base反向的反向迭代器
			*/
			const_reverse_iterator crbegin(void)const;

			/*!返回指向vector_base最后一个元素后面的一个迭代器
			* \return begin()+size()
			*/
			iterator end(void);

			/*!返回指向vector_base最后一个元素后面的一个const迭代器
			* \return begin()+size()
			*/
			const_iterator end(void)const ;

			/*!返回指向vector_base最后一个元素后面的一个const迭代器
			* \return begin()+size()
			*/
			const_iterator cend(void);


			/*!返回指向反向vector_base序列最后一个元素后面的一个迭代器
			* \return rbegin()+size()
			*/
			reverse_iterator rend(void);

			/*!返回指向反向vector_base序列最后一个元素后面的一个const迭代器
			* \return begin()+size()
			*/
			const_reverse_iterator rend(void)const ;

			/*!返回指向反向vector_base序列最后一个元素后面的一个const迭代器
			* \return begin()+size()
			*/
			const_reverse_iterator crend(void)const ;


			const_reference front(void)const;

			reference front(void);

			const_reference back(void)const;

			reference back(void);

			pointer data(void);

			const_pointer data(void)const;

			void clear(void);

			bool empty(void)const;

			void push_back(const value_type& x);

			void pop_back(void);

			void swap(vector_base& v);

			iterator erase(iterator pos);

			iterator erase(iterator first, iterator last);

			iterator insert(iterator position, const T& x);

			void insert(iterator pisition, size_type n, const T& x);

			template<typename InputIterator>
			void insert(iterator position, InputIterator first, InputIterator last);

			void assign(size_type n, const T& x);

			template <typename InputIterator>
			void assign(InputIterator first, InputIterator last);


			allocator_type get_allocator(void)const;

		protected:

			storage_type m_storage;

			size_type m_size;
			//
		private:

			template<typename IteratorIntegralType>
			void init_dispatch(IteratorIntegralType begin, IteratorIntegralType end, false_type);

			template<typename IteratorIntegralType>
			void init_dispatch(IteratorIntegralType begin, IteratorIntegralType value, true_type);

			/*template<typename InputInterator>
			void range_init(InputInterator first, InputInterator last);*/

			template<typename InputInterator>
			void range_init(InputInterator first, InputInterator last,thrust::incrementable_traversal_tag);

			template<typename ForwardIterator>
			void range_init(ForwardIterator first, ForwardIterator last, thrust::random_access_traversal_tag);

			void default_init(size_type n);

			void fill_init(size_type n, const T& x);

			template<typename InputIteratorOrIntegralType>
			void insert_dispatch(iterator position, InputIteratorOrIntegralType first, InputIteratorOrIntegralType last, false_type);

			template<typename InputIteratorOrIntegralType>
			void insert_dispatch(iterator position, InputIteratorOrIntegralType first, InputIteratorOrIntegralType x, true_type);

			void append(size_type n);

			void fill_insert(iterator position, size_type n, const T& x);

			template<typename InputIterator>
			void copy_insert(iterator position, InputIterator first, InputIterator last);

			template<typename InputIterator>
			void assign_dispatch(InputIterator first, InputIterator last, false_type);

			template<typename Integral>
			void assign_dispatch(Integral n, Integral x, true_type);

			template<typename InputIterator>
			void range_assign(InputIterator first, InputIterator last);

			template<typename RandomAccessIterator>
			void range_assign(RandomAccessIterator first, RandomAccessIterator last, thrust::random_access_traversal_tag);

			template<typename InputIterator>
			void range_assign(InputIterator first, InputIterator last, thrust::incrementable_traversal_tag);

			void fill_assign(size_type n, const T& x);

			template<typename ForwardIterator>
			void allcate_and_copy(size_type requested_size, ForwardIterator first, ForwardIterator last, storage_type& new_storage);
		};
	}//detail


	template<typename T, typename Alloc>
		void swap(detail::vector_base<T, Alloc>& a,
			detail::vector_base<T, Alloc>& b);

	template<typename T1,typename Alloc1,typename T2,typename Alloc2>
	void operator==(const detail::vector_base<T1, Alloc1>& lhs,
		const detail::vector_base<T2, Alloc2>& rhs);


	template<typename T1, typename Alloc1, typename T2, typename Alloc2>
	void operator==(const detail::vector_base<T1, Alloc1>& lhs,
		const std::vector<T2, Alloc2>& rhs);

	template<typename T1, typename Alloc1, typename T2, typename Alloc2>
	void operator==(const std::vector<T1, Alloc1>& lhs,
		const detail::vector_base<T2, Alloc2>& rhs);


	template<typename T1, typename Alloc1, typename T2, typename Alloc2>
	void operator!=(const detail::vector_base<T1, Alloc1>& lhs,
		const detail::vector_base<T2, Alloc2>& rhs);


	template<typename T1, typename Alloc1, typename T2, typename Alloc2>
	void operator!=(const detail::vector_base<T1, Alloc1>& lhs,
		const std::vector<T2, Alloc2>& rhs);

	template<typename T1, typename Alloc1, typename T2, typename Alloc2>
	void operator!=(const std::vector<T1, Alloc1>& lhs,
		const detail::vector_base<T2, Alloc2>& rhs);







}//thrust


#include <thrust_follow/detail/vector_base.inl>