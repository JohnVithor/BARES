/**
 * @file bares.cpp
 * @brief      Avalia a(s) expressão(ões) informadas pelo usuário, informando o
 *             resultado ou se algum erro foi encontrado
 * @details    
 *
 * @author     João Vítor Venceslau Coelho / Selan Rodrigues dos Santos
 * @since      28/10/2017
 * @date       12/11/2017
 */

#include <iostream>
#include <iomanip>
#include <vector>

#include "parser.hpp"
#include "evaluator.hpp"

/**
 * @brief      Imprime a devida mensagem de erro a partir do código informado
 *
 * @param[in]  result  O resultado da avaliação do Parser
 * @param[in]  str     A string com a expressão onde o erro foi encontrado
 */
void print_parser_error( const Parser::ParserResult & result )
{
	switch ( result.type )
	{
		case Parser::ParserResult::UNEXPECTED_END_OF_EXPRESSION:
			std::cout << "Unexpected end of input at column (" << result.at_col+1 << ")!\n";
			break;
		case Parser::ParserResult::ILL_FORMED_INTEGER:
			std::cout << "Ill formed integer at column (" << result.at_col+1 << ")!\n";
			break;
		case Parser::ParserResult::MISSING_TERM:
			std::cout << "Missing <term> at column (" << result.at_col+1 << ")!\n";
			break;
		case Parser::ParserResult::EXTRANEOUS_SYMBOL:
			std::cout << "Extraneous symbol after valid expression found at column (" << result.at_col+1 << ")!\n";
			break;
		case Parser::ParserResult::INTEGER_OUT_OF_RANGE:
			std::cout << "Integer constant out of range beginning at column (" << result.at_col+1 << ")!\n";
			break;
		case Parser::ParserResult::MISSING_CLOSING_PARENTHESIS:
			std::cout << "Missing closing \")\" at column (" << result.at_col+1 << ")!\n";
			break;
		default:
			std::cout << ">>> Unhandled error found!\n";
			break;
	}
}

/**
 * @brief      Imprime a devida mensagem de erro a partir do código informado
 *
 * @param[in]  result  O resultado da avaliação do Evaluator
 */
void print_evaluator_error( const Evaluator::EvaluatorResult & result )
{
	switch ( result.type )
	{
		case Evaluator::EvaluatorResult::NUMERIC_OVERFLOW:
			std::cout << "Numeric overflow error!\n";
			break;
		case Evaluator::EvaluatorResult::DIVISION_BY_ZERO:
			std::cout << "Division by zero!\n";
			break;
		default:
			std::cout << "Unhandled error found!\n";
			break;
	}
}

/**
 * @brief      Função Principal
 *
 * @param[in]  argc  The argc
 * @param      argv  The argv
 *
 * @return     0
 */
int main(int argc, char const *argv[])
{

	std::vector<std::string> expressions;
	std::string aux;

	while ( std::getline(std::cin, aux) and aux != "q" and aux != "p")
	{
		expressions.push_back( aux );
	}

	Parser my_parser;
	Evaluator my_evaluator;

	for( const auto & expr : expressions )
	{
		auto result = my_parser.parse( expr );

		if ( result.type != Parser::ParserResult::PARSER_OK )
		{
			print_parser_error( result );
		}
		else
		{
			auto lista = my_parser.get_tokens();

			auto postfix = my_evaluator.infix_to_postfix( lista );
			auto resultado = my_evaluator.evaluate_postfix( postfix );

			if ( resultado.type != Evaluator::EvaluatorResult::code_t::RESULT_OK )
			{
				print_evaluator_error( resultado );
			}
			else
			{
				std::cout << resultado.value << std::endl;
			}
		}
	}

	return 0;
}