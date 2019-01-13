/*!produced by Lucas Lima Marques de Oliveira.
*/
#ifndef STACK_H_
#define STACK_H_

#include <iostream>

namespace sc{
	template <typename T>
	class stack
	{
	private:

		T* storage;
		size_t m_size;
		size_t m_top;

		void double_storage(void){
			T* temp = new T[2*m_size];

			for(auto i(0u); i < m_top;i++){
				temp [i] = storage[i];
			}

			delete[] storage;

			storage = temp;

			m_size *= 2;
		}

	public:
		explicit stack(void):
		 storage(new T[1]),
		 m_size(1),m_top(0)
		 {/*empty*/}
		~stack(void){delete[] storage;}

		void push(T& value){
			if(m_size == m_top){
				double_storage();
			}

			storage[m_top++] = value;
		}
		
		void pop(void){
			if(empty()){
				//throw std::runtime_error(" pop");
			}

			--m_top;
		}

		T top(void)const{
			if(empty()){
				std::runtime_error("error top");
			}

			return storage[m_top-1];
		}

		bool empty(void)const{ return m_top == 0; }

		size_t size(void)const{ return m_top; }

		void clear(void){ m_top = 0; }
	};
}

#endif