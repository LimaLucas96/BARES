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

#include "../include/Token.h"
#include <iterator>
#include <algorithm>

/* <! verifica se a entrada no formato char é valida.
		@param c_ variavel char com a entrada a ser lida.
		@return o codigo referente a variavel c_ caso seja valida.
*/
parser::terminal_symbol_t parser::lexer(char c_) const{
	switch(c_){
		
		case '+': return terminal_symbol_t::TS_PLUS;
		case '-': return terminal_symbol_t::TS_MINUS;
		case '*': return terminal_symbol_t::TS_MULTI;
		case '/': return terminal_symbol_t::TS_DIV;
		case '%': return terminal_symbol_t::TS_MOD;
		case '^': return terminal_symbol_t::TS_PORC;
		case ' ': return terminal_symbol_t::TS_WS;
		case   9: return terminal_symbol_t::TS_TAB;
		case '0': return terminal_symbol_t::TS_ZERO;
		case '1': 
		case '2': 
		case '3': 
		case '4': 
		case '5': 
		case '6': 
		case '7': 
		case '8': 
		case '9': return terminal_symbol_t::TS_NON_ZERO_DIGIT;
		case '(': return terminal_symbol_t::TS_KEY_OPEN;
		case ')': return terminal_symbol_t::TS_KEY_CLOSE;
		case '\0': return terminal_symbol_t::TS_EOS;
	}
	return terminal_symbol_t::TS_INVALID;
}
/* <! Avança uma posição o ponteiro para do arquivo de entrada.
*/
void parser::next_symbol(void){
	std::advance(m_it,1);
}
/* <! Verifica se a linha de entrada ja chegou ao fim.
*		@return true se o ponteiro estiver no fim do arquivo. false caso contrario.
*/
bool parser::end_input(void) const{
	return m_it == m_expr.end();
}

/* <! Verifica se o terminal_symbol_t de entrada é gual ao simbolo atual do ponteiro.
*		@param c_ terminal_simbol_t que quer comparar.
*		@return True se a entrada for igual ao indice do ponteiro. False caso contrario.
*/
bool parser::accept( terminal_symbol_t c_){
	if(!end_input() && lexer(*m_it) == c_){
		next_symbol();
		return true;
	}

	return false;
}

/* <! Pula os espaços e Tabs que estiver no arquivo de entrada.
*/
void parser::skip_ws(void){
	while(! end_input() && (std::isspace(*m_it))){
		next_symbol();
	}
}
/* <! Tenta consumir uma expressão valida da entrada.
*		@return true se a expressao estiver toda correta. False caso encontre algum erro na expressão.
*
* A regra de producao será:
*
*	<expr> := <term>,{("+"|"-"|"*"|"/"|"%"|"^"),<term>};
*
*/
bool parser::expression(){
	term();

	while (m_result.type == ResultType::OK){
		skip_ws();

		if(accept(parser::terminal_symbol_t::TS_MINUS)){
			m_tk_list.push_back( token("-",1));
		}else if(accept(parser::terminal_symbol_t::TS_PLUS)){
			m_tk_list.push_back( token("+",1));
		}
		else if(accept(parser::terminal_symbol_t::TS_MULTI)){
			m_tk_list.push_back( token("*",2));
		}
		else if(accept(parser::terminal_symbol_t::TS_DIV)){
			m_tk_list.push_back( token("/",2));
		}
		else if(accept(parser::terminal_symbol_t::TS_MOD)){
			m_tk_list.push_back( token("%",2));
		}
		else if(accept(parser::terminal_symbol_t::TS_PORC)){
			m_tk_list.push_back( token("^",3));
		}

		else break;

		if(!term() && end_input() && m_result.type != ResultType::MISSING_CLOSE){
			m_result.type = ResultType::MISSING_TERM;
		}
	}

	return m_result.type == ResultType::OK;
}

/* <!Tenta consumir um termo valido da entrada.
 * Esta função verifica se o termo é um parenteses ou um inteiro.
 * 		@return true se o tiver tiver encontrado um termo valido. False caso contrario.
 *
 * A regra de produção será:
 *
 *<term> := "(",<expr>,")" | <integer>;
 *
 * */
bool parser::term(void){

	skip_ws();

	auto begin_token( m_it);

	if(accept(terminal_symbol_t::TS_KEY_OPEN)){
		m_tk_list.push_back(token("(",-1));
		skip_ws();
		
		if(end_input()){
			m_result = ResultType(ResultType::UNEXPECTED_END_OF_EXPRESSION,
				std::distance(m_expr.begin(),m_it)+1);
			return true;
		}

		if(expression()){
			if(accept(terminal_symbol_t::TS_KEY_CLOSE)){
			
				m_tk_list.push_back(token(")",-2));

			}else{
				m_result = ResultType(ResultType::MISSING_CLOSE,
					std::distance(m_expr.begin(),m_it)+1);
				return true;
			}
		}else{

			return false;
		}
	}
	else if(integer()){

		std::string token_str;
		std::copy(begin_token, m_it, std::back_inserter( token_str));

		if(token_str[0] == '-'){
			std::string temp("");
			size_t i(0);
			while(token_str[i] == '-'){ i++; }

			if(i%2){
				temp+="-";
			}

			temp += std::string(token_str.begin()+i,token_str.end());
			token_str = temp;
		}
		input_int_type token_int;
		try{token_int = stoll(token_str); }
		catch(const std::invalid_argument &e ){
			
			m_result = ResultType(ResultType::ILL_FORMED_INTEGER, std::distance(m_expr.begin(),m_it) +1);
			//return false;
		}

		//std::cout << token_int << "\n";
		if(token_int < std::numeric_limits <required_int_type >::min() ||
			token_int > std::numeric_limits <required_int_type>::max()){
			//std::cout << "test";
			m_result = ResultType(ResultType::INTEGER_OUT_OF_RANGE, std::distance(m_expr.begin(), begin_token)+1);
			return true;
		}else{
			m_tk_list.push_back(token(token_str, 0 ));
		}
	}
	else{
		//std::cout << "con \n";
		m_result = ResultType(ResultType::ILL_FORMED_INTEGER,
		 std::distance(m_expr.begin(),begin_token)+1);
	}

	return m_result.type == ResultType::OK;
}

/* <! Tenta consumir um inteiro valido da entrada.
* Verifica se o numero é composto por ZERO , se é um numero natural ou num numero negativo.
* 		@return True caso o numero seja valido. E false caso contrario.
* 
* A regra de produção será:
*
* <integer> := 0 | ["-"], <natural_number>;
*
*/
bool parser::integer(){
	if(accept(terminal_symbol_t::TS_ZERO)){
		return true;
	}
	else {
	//	while(accept(terminal_symbol_t::TS_MINUS))  /*empty*/   ;

		if(accept(terminal_symbol_t::TS_MINUS)){
			while(accept(terminal_symbol_t::TS_MINUS))  ;
		}
	}

	
	//accept(terminal_symbol_t::TS_MINUS)
	return natural_number();
}

/* <! Tenta consumir um numero natural valido da entrada.
* Verifica se o indice é um numero diferente de zero.
*		@return True se é um numero natural diferente de zero. False caso contrario.
*
* A regra de produção será:
*
* <natural_number> := <digit_excl_zero>,{<digit>};
*
*/
bool parser::natural_number(){
	if(not digit_excl_zero()){
		return false;
	}

	while(digit())/*empty*/;

	return true;
}
/* <! Tenta consumir um digito diferente de zero na entrada.
* Verifica se é um numero é diferente de zero.
* 		@return true caso seja um numero natural diferente de zero. False caso contrario.
*
* A regra da produção será:
*
* <digit_excl_zero> := "1"|"2"|"3"|"4"|"5"|"6"|"7"|"8"|"9";
*
*/
bool parser::digit_excl_zero(){
	return accept(terminal_symbol_t::TS_NON_ZERO_DIGIT);
}

/* <! Tenta consumir um digito da entrada.
* Verifica se o digito é zero ou um numero diferente de zero.
* 		@return true caso seja um numero valido. False caso contrario.
*
* A regra de prodição será:
*
* <digit> := "0" | <digit_excl_zero>;
*
*/
bool parser::digit(){
	return (accept(terminal_symbol_t::TS_ZERO) or digit_excl_zero()) ? true : false;
}


/* <! Le a entrada e preenche o vetor de tokens.
*		@param e_ recebe uma string com a entrada.
*		@return retorna o ResultType com a mensagem de erro, caso haja.
*/
parser::ResultType parser::parse( std::string e_){
	m_expr = e_;
	m_it = m_expr.begin();
	m_result = ResultType(ResultType::OK);

	m_tk_list.clear();
	postfix_e.clear();

	skip_ws();
	if(end_input()){
		m_result = ResultType(ResultType::UNEXPECTED_END_OF_EXPRESSION, std::distance(m_expr.begin(),m_it)+1);
	}else{
		if(expression()){
			skip_ws();
			if(!end_input()){
				m_result = ResultType(ResultType::EXTRANEOUS_SYMBOL,
					std::distance(m_expr.begin(),m_it)+1);
			}
		}
	}

	return m_result;
}


/* <! Verifica qual dos dois tokens tem maior prioridade.
*		@param op1 token com uma operação.
*		@param op2 token com uma operação.
*		@return true se a prioridade do op1 é maior que a do op2. False caso contrario.
*/
bool parser::has_higher_or_eq_precedence(token op1,token op2){
	int p_op1 = op1.m_priority;
	int p_op2 = op2.m_priority;

	if(p_op1 > p_op2){
		return true;
	}else if(p_op1 < p_op2){
		return false;
	}
	else {
		if(!is_right_association(op2.m_value)) {return true;}
	}

	//return false;
}

/* <!Verifica se o token é um parentese abrindo.
*		@param i token de entrada que contem a string.
*		@return true caso seja um parentese abrindo "(". False caso contrario.
*/
bool parser::isOpen(token i){ return i.m_priority == -1; }

/* <!Verifica se o token é um parentese "(" fechando.
		@param i token de entrada que contem a string.
		@return true caso seja um parentese fechando. False caso contrario.
*/
bool parser::isClose(token i){ return i.m_priority == -2; }

/* <!Verifica se a string de entrada é o operador de potencia "^".
		@param op string com o sinal de operador.
		@return true caso o op seja o sinal de potencia. False caso contrario.
*/
bool parser::is_right_association(std::string op){ return op == "^"; }

/* <! executa as operações matematicas.
		@param v1 numero de entrada.
		@param v2 numero de entrada.
		@param op token com o operador.
		@return Retorna o valor da operação entre v1 e v2 se tudo estiver ok.
*/
parser::input_int_type parser::execute_operator(input_int_type v1, input_int_type v2, token op){
	
	switch(op.m_value[0]){
		case '^': 
			if(pow(v1,v2) > std::numeric_limits <required_int_type >::max() or 
				pow(v1,v2) < std::numeric_limits <required_int_type >::min())
			{	
				//std::cout << "num1 -> "<< v1 << "num2 -> " << v2 << "result - >" <<pow( v1,v2) << "\n" ;
				m_result.type = ResultType::OVERFLOW_ERROR;
				 //std::cout << "Numeric overflow error!\n";	
			}else
		 		return pow( v1,v2 );

        case '*':
        	if(v1*v2 > std::numeric_limits <required_int_type >::max() or 
				v1*v2 < std::numeric_limits <required_int_type >::min())
			{	
				m_result.type = ResultType::OVERFLOW_ERROR;
				// std::cout << "Numeric overflow error!\n";	
			}else
          		return v1*v2;

        case '/':  
        	if ( v2 == 0 ){
        		m_result.type = ResultType::DIV_ZERO;
				// std::cout << "Division by zero!\n";
            }else if(v1/v2 > std::numeric_limits <required_int_type >::max() or 
				v1/v2 < std::numeric_limits <required_int_type >::min())
			{
				m_result.type = ResultType::OVERFLOW_ERROR;
				 //std::cout << "Numeric overflow error!\n" ;	
			}else
            	return v1/v2;

        case '%': 
        	if ( v2 == 0 ) {
        		m_result.type = ResultType::DIV_ZERO;
				// std::cout <<"Division by zero!\n" ;
            }
        	else if(v1%v2 > std::numeric_limits <required_int_type >::max() or 
				v1%v2 < std::numeric_limits <required_int_type >::min())
			{
				m_result.type = ResultType::OVERFLOW_ERROR;
				// std::cout << "Numeric overflow error!\n" ;	
			}else 
            	return v1%v2;

        case '+': 
        	if(v1+v2 > std::numeric_limits <required_int_type >::max() or 
				v1+v2 < std::numeric_limits <required_int_type >::min())
			{
				m_result.type = ResultType::OVERFLOW_ERROR;
				// std::cout << "Numeric overflow error!\n" ;	
			}else
        		return v1+v2;

        case '-': 
        	if(v1-v2 > std::numeric_limits <required_int_type >::max() or 
				v1-v2 < std::numeric_limits <required_int_type >::min())
			{
				m_result.type = ResultType::OVERFLOW_ERROR;
				 //std::cout <<"Numeric overflow error!\n";	
			}else
        		return v1-v2;

        default:  std::cout<< "undefined operator\n";

	}
}

/* <! Transforma a expressão do formato infixo para o formato posfixo.
*/
void parser::InfixToPostfix(void){
	sc::stack<token> s;

	for(auto &i : this -> m_tk_list){
		
		if(i.m_priority == 0){
			postfix_e.push_back(i);
		}
		else if(i.m_priority > 0 ){
			while (not s.empty() && 
				has_higher_or_eq_precedence(s.top(),i))
			{	
				postfix_e.push_back(s.top());
				s.pop();
			}
			s.push(i);
		}

		else if(isOpen(i)){
			s.push(i);
		}else if(isClose(i)){
		
			while(not isOpen(s.top())){
				postfix_e.push_back(s.top());
				s.pop();
			}
			s.pop();
		}
	}

	while(not s.empty()){
		postfix_e.push_back(s.top());
		s.pop();
	}

	//return true;
}

/* <! transforma uma string em um inteiro.
		@param c Token que contem um numero a ser transformado.
		@return o numero no formato input_int_type.
*/
parser::input_int_type& parser::char2integer(token c){
	std::stringstream ss(c.m_value);
	input_int_type vl;
	ss >> vl;

	return vl;
}

/* <! Executa a expressao no formato posfixo.
*/
void parser::solve(void){

	sc::stack<input_int_type> s;

	for(auto c : postfix_e){
		if(c.m_priority == 0){
			s.push(char2integer(c));
		}else if(c.m_priority > 0){
			input_int_type op2 = s.top(); s.pop();
			input_int_type op1 = s.top(); s.pop();

			auto result = execute_operator( op1, op2, c);
			s.push(result);
		}
		else assert(false);
	}

	v_result = s.top();
}












