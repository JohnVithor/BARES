/**
 * @file evaluator.cpp
 * @brief      Implementação do métodos da classe Evaluator
 * @details    Converte uma expressão aritmética em notação infixa para posfixa.
 *             Depois que a conversão for feita, o programa avalia a expressão
 *             utilizando stacks. Apenas os operadores '+', '-', '*', '%', '/',
 *             e '^' ( para exponenciação ) são aceitos.
 *
 * @author     João Vítor Venceslau Coelho / Selan Rodrigues dos Santos
 * @since      28/10/2017
 * @date       11/11/2017
 */

#include "evaluator.hpp"
#include "parser.hpp"

using value_type = long int;

/**
 * @brief      Determina se o token avalizado é um operando
 *
 * @param[in]  c     Token a ser avaliado
 *
 * @return     True se for operando, False caso contrário.
 */
bool Evaluator::is_operand ( Token c )
{
	return c.type == Token::token_t::OPERAND;
}

/**
 * @brief      Determina se o token avalizado é um operador
 *
 * @param[in]  c     Token a ser avaliado
 *
 * @return     True se for operador, False caso contrário.
 */
bool Evaluator::is_operator ( Token c )
{
	return c.type == Token::token_t::OPERATOR;
}

/**
 * @brief      Determina se o token avalizado é um "("
 *
 * @param[in]  c     Token a ser avaliado
 *
 * @return     True se for "(", False caso contrário.
 */
bool Evaluator::is_opening_scope ( Token c )
{
	return c.type == Token::token_t::OPENING_SCOPE;
}

/**
 * @brief      Determina se o token avalizado é um ")"
 *
 * @param[in]  c     Token a ser avaliado
 *
 * @return     True se for ")", False caso contrário.
 */
bool Evaluator::is_closing_scope ( Token c )
{
	return c.type == Token::token_t::CLOSING_SCOPE;
}

/**
 * @brief      Determina se o token avalizado é um "^"
 *
 * @param[in]  c     Token a ser avaliado
 *
 * @return     True se for "^", False caso contrário.
 */
bool Evaluator::is_right_association ( Token c )
{
	return c.value[0] == '^';
}

/**
 * @brief      Determina qual a prioridade desse token ( operador )
 *
 * @param[in]  c     Token a ser avaliado
 *
 * @return     A prioridade do token avaliado.
 */
short Evaluator::get_precedence ( Token c )
{
	switch( c.value[0] )
	{
		case '^': return 3;
		// ==================================================
		case '*':
		case '%':
		case '/': return 2;
		// ==================================================
		case '+':
		case '-': return 1;
		// ==================================================
		default: return 0;
	}
}

/**
 * @brief      Determina qual dos dois tokens possui maior prioridade
 *
 * @param[in]  op1   A operação representada pelo primeiro token
 * @param[in]  op1   A operação representada pelo segundo token
 *
 * @return     True se a primeira operação tiver maior prioridade, False caso contrário.
 */
bool Evaluator::has_higher_precedence ( Token op1, Token op2 )
{
	short w1 = get_precedence( op1 );
	short w2 = get_precedence( op2 );

	if ( w1 == w2 and is_right_association( op1 ) )
	{
		return false;
	}

	return w1 >= w2 ;
}

/**
 * @brief      Converte a expressão tokenizada de infixa para posfixa
 *
 * @param[in]  infix_  Expressão em notação infixa
 *
 * @return     Mesma expressão em notação posfixa
 */
std::vector< Token > Evaluator::infix_to_postfix ( std::vector< Token > infix_ )
{
	std::vector< Token > postfix;
	
	jv::stack< Token > st( infix_.size() );

	for( Token s : infix_ )
	{
		if ( is_operand( s ) )
		{
			postfix.push_back( s );
		}
		else if ( is_opening_scope( s ) )
		{
			st.push( s );
		}
		else if ( is_closing_scope( s ) )
		{
			while( not st.empty() and not is_opening_scope( st.top() ) )
			{
				postfix.push_back( st.top() );
				st.pop();
			}
			st.pop();
		}
		else if ( is_operator( s ) )
		{
			while( not st.empty() and has_higher_precedence( st.top() , s ) ) 
			{
				postfix.push_back( st.top() );
				st.pop();
			}
			
			st.push( s );
		}
		else
		{
			assert(false);
		}
	}

	while( not st.empty() )
	{
		postfix.push_back( st.top() );
		st.pop();
	}

	return postfix;
}

/**
 * @brief      Executa a operação entre os dois valores informados com a
 *             operação indicada pelo token
 *
 * @param[in]  term1  Primeiro termo
 * @param[in]  term2  Segundo termo
 * @param[in]  op     Token que representa a operação
 *
 * @return     Retorna um EvaluatorResult, com o valor da operação e caso tenha
 *             ocorrido um erro qual foi.
 */
Evaluator::EvaluatorResult Evaluator::execute_operator ( value_type term1, value_type term2, Token op )
{
	Evaluator::EvaluatorResult result;
	result.value = 0;

	value_type result_aux;

	switch ( op.value[0] )
	{
		case '+' : result_aux = term1 + term2;
			break;
		case '-' : result_aux = term1 - term2;
			break;
		case '*' : result_aux = term1 * term2;
			break;			
		case '^' : result_aux = ( pow( term1, term2 ) );
			break;
		case '/' : 
			if ( term2 == 0 )
			{
				result.type = EvaluatorResult::code_t::DIVISION_BY_ZERO;
			}
			else
			{
				result_aux = term1/term2;	
			}		
			break;
		case '%' :
			if ( term2 == 0 )
			{
				result.type = EvaluatorResult::code_t::DIVISION_BY_ZERO;
			}
			else
			{
				result_aux = term1%term2;
			}		
			break;
	}

	if( result_aux <= std::numeric_limits< Parser::required_int_type >::max() 
        and result_aux >= std::numeric_limits< Parser::required_int_type >::min())
	{
		result.value = result_aux;
	}
	else if (result.type != EvaluatorResult::code_t::DIVISION_BY_ZERO)
	{
		result.type = EvaluatorResult::code_t::NUMERIC_OVERFLOW;
	}

	return result;
}

/**
 * @brief      Converte o valor de um token em um valor numérico
 *
 * @param[in]  tok   O token a ser convertido
 *
 * @return     O valor convertido
 */
value_type Evaluator::Token2Value ( Token tok )
{
	std::stringstream ss( tok.value );

	value_type result;

	ss >> result;
	
	return result;
}

/**
 * @brief      Avalia o valor representado pela expressão posfixa
 *
 * @param[in]  postfix  expressão a ser avaliada
 *
 * @return     EvaluatorResult contendo o valor encontrado e um código indicando se houve ou não um erro
 */
Evaluator::EvaluatorResult Evaluator::evaluate_postfix ( std::vector< Token > postfix )
{
	jv::stack< value_type > st( postfix.size() );

	Evaluator::EvaluatorResult result;

	for( Token s : postfix )
	{
		if ( is_operand( s ) )
		{
			st.push( Token2Value( s ) );
		}
		else if ( is_operator( s ) )
		{
			auto term2 = st.top(); st.pop();
			auto term1 = st.top(); st.pop();

			result = execute_operator( term1, term2, s );

			st.push( result.value );
		}
		else
		{
			assert(false);
		}
	}
	result.value = st.top();

	return result;
}