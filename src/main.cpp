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

#include <iostream>

#include "../include/Token.h"

/* <! Imprime a mensagem de erro.
		@param &result Recebe o tipo de erro.
 */
void print_error_msg(const parser::ResultType & result){
	switch(result.type){
		case parser::ResultType::MISSING_CLOSE:
			std::cout << "Missing closing \")\" at column ("<< result.at_col <<")!\n";
		break;
		case parser::ResultType::UNEXPECTED_END_OF_EXPRESSION:
			std::cout << "Unexpected end of expression at column ("<< result.at_col <<")!\n";
		break;
		case parser::ResultType::ILL_FORMED_INTEGER:
			std::cout << "Ill formed integer at column ("<< result.at_col <<")!\n";
		break;
		case parser::ResultType::MISSING_TERM:
			std::cout << "Missing <term> at column ("<< result.at_col <<")!\n";
		break;
		case parser::ResultType::EXTRANEOUS_SYMBOL:
			std::cout << "Extraneous symbol after valid expression found at column ("<< result.at_col <<")!\n";
		break;
		case parser::ResultType::INTEGER_OUT_OF_RANGE:
			std::cout << "Integer constant out of range beginning at column (" << result.at_col << ")!\n";
		break;
		case parser::ResultType::DIV_ZERO:
			std::cout << "Division by zero!\n";
		break;
		case parser::ResultType::OVERFLOW_ERROR:
			std::cout << "Numeric overflow error!\n";
		break;
	}
}

int main(int argc, char const **argv)
{
	std::string input;
	parser myParser;
	while (std::getline(std::cin,input)){

		auto result = myParser.parse( input );

		if(result.type != parser::ResultType::OK){
			print_error_msg(result);
		}
		else if(result.type == parser::ResultType::OK){
			myParser.InfixToPostfix();
			myParser.solve();
			auto Resp = myParser.getResultType();

			if(Resp.type == parser::ResultType::OK){
				std::cout << myParser.getResult() << "\n";
			}else{
				print_error_msg(Resp);
			}
		}
	}

	return 0;
}