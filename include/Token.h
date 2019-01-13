/*
****************************************************************
*
*  Linguagem de Programação Basica I ● DIM0120
*			- projeto Bares -
*
*  Projeto porduzido por Lucas Lima Marques de Oliveira.
*
****************************************************************
*/

#ifndef TOKEN_H_
#define TOKEN_H_

#include <iostream> // cout, cin
#include <iterator> // std::distance()
#include <vector>   // std::vector
#include <sstream>  // std::istringstream
#include <cstddef>  // std::ptrdiff_t
#include <limits>   // std::numeric_limits, para validar a faixa de um inteiro.
#include <algorithm>// std::copy, para copiar substrings.
#include <cctype>   // std::isspace()
#include <stdexcept> // std::runtime_error
#include <cmath>

#include "vector.h"
#include "stack.h"

class parser{

	public:

		struct ResultType
		{
			typedef std::ptrdiff_t size_type;

			enum code_t{
				OK = 0,
				MISSING_CLOSE,
				UNEXPECTED_END_OF_EXPRESSION,
               	ILL_FORMED_INTEGER,
                MISSING_TERM,
                EXTRANEOUS_SYMBOL,
                INTEGER_OUT_OF_RANGE,
                DIV_ZERO,
                OVERFLOW_ERROR
			};

			code_t type;
			size_type at_col;

			explicit ResultType(code_t type_ = OK, size_type col_ = 0u):
			type{type_},
			at_col{col_}
			{/*empty*/}
		};

		typedef short int required_int_type; //!< The interger type we accept as valid for an expression.
        typedef long long int input_int_type;

        ResultType parse(std::string);

        parser() = default;
		~parser() = default;
		parser(const parser &) = delete;

		parser& operator=(const parser&) = delete;

		void InfixToPostfix();

		void solve(void);

		ResultType getResultType(void){ return m_result; }

		input_int_type getResult(void){ return v_result; }

	private:

		class token{
		
		public:
			std::string m_value;
			int m_priority;

			explicit token( std::string value_="", int priority_ = 0 )
            : m_value( value_ )
            , m_priority( priority_ )
        	{/* empty */}

			token & operator=(const token &rhs){
				this->m_value = rhs.m_value;
				this->m_priority = rhs.m_priority;
				return *this;
			}
		};

		enum class terminal_symbol_t{
			TS_KEY_OPEN,        //<! code for "("
			TS_KEY_CLOSE,       //<! code for ")"
			TS_PLUS,	        //!< code for "+"
            TS_MINUS,	        //!< code for "-"
            TS_MULTI,           //<! code for "*"
            TS_DIV,             //<! code for "/"
            TS_MOD,             //<! code for "%"
            TS_PORC,            //<! code for "^"
            TS_ZERO,            //!< code for "0"
            TS_NON_ZERO_DIGIT,  //!< code for digits, from "1" to "9"
            TS_WS,              //!< code for a white-space
            TS_TAB,             //!< code for tab
            TS_EOS,             //!< code for "End Of String"
            TS_INVALID	 		//!< invalid token
		};

		std::string m_expr;
		std::string::iterator m_it;
		sc::vector< token > m_tk_list;
		sc::vector< token > postfix_e;
		ResultType m_result;
		input_int_type v_result;

		terminal_symbol_t lexer(char) const;
		void next_symbol(void);
		bool accept(terminal_symbol_t);
		void skip_ws(void);
		bool end_input(void) const;

		bool expression();
		bool term();
		bool integer();
		bool natural_number();
		bool digit_excl_zero();
		bool digit();

		bool has_higher_or_eq_precedence(token,token);
		bool isOpen(token);
		bool isClose(token);
		bool is_right_association(std::string);

		input_int_type execute_operator(input_int_type, input_int_type, token);
		
		input_int_type& char2integer(token);
};
#endif