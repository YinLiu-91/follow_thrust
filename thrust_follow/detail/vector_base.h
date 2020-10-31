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

			/*! ������һ���յ�vector_base
			*/


			vector_base(void);

			/*!�˴���һ���յ�vector_base
			* \param alloc �Ǵ�vector_baseʹ�õķ�����
			*/
			explicit vector_base(const Alloc& alloc);

			/*!�˹��캯������һ��Ĭ�Ϲ���Ԫ�ص�vector_base
			*  \param n ����Ԫ�ص�����
			*/
			explicit vector_base(size_type n);


			/*!��������Ԫ��������vector_base
			* \param n ������Ԫ������
			* \param alloc ��vector_baseʹ�õķ�����
			*/
			explicit vector_base(size_type n, const Alloc& alloc);

			/*!�˹��캯������һ������һ��ֵ��vector_base
			* \param n Ԫ������
			* \param value Ԫ�ص�ֵ
			*/
			explicit vector_base(size_type n, const value_type& value);

			/*!�˹��캯������һ������һ��ֵ����ָ����������vector_base
			* \param n Ԫ������
			* \param value Ԫ�ص�ֵ
			* \param alloc ָ���ķ�����
			*/

			explicit vector_base(size_type n, const value_type& value, const Alloc& alloc);


			/*!�������캯��������һ��vector_base
			* \param v ��������vector_base
			* \param
			*/

			vector_base(const vector_base& v);

			/*!�������캯��������һ��ָ��������vector_base
			* \param v ��������vector_base
			* \param alloc ָ���ķ�����
			*/
			vector_base(const vector_base& v, const Alloc& alloc);

#if THRUST_CPP_DIALECT >=2011
			/*!�ƶ����캯���ƶ���һ��vector_base
			* \param v :���ƶ���vector_base
			*/
			vector_base(vector_base&& v);
#endif

			/*!������ֵ�������������һ��vector_base ��ֵ���ұߵ�vector_base
			* \param v �Ǳ�������vector_base
			*/

			vector_base& operator=(const vector_base& v);

#if THRUST_CPP_DIALECT >=2011
			/*!�ƶ�������ֵ���������һ��vector_base �ƶ��������ұߵ�vector_base
			* \param �Ǳ�������vector_base
			*/

			vector_base& operator=(vector_base&& v);
#endif

			/*!��������һ����ͬ���͵�vector_base
			* \param v ��Ҫ��������vector_base
			*/

			template<typename OtherT, typename OtherAlloc>
			vector_base(const vector_base<OtherT, OtherAlloc>& v);


			template<typename OtherT, typename OtherAlloc>
			vector_base& operator=(const vector_base<OtherT, OtherAlloc>& v);

			/*!������ֵһ����ͬ���͵�vector_base
			* \param v ��Ҫ��������vector_base
			*/
			template<typename OtherT, typename OtherAlloc>
			vector_base& operator=(const std::vector<OtherT, OtherAlloc>& v);

			/*!��������һ����ͬ���͵�std::vector
			* \param v ��Ҫ��������vector
			*/

			template<typename OtherT, typename OtherAlloc>
			vector_base(const std::vector<OtherT, OtherAlloc>& v);


			/*!������ֵһ����ͬ���͵�std::vector
			* \param v ��Ҫ��������vector
			*/
			/*template<typename OtherT, typename OtherAlloc>
			vector_base& operator=(const std::vector<OtherT, OtherAlloc>& v);*/



			template<typename InputIterator>
			void range_init(InputIterator first, InputIterator last);


			/*!�˹��캯��Դ������һ����Χ
			* \param first �˷�Χ�Ŀ�ʼ
			* \param last �˷�Χ�Ľ���
			*/
			template<typename InputIterator>
			vector_base(InputIterator first, InputIterator last);


			/*!�˹��캯��Դ������һ�����������ķ�Χ
			* \param first �˷�Χ�Ŀ�ʼ
			* \param last �˷�Χ�Ľ���
			*/
			template<typename InputIterator>
			vector_base(InputIterator first, InputIterator last, const Alloc& alloc);


			/*!��������
			*
			*/
			~vector_base(void);


			/*!\brief ��vector_base resize��ȷ���������������Ĭ�Ϲ���
			* \param new_size ��vector_baseӦ�����е�����
			* \throw �������������� max_size9 �׳�std::length_error
			*/
			void resize(size_type new_size);

			/*!\brief ��vector_base resize��ȷ��������,����ĸ���ֵx
			* \param new_size ��vector_baseӦ�����е�����
			* \throw �������������� max_size9 �׳�std::length_error
			* \ x �µ�Ԫ��Ӧ������ֵ
			*/

			void resize(size_type new_size, const value_type& x);

			/*! ����vector_base����Ԫ�صĸ���
			* \

			*/
			size_type size(void)const;

			/*! ����vector_base�����е����Ԫ�ظ���
			*
			*/
			size_type max_size(void)const;

			/*! \brief ���nС��vector_base�Ĵ�С����˺���û��Ӱ�죬
			* ���򣬴˲������������ڴ棬����ڴ�����ɹ���������capacity()����n��
			* ������������仯���������κ�����£�size()�Ĵ�С������
			* \throw ���n������max_size() ��ô���׳�std::length_error
			*/

			void reserve(size_type n);


			/*! ����reserve()������
			*/
			size_type capacity(void)const;

			/*!ʹ��size()��capacity������һ��
			*
			*/
			void shrink_to_fit(void);

			/*!\brief �õ�vector_base��Ԫ�ص��±�
			* \param n Ԫ���±�
			* \return  д��/��ȡ������
			*/
			reference operator[](size_type n);

			/*!\brief �õ�vector_base��Ԫ�ص�const �±�
			* \param n Ԫ���±�
			* \return  ��ȡ������
			*/
			const_reference operator[](size_type n)const;


			/*!����ָ��vetor_base��ʼ��ĵ�����
			* \return mStart
			*/
			iterator begin(void);

			/*!����ָ��vetor_base��ʼ���const������
			* \return mStart
			*/
			const_iterator begin(void)const;

			/*!����ָ��vetor_base��ʼ���const������
			* \return mStart
			*/
			const_iterator cbegin(void)const;

			/*! ���ط��������
			* \return ָ��vector_base����ķ��������
			*/
			reverse_iterator rbegin(void);

			/*! ����const���������
			* \return ָ��vector_base����ķ��������
			*/
			const_reverse_iterator rbegin(void)const;

			/*! ����const���������
			* \return ָ��vector_base����ķ��������
			*/
			const_reverse_iterator crbegin(void)const;

			/*!����ָ��vector_base���һ��Ԫ�غ����һ��������
			* \return begin()+size()
			*/
			iterator end(void);

			/*!����ָ��vector_base���һ��Ԫ�غ����һ��const������
			* \return begin()+size()
			*/
			const_iterator end(void)const ;

			/*!����ָ��vector_base���һ��Ԫ�غ����һ��const������
			* \return begin()+size()
			*/
			const_iterator cend(void);


			/*!����ָ����vector_base�������һ��Ԫ�غ����һ��������
			* \return rbegin()+size()
			*/
			reverse_iterator rend(void);

			/*!����ָ����vector_base�������һ��Ԫ�غ����һ��const������
			* \return begin()+size()
			*/
			const_reverse_iterator rend(void)const ;

			/*!����ָ����vector_base�������һ��Ԫ�غ����һ��const������
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