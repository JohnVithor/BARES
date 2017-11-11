/**
 * @file parser.cpp
 * @brief      Implementação do métodos da classe Parser
 * @details    Implementa um analisador descendente recursivo para uma gramática EBNF
 * 
 *             <expr>            := <term>,{ ("+"|"-"),<term> };
 *             <term>            := <integer>;
 *             <integer>         := 0 | ["-"],<natural_number>;
 *             <natural_number>  := <digit_excl_zero>,{<digit>};
 *             <digit_excl_zero> := "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9";
 *             <digit>           := "0"| <digit_excl_zero>;
 *             
 * @author     João Vítor Venceslau Coelho / Selan Rodrigues dos Santos
 * @since      28/10/2017
 * @date       11/11/2017
 */

#include "parser.hpp"
#include <iterator>
#include <algorithm>

/**
 * @brief      Categoriza o símbolo ( char ) informado
 *
 * @param[in]  c_    Char a ser categorizado
 *
 * @return     Categora do símbolo
 */
Parser::terminal_symbol_t Parser::lexer ( char c_ ) const
{
	switch( c_ )
	{
		case '+':  return terminal_symbol_t::TS_PLUS;
		case '-':  return terminal_symbol_t::TS_MINUS;
		case '%':  return terminal_symbol_t::TS_MOD;
		case '/':  return terminal_symbol_t::TS_SLASH;
		case '*':  return terminal_symbol_t::TS_ASTERISK;
		case '^':  return terminal_symbol_t::TS_EXP;
		case ')':  return terminal_symbol_t::TS_CLOSING_SCOPE;
		case '(':  return terminal_symbol_t::TS_OPENING_SCOPE;
		case ' ':  return terminal_symbol_t::TS_WS;
		case   9:  return terminal_symbol_t::TS_TAB;
		case '0':  return terminal_symbol_t::TS_ZERO;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':  return terminal_symbol_t::TS_NON_ZERO_DIGIT;
		case '\0': return terminal_symbol_t::TS_EOS;
	}
	return terminal_symbol_t::TS_INVALID;
}

/**
 * @brief      Avança do símbolo atual para o próximo
 */
void Parser::next_symbol ( void )
{
	std::advance( it_curr_symb, 1 );
}

/**
 * @brief      Verifica se a string acabou
 *
 * @return     True se é o fim da string, False caso contrário
 */
bool Parser::end_input ( void ) const
{
	return it_curr_symb == expr.end();
}

/**
 * @brief      Verifica se é o fim da string e se o símbolo atual é da categoria informada
 *
 * @param[in]  c_    Categoria do símbolo a ser comparado
 *
 * @return     True se não for o fim da string e a catégoria é a mesma da informada, False caso contrário
 */
bool Parser::peek ( terminal_symbol_t c_ ) const
{
	return ( not end_input() and lexer( *it_curr_symb ) == c_ );
}

/**
 * @brief      Tenta aceitar um símbolo e avança para o próximo símbolo
 *
 * @param[in]  c_    Categoria do símbolo a ser comparado
 *
 * @return     True se catégoria é a mesma da informada, False caso contrário
 */
bool Parser::accept ( terminal_symbol_t c_ )
{
	if ( peek( c_ ) )
	{
		next_symbol();
		return true;
	}

	return false;
}

/**
 * @brief      Ignora todos os espaços em branco anteriores ao próximo símbolo e tenta aceitá-lo
 *
 * @param[in]  c_	 Categoria do símbolo a ser comparado
 *
 * @return     True se catégoria é a mesma da informada, False caso contrário
 */
bool Parser::expect ( terminal_symbol_t c_ )
{
	skip_ws();
	return accept( c_ );
}

/**
 * @brief      Ignora os espaços em branco até chegar em um símbolo novo
 */
void Parser::skip_ws ( void )
{
	while ( not end_input() and
			( lexer( *it_curr_symb ) == Parser::terminal_symbol_t::TS_WS  or
			  lexer( *it_curr_symb ) == Parser::terminal_symbol_t::TS_TAB ) )
	{
		next_symbol();
	}
}

/**
 * @brief      Tenta aceitar um expressão
 *
 * @return     Resultado da Tokenização da expressão
 */
Parser::ParserResult Parser::expression ()
{
	skip_ws();

	Parser::ParserResult result = term();

	while ( result.type == ParserResult::code_t::PARSER_OK )
	{
		if ( expect( terminal_symbol_t::TS_PLUS) )
		{
			token_list.emplace_back( Token( "+", Token::token_t::OPERATOR) );
		}
		else if ( expect( terminal_symbol_t::TS_MINUS) )
		{
			token_list.emplace_back( Token( "-", Token::token_t::OPERATOR) );   
		}
		else if ( expect( terminal_symbol_t::TS_ASTERISK) )
		{
			token_list.emplace_back( Token( "*", Token::token_t::OPERATOR) );
		}
		else if ( expect( terminal_symbol_t::TS_SLASH) )
		{
			token_list.emplace_back( Token( "/", Token::token_t::OPERATOR) );
		}
		else if ( expect( terminal_symbol_t::TS_MOD) )
		{
			token_list.emplace_back( Token( "%", Token::token_t::OPERATOR) );
		}
		else if ( expect( terminal_symbol_t::TS_EXP) )
		{
			token_list.emplace_back( Token( "^", Token::token_t::OPERATOR) );
		}
		else 
		{
			return result;
		}

		result = term();
		if ( result.type != ParserResult::code_t::PARSER_OK 
			and result.type != ParserResult::code_t::INTEGER_OUT_OF_RANGE and end_input())
		{
			result.type = ParserResult::code_t::MISSING_TERM;
			return result;
		}
	}

	return result;
}

/**
 * @brief      Tenta aceitar um termo
 *
 * @return     Resultado da Tokenização do termo
 */
Parser::ParserResult Parser::term ()
{
	skip_ws();

	ParserResult result = ParserResult( ParserResult::code_t::MISSING_TERM, std::distance( expr.begin(), it_curr_symb) );

	if( expect( terminal_symbol_t::TS_OPENING_SCOPE) )
	{
		token_list.emplace_back( Token( "(", Token::token_t::OPENING_SCOPE) );
		result = expression();
		
		if(result.type == ParserResult::code_t::PARSER_OK)
		{
			if( not expect( terminal_symbol_t::TS_CLOSING_SCOPE) )
			{
				return ParserResult( ParserResult::code_t::MISSING_CLOSING_PARENTHESIS
				, std::distance( expr.begin(), it_curr_symb) );
			}

			token_list.emplace_back( Token( ")", Token::token_t::CLOSING_SCOPE) );
		}
	}
	else
	{
		result = integer();
	}

	return result;
}

/**
 * @brief      Tenta aceitar um inteiro
 *
 * @return     Resultado da Tokenização do inteiro
 */
Parser::ParserResult Parser::integer ()
{
	if ( accept( terminal_symbol_t::TS_ZERO ) )
	{
		token_list.emplace_back( Token( "0", Token::token_t::OPERAND ) );
		return ParserResult( ParserResult::code_t::PARSER_OK );
	}

	auto cont = 0;
	while ( expect( terminal_symbol_t::TS_MINUS ) )
	{
		++cont;
	}

	auto begin_token = it_curr_symb;
	auto result = natural_number();
	
	if ( result.type == ParserResult::PARSER_OK )
	{
		result.at_col = cont;

		std::string token_str;
		std::copy( begin_token, it_curr_symb, std::back_inserter( token_str ) );

		if ( cont % 2 == 1 )
		{
			token_str = "-" + token_str;
			--cont;
		}

		input_int_type token_int;

		std::istringstream iss( token_str );

		iss >> token_int;

		if( iss.fail()
			or token_int >= std::numeric_limits< Parser::required_int_type >::max() 
			or token_int <= std::numeric_limits< Parser::required_int_type >::min())
		{
			return ParserResult( ParserResult::code_t::INTEGER_OUT_OF_RANGE,
					std::distance( expr.begin(), begin_token ) );
		}

		token_list.emplace_back( Token( token_str, Token::token_t::OPERAND ) );
	}

	return result;
}

/**
 * @brief      Tenta aceitar um número natural
 *
 * @return     Resultado da Tokenização do natural
 */
Parser::ParserResult Parser::natural_number ()
{
	if ( not digit_excl_zero() )
	{
		return ParserResult( ParserResult::code_t::ILL_FORMED_INTEGER
		, std::distance( expr.begin(), it_curr_symb ) );
	}

	while( digit() ) /* Vazio */ ;

	return ParserResult( ParserResult::code_t::PARSER_OK );
}

/**
 * @brief      Verifica se é um digito diferente de zero
 *
 * @return     True se for um digito diferente de zero, False se for zero
 */
bool Parser::digit_excl_zero ()
{
	return accept( terminal_symbol_t::TS_NON_ZERO_DIGIT );
}

/**
 * @brief      Verifica se é um digito
 *
 * @return     True se for um digito, False caso contrário
 */
bool Parser::digit ()
{
	return ( accept( terminal_symbol_t::TS_ZERO ) or digit_excl_zero() );
}

/**
 * @brief      Função que tokeniza um string
 *
 * @param[in]  e_    String a ser tokenizada
 *
 * @return     O código com o resultado da tokenização
 */
Parser::ParserResult Parser::parse ( std::string e_ )
{
	expr = e_;
	it_curr_symb = expr.begin();
	token_list.clear();

	skip_ws();
	if ( end_input() )
	{
		return ParserResult( ParserResult::code_t::UNEXPECTED_END_OF_EXPRESSION
			, std::distance( expr.begin(), it_curr_symb ) );
	}

	auto result = expression();

	if ( result.type == ParserResult::code_t::PARSER_OK )
	{

		skip_ws();
		if ( not end_input() )
		{
			return ParserResult( ParserResult::code_t::EXTRANEOUS_SYMBOL, std::distance( expr.begin(), it_curr_symb ) );
		}
	}
	return result;
}

/**
 * @brief      Recupera o vector com os tokens da string
 *             
 * @return     Vector com os tokens
 */
std::vector< Token > Parser::get_tokens ( void ) const
{
	return token_list;
}



//==========================[ End of parse.cpp ]==========================//
