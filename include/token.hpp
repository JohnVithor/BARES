/**
 * @file token.hpp
 * @brief      Declaração da classe / struct Token
 * @details    
 *
 * @author     João Vítor Venceslau Coelho / Selan Rodrigues dos Santos
 * @since      28/10/2017
 * @date       11/11/2017
 */

#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>   // std::string
#include <iostream> // std::ostream

/**
 * @brief      Struct que representa um Token, isto é, informa qual é o valor do
 *             Token e qual é a sua identificação.
 */
struct Token
{
	public:

		/**
		 * @brief      Enum que informa a identificação do Token
		 */
		enum class token_t : int
		{
			OPERAND = 0,    // Basicamente números
			OPERATOR,       // "+", "-", "*", "/", "%", "^"
			CLOSING_SCOPE,  // ")"
			OPENING_SCOPE   // "("
		};

		std::string value; // O valor do Token
		token_t type;      // O tipo do Token

		/**
		 * @brief      Construtor de um Token
		 *
		 * @param[in]  v_    O Valor do Token
		 * @param[in]  t_    O tipo do Token
		 */
		explicit Token( std::string v_="", token_t t_ = token_t::OPERAND )
			: value( v_ )
			, type( t_ )
		{/* Vazio */}

		/**
		 * @brief      Sobrecarga do operador << para Tokens
		 *             
		 * @param[in]  os_   O ostream para o qual o token será mandado
		 * @param[in]  t_    O Token a ser impresso
		 * 
		 * @return     O ostream com o Token já formatado
		 */
		friend std::ostream & operator<<( std::ostream& os_, const Token & t_ )
		{
			std::string types[] = { "OPERAND", "OPERATOR", "CLOSING SCOPE", "OPENING SCOPE" };

			os_ << "<" << t_.value << "," << types[(int)(t_.type)] << ">";

			return os_;
		}
};

#endif
