/**
 * @file evaluator.hpp
 * @brief      Declaração do métodos e atributos da classe Evaluator
 * @details    Converte uma expressão aritmética em notação infixa para posfixa.
 *             Depois que a conversão for feita, o programa avalia a expressão
 *             utilizando stacks. Apenas os operadores '+', '-', '*', '%', '/',
 *             e '^' ( para exponenciação ) são aceitos.
 *
 * @author     João Vítor Venceslau Coelho / Selan Rodrigues dos Santos
 * @since      28/10/2017
 * @date       11/11/2017
 */

#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_

#include <iostream>  	// std::cout, std::endl
#include <string>    	// std::string
#include <cassert>   	// std::assert
#include <cmath>     	// std::pow
#include <stdexcept>	// std::runtime_error
#include <vector>		// std::vector
#include <limits> 		// std::numeric_limits

#include "token.hpp"	// struct Token.
#include "stack.hpp"  	// jv::stack

/**
 * @brief      Classe para converter uma expressão infixa para posfixa e então
 *             avaliando-a
 */
class Evaluator
{
	using value_type = long int;

	public:
	
	/**
	 * @brief      Struct que guarda o valor da avalização e um código que
	 *             representa o resultado ( ocorreu um erro ou não )
	 */
	struct EvaluatorResult
	{
		
		/**
		 * @brief      Enum com os possiveis resultados que podem ocorrer ( erros )
		 */
		enum code_t
		{
			RESULT_OK = 0,
			DIVISION_BY_ZERO,
			NUMERIC_OVERFLOW
		};

		value_type value; 
		code_t type;      

		/**
		 * @brief      Construtor para EvaluatorResult
		 *
		 * @param[in]  val   O valor a ser armazenado
		 * @param[in]  cod   O código para o resultado
		 */
		explicit EvaluatorResult( value_type val = 0, code_t cod = code_t::RESULT_OK )
			: value( val )
			, type( cod )
		{ /* Vazio */ }
	};

	private:
		
		std::vector< Token > expression;

		/**
		 * @brief      Determina se o token avalizado é um operando
		 *
		 * @param[in]  c     Token a ser avaliado
		 *
		 * @return     True se for operando, False caso contrário.
		 */
		bool is_operand ( Token c );

		/**
		 * @brief      Determina se o token avalizado é um operador
		 *
		 * @param[in]  c     Token a ser avaliado
		 *
		 * @return     True se for operador, False caso contrário.
		 */
		bool is_operator ( Token c );

		/**
		 * @brief      Determina se o token avalizado é um "("
		 *
		 * @param[in]  c     Token a ser avaliado
		 *
		 * @return     True se for "(", False caso contrário.
		 */
		bool is_opening_scope ( Token c );

		/**
		 * @brief      Determina se o token avalizado é um ")"
		 *
		 * @param[in]  c     Token a ser avaliado
		 *
		 * @return     True se for ")", False caso contrário.
		 */
		bool is_closing_scope ( Token c );

		/**
		 * @brief      Determina se o token avalizado é um "^"
		 *
		 * @param[in]  c     Token a ser avaliado
		 *
		 * @return     True se for "^", False caso contrário.
		 */
		bool is_right_association ( Token c );

		/**
		 * @brief      Determina qual a prioridade desse token ( operador )
		 *
		 * @param[in]  c     Token a ser avaliado
		 *
		 * @return     A prioridade do token avaliado.
		 */
		short get_precedence ( Token c );

		/**
		 * @brief      Determina qual dos dois tokens possui maior prioridade
		 *
		 * @param[in]  op1   A operação representada pelo primeiro token
		 * @param[in]  op1   A operação representada pelo segundo token
		 *
		 * @return     True se a primeira operação tiver maior prioridade, False caso contrário.
		 */
		bool has_higher_precedence ( Token op1, Token op2 );

		/**
		 * @brief      Executa a operação entre os dois valores informados com a
		 *             operação indicada pelo token
		 *
		 * @param[in]  term1  Primeiro termo
		 * @param[in]  term2  Segundo termo
		 * @param[in]  op     Token que representa a operação
		 *
		 * @return     Retorna um EvaluatorResult, com o valor da operação e
		 *             caso tenha ocorrido um erro qual foi.
		 */
		Evaluator::EvaluatorResult execute_operator ( value_type term1, value_type term2, Token op );

		/**
		 * @brief      Converte o valor de um token em um valor numérico
		 *
		 * @param[in]  tok   O token a ser convertido
		 *
		 * @return     O valor convertido
		 */
		value_type Token2Value ( Token tok );


	public:

		/**
		 * @brief      Construtor padrão do Evaluator
		 */
		Evaluator() = default;

		/**
		 * @brief      Destrutor padrão do Evaluator
		 */
		~Evaluator() = default;
		
		/**
		 * @brief      Construtor cópia do Evaluator deletado
		 *
		 * @param[in]  other  O outro Evaluator
		 */
		Evaluator( const Evaluator & other ) = delete;

		/**
		 * @brief      Sobrecarga do operador = deletado
		 *
		 * @param[in]  other  O outro Evaluator
		 *
		 * @return     O novo Evaluator
		 */
		Evaluator & operator=( const Evaluator & other ) = delete;
	
		/**
		 * @brief      Converte a expressão tokenizada de infixa para posfixa
		 *
		 * @param[in]  infix_  Expressão em notação infixa
		 *
		 * @return     Mesma expressão em notação posfixa
		 */
		std::vector< Token > infix_to_postfix ( std::vector< Token > infix_ );

		/**
		 * @brief      Avalia o valor representado pela expressão posfixa
		 *
		 * @param[in]  postfix  expressão a ser avaliada
		 *
		 * @return     EvaluatorResult contendo o valor encontrado e um código
		 *             indicando se houve ou não um erro
		 */
		Evaluator::EvaluatorResult evaluate_postfix ( std::vector< Token > postfix );

};

#endif