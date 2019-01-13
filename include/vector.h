/*!produced by Lucas Lima Marques de Oliveira.
*/
#ifndef VECTOR_H
#define VECTOR_H

#include <initializer_list>

#include <iterator>

using size_type = unsigned long;

namespace sc{

	template <typename T>
	class vector
	{
	public:

		class iterator 
		{
		public:

            typedef T value_type;
            typedef T& reference;
            typedef T* pointer;
			
			typedef std::forward_iterator_tag iterator_category;
            typedef int difference_type;

			iterator(T* ptr):current(ptr){/*empty*/}
			iterator(){current =new T;}
			reference operator*(){return *current;}
			iterator & operator++(){++current;return *this;} //++it;
			iterator operator++(int){iterator temp(current); ++ (*this);return temp;} //it++;
			iterator & operator--(){--current;return *this;} //--it;
			iterator operator--(int){iterator temp(current); -- (*this);return temp;} //it--;
			iterator operator+(int value){return iterator(current + value);} //it + value;
			iterator operator-(int value){return iterator(current - value);} //it - value;
			pointer operator->(){return current;} //it -> it;
			iterator* operator=(const iterator &rhs){this->current = rhs.current;} //it = it;
			bool operator==(const iterator & rhs)const{
				return (current == rhs.current && rhs.current == current);// it == it;
			}
			bool operator!=(const iterator & rhs)const{// it != it;
				return !(current == rhs.current && rhs.current == current);
			}
		private:
			pointer current;
		};

		class const_iterator
		{
		public:

			typedef T value_type;
            typedef const T& const_reference;
            typedef T* pointer;
			
			typedef std::forward_iterator_tag iterator_category;
            typedef int difference_type;

			const_iterator(T* ptr):current(ptr){/*empty*/}
			const_reference operator*()const{return *current;}
			const_iterator & operator++(){++current;return *this;} //++it;
			const_iterator operator++(int){const_iterator temp(current); ++ (*this);return temp;} //it++;
			const_iterator & operator--(){--current;return *this;} //--it;
			const_iterator operator--(int){const_iterator temp(current); -- (*this);return temp;} //it--;
			const pointer operator->(){return current;} //it -> it;
			const_iterator operator+(int value){return const_iterator(current + value);}// it + value;
			const_iterator operator-(int value){return const_iterator(current - value);} //it - value;

			const_iterator* operator=(const const_iterator &rhs){this->current=rhs.current;}//it = it;
			bool operator==(const const_iterator & rhs)const{ //it == it;
				return (current == rhs.current && rhs.current == current);
			}
			bool operator!=(const const_iterator & rhs)const{ //it != it;
				return !(current == rhs.current && rhs.current == current);
			}
		private:
			T *current;
		};

		const static size_type DEFAULT_SIZE=0;
		using const_reference = const T&;
		using reference = T&;
		using pointer = const T*;

		//[I] SPECIAL MEMBERS

		/*! Default constructor that creates an empty list
	 	*/
		vector():m_capacity(0),m_storage(new T[m_capacity]),m_end(0){/*empty*/}
		
		/*! Default destructor
	 	*/
		~vector(){
			delete[] m_storage;
		}
		/*! Copy constructor, makes a *deep copy* of another Vector object
			@param 	const Vector &other : Another Vector object
		*/
		vector(size_type value){
			m_capacity = value;
			m_storage= new T[m_capacity];
			m_end=0;
		}

		/*! Constructor that takes a std::initializer_list as arg.
				@param 	std::initializer_list ilist : Initializer list that will turn into a Vector
		*/
		vector(std::initializer_list<T> ilist){
			m_capacity = ilist.size();
			m_storage = new T[m_capacity];
			m_end = ilist.size();

			std::copy(ilist.begin(),ilist.end(),begin());
		}

		/*! Copy constructor, makes a 'deep copy' of another Vector object
			@param 	const Vector &other : Another Vector object
		*/
		vector(const vector &other){
			m_capacity=other.m_capacity;
			m_storage=new T[m_capacity];
			m_end=other.m_end;

			for(auto i(0u);i<m_end;i++){
				m_storage[i]=other.m_storage[i];
			}
		}

		/*! Move operator. Replaces the contents with a copy of the contents of other.
		* (i.e. the data in other is moved from other into this container).
		*		@param vector &&other : Another vector object
		*/
		vector( vector &&other){
			m_capacity=other.m_capacity;
			m_storage=new T[m_capacity];
			m_end=other.m_end;

			for(auto i(0u);i<m_end;i++){
				m_storage[i]=other.m_storage[i];
			}

			other.m_capacity=0;
			other.m_storage=nullptr;
			other.m_end=0;
		}

		/*! Constructor that makes a copy from another simple array.
		 * 		@param 	InputIt first : First element from the array
		 *		@param 	InputIt last : Last (stub) element from the array
		*/
		template<typename InputItr>
		vector(InputItr first,InputItr last){
			int cont(0);
			for (auto i(first);i!= last;i++){cont++;}
			//m_capacity = std::distance(first,last);
			m_capacity=cont;

			m_storage=new T[m_capacity];
			m_end=0;
			for(auto i(first);i!=last;i++){
				m_storage[m_end]=*i;
				m_end++;
			}
		}
		/*! sc::Vector operator `=` overload function
		 * 		@param	const Vector &rhs : Right hand side object
		 * 		@return	The right hand side object
		*/
		vector& operator = (const vector &other){
			m_capacity=other.m_capacity;
			m_storage=new T[m_capacity];
			m_end=other.m_end;

			for(auto i(0u);i<m_end;i++){
				m_storage[i]=other.m_storage[i];
			}
			return *this;
		}

		/*! sc::Vector operator `=` overload function
		 * 		@param Vector &&rhs : Right hand side object
		 * 		@return	The right hand side object
		*/
		vector& operator = ( vector &&other){
			m_capacity=other.m_capacity;
			m_storage=new T[m_capacity];
			m_end=other.m_end;

			for(auto i(0u);i<m_end;i++){
				m_storage[i]=other.m_storage[i];
			}

			other.m_capacity=0;
			other.m_storage=nullptr;
			other.m_end=0;

			return *this;
		}

		//[II] ITERATORS

		/*! Begin iterator
		 * 		@return Iterator to the first element of the vector.
		*/
		iterator begin(void){
			return iterator(m_storage);
		}

		/*! End iterator
		 * 		@return Iterator to the last element of the vector.
		*/
		iterator end(void){
			return iterator(m_storage+m_end);
		}

		/*! Constant begin iterator
		 * 		@return A constant iterator to the first element of the vector.
		*/
		const_iterator cbegin(void) const{
			return const_iterator(m_storage);
		}

		/*! Constant end iterator
		 * 		@return A constant iterator to the last element of the vector.
		*/
		const_iterator cend(void)const{
			return const_iterator(m_storage+m_end);
		}

		//[III] CAPACITY

		/*! Discover how many elements the sc::Vector has
	 	* 		@return	size_type size : Size of elements in the sc::Vector
		*/
		size_type size(void)const{
			return m_end;
		}

		/*! Discover what is the total capacity of the sc::Vector	
	 	*		 @return size_type capacity : maximum amount of elements for current allocated size
	 	*/
		size_type capacity(void)const{
			return m_capacity;
		}

		/*! Tells if a sc::Vector is empty (0 elements)
		 *		@return	bool result : True if the element is empty, false otherwise	
		 */
		bool empty(void)const{
			return m_end==0;
		}

		//[IV] MODIFIERS

		/*! Reset current sc::Vector object
	 	*/
		void clear(void){
			m_end = 0;
		}

		/*! Insert an element in the first position in the sc::Vector
	 	* 		@param	const T& value : Value to be inserted on the list
		*/
		void push_front(const_reference value){
			if(m_capacity == 0){
				reserve(8);
			}else if(m_end + 1 == m_capacity){
				reserve(2*m_capacity);
			}
			
			std::unique_ptr<T[]> newv(new T[m_capacity]);
			int cont(0);

			newv[cont]=value;
			for(auto i(begin());i != end();i++){
				cont+=1;
				newv[cont]=*i;
			}

			std::unique_ptr<T[]> oldD(m_storage);
			m_storage=newv.release();

			m_end++;
		}

		/*! Insert an element in the end of the sc::Vector
		 * 		@param 	const T& value : Value to be inserted on the list
		 */
		void push_back(const_reference value){
			if(m_capacity == 0){
				reserve(8);
			}else if(m_end == m_capacity){
				reserve(2 * m_capacity);
			}
			m_storage[m_end]=value;
			m_end++;
		}

		/*! Remove an element from the last position on the sc::Vector
		*/
		void pop_back(void){
			if(m_end>0){
				m_end--;
			}
		}

		/*! Remove an element from the first position on the sc::Vector
		 */
		void pop_front(void){
			if(m_end>0){
				std::unique_ptr<T[]> newv(new T[m_capacity]);
				std::copy(++begin(),end(),newv.get());
				std::unique_ptr<T[]> oldD(m_storage);
				m_storage=newv.release();
				m_end--;
			}
		}

		/*! Insert an defined number of terms on the sc::Vector object
	 	* 		@param 	iterator pos : Position to start inserting elements
	 	* 		@param 	const T& value : Value to be inserted
	 	*/
		iterator insert(iterator pos,const_reference val){
			
			int n = std::distance(begin(),pos);
			if(m_end == m_capacity || pos == end()){
				reserve(2 * m_capacity);
			}
			m_end++;
			iterator temp(end()-1);

			for(auto i(end());i != pos;i--){
				*i=*temp;
				temp --;
			}
			
			m_storage[n]=val;

			return pos;
		}

		/*! Insert an defined number of terms on the sc::Vector object
		 * 		@param 	iterator pos : Position to start inserting elements
		 * 		@param 	iterator first : First iterator that will be inserted
		 * 		@param 	iterator last : Last iterator that will stop the insertion
		*/
		template<typename InputItr>
		iterator insert(iterator pos,InputItr first,InputItr last){

			int disPos=std::distance(begin(),pos);

			if(disPos>size()){
				return pos;
			}
			int n=std::distance(first,last);
			if(m_end==m_capacity || pos==end()){
				reserve(2*(m_capacity+n));
			}
			m_end+=n;
			iterator temp(begin()+disPos);
			for(auto i(begin()+(disPos+n));i!=end();i++){
				*i=*temp;
				temp++;
			}
			temp =(begin()+disPos);
			while(first != last){
				*temp=*first;
				temp++;
				first++;
			}
			return begin() + disPos;
		}

		/*! Insert an defined number of terms on the sc::Vector object
		 * 		@param 	iterator pos : Position to start inserting elements
		 * 		@param 	std::initializer_list<T> ilist : initializer_list object that will be inserted on the sc::Vector
		*/
		iterator insert(iterator pos,std::initializer_list<T> ilist){return (insert(pos,ilist.begin(),ilist.end()));}

		/*! Allocates memory if new_cap > capacity
	 	* 		@param new_cap expected size of the array after function call
	 	*/
		void reserve(size_type new_cap){
			if(new_cap>m_capacity){
				std::unique_ptr<T[]> newv(new T[new_cap]);

				std::copy(begin(),end(),newv.get());
				std::unique_ptr<T[]> oldD(m_storage);
				
				m_storage = newv.release();
				m_capacity = new_cap;
			}
		}

		/*! Resizes if needed the total capacity of a sc::Vector object
		*/
		void shrink_to_fit(void){
			std::unique_ptr<T[]> newv(new T[m_end]);

			std::copy(begin(),end(),newv.get());
			std::unique_ptr<T[]> oldD(m_storage);
				
			m_storage = newv.release();
			m_capacity = m_end;
		}

		/*! Replaces the contents with count copies of pre-defined values
	 	* 		@param 	size_type count : How many times the const T &value will appear
	 	* 		@param 	const T &value : Value that will be inserted
	 	*/
		void assign(size_type count, const_reference value){
			std::unique_ptr<T[]> newv(new T[count]);
			
			m_storage = newv.release();
			m_capacity = count;	

			for(int i(0);i<count;i++){
				m_storage[i]=value;
				}
			m_end=count;
		}  

		/*! Replaces the contents with count copies of pre-defined values
		 * @param 	std::initializer_list<T> ilist : initializer_list that will populate the sc::Vector object
		*/
		void assign(std::initializer_list<T> ilist){
			iterator it = insert(begin(),ilist.begin(),ilist.end());
		}

		/*! Replaces the contents with count copies of pre-defined values
		 * 		@param 	iterator first : First iterator element
		 * 		@param 	iterator last : Last iterator element
		*/
		template<typename InputItr>
		void assign(InputItr first,InputItr last){
			insert(begin(),first,last);
		}

		/*! Removes from the sc::Vector either a element or a range of elements (first, last)
		 * 		@param	iterator first : First position iterator
		 * 		@param	iterator last : Last position iterator
		*/
		iterator erase(iterator first,iterator last){
			int cont(0);
			iterator temp(first);
			for(auto i(first);i!=last;i++){cont++;}
			for(auto i(last);i!=end();i++){
				*first=*i;
				first++;
			}
			
			m_end-=cont;

			return temp;
		}

		/*! Removes from the sc::Vector either a element or a range of elements (first, last)
		 * 		@param	iterator pos : The position of the element to be erased	
		*/
		iterator erase(iterator pos){

			iterator temp(pos);
			++temp;

			for(iterator i(temp);i != end();i++){
				*pos=*i;
				pos++;
			}

			m_end--;
			return --temp;
		}

		//[V] ELEMENT ACCESS

		/*! Access the last element of the vector.
		 * 		@return The value of the last element of the vector (vector[size-1]).
		*/
		const_reference back(void)const{return m_storage[m_end-1];}

		/*! Access the first element of the vector.
		 * 		@return The value of the first element of the vector (vector[0]).
		*/
		const_reference front(void)const{return m_storage[0];}

		/*! sc::Vector operator `[]` overload function
		 * 		@param 	size_type pos : The desired position to access in elements[pos].
		 * 		@return The element at elements[pos].
		*/
		const_reference operator[](size_type pos)const{return m_storage[pos];}
		reference operator[](size_type pos){return m_storage[pos];}
		const_reference at(size_type pos)const{
			if(pos<0 || pos>=m_end){
				throw std::out_of_range{"Vector element out of range"};
			}

			return m_storage[pos];

		}

		/*! Access element from a specified position
		 * 		@param size_type pos : User defined position
		 * 		@return Element at defined position
		*/
		reference at(size_type pos){
			if(pos<0 || pos>=m_end){
				throw std::out_of_range{"Vector element out of range"};
			}

			return m_storage[pos];

		}

		/*! Peek inside the class during the tests.
		 *		@return return a raw pointer to the underlying array.*/
		pointer data(void){ return m_storage;}

		/*! Peek inside the class during the tests.
		 *		@return return a raw pointer const to the underlying array.*/
		const_reference data(void)const{return m_storage;}

		//[VI] OPERATORS

		/*! sc::Vector operator `==` overload function
		 * 		@param	const Vector &rhs : Right hand side object
		 * 		@return	bool value if the two objects are equal.True if the element is equal, false otherwis
		*/
		bool operator == (const vector& rhs)const{
			if(size() != rhs.size()){
				return false;
			}else{
				for(int i(0);i<rhs.size();i++){
					if(m_storage[i]!=rhs[i]){
						return false;
					}
				}
			}
			return true;
		}

		/*! sc::Vector operator `!=` overload function
		 * 		@param	const Vector &rhs : Right hand side object
		 * 		@return	bool value if the two objects are unequal. True if the element is equal, false otherwise
		*/
		bool operator != (const vector& rhs)const{
			if(size() != rhs.size()){
				return true;
			}else{
				for(int i(0);i<rhs.size();i++){
					if(m_storage[i]!=rhs[i]){
						return true;
					}
				}
			}
			return false;
		}

		//[VII] FRIEND FUNCTIONS
		friend std::ostream& operator<<(std::ostream &os_,const vector<T> &v_);
		friend void swap(vector <T> &first_,vector<T> &second_);

	private:
		size_type m_end; //<! Current list size (or index past-last valid element)
		size_type m_capacity; //<! List's storage capacity.
		T *m_storage; //<! Data storage area for the dynamic array.
	};
}

#endif