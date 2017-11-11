/**
 * @file parser.hpp
 * @brief      Declaração do métodos e atributos da classe Parser
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

#ifndef _PARSER_H_
#define _PARSER_H_

#include <iostream> 	// std::cout, std::cin
#include <iterator> 	// std::distance()
#include <vector>   	// std::vector
#include <sstream>  	// std::istringstream
#include <cstddef>  	// std::ptrdiff_t
#include <limits>   	// std::numeric_limits
#include <algorithm>	// std::copy

#include "token.hpp"  	// struct Token.

/**
 * @brief      Classe Parser, separa uma string em tokens
 *  		   e informa se algum erro ocorreu durante a tokenização
 */
class Parser
{
	public:

		/**
		 * @brief      Struct que contém o resultado da tokenização
		 * 			   e caso um erro tenha ocorrido em qual coluna da string
		 */
		struct ParserResult
		{

			typedef std::ptrdiff_t size_type;

			/**
			 * @brief      Enum com os possíveis códigos que podem ocorrer
			 */
			enum code_t {
					PARSER_OK = 0,
					UNEXPECTED_END_OF_EXPRESSION,
					ILL_FORMED_INTEGER,
					MISSING_TERM,
					EXTRANEOUS_SYMBOL,
					MISSING_CLOSING_PARENTHESIS,
					INTEGER_OUT_OF_RANGE
			};

			code_t type;      // Campo que informa o resultado da tokenização
			size_type at_col; // Guarda a coluna do erro.

			/**
			 * @brief      Construtor do ParserResult
			 *
			 * @param[in]  type_  O tipo do código
			 * @param[in]  col_   A coluna do erro
			 */
			explicit ParserResult( code_t type_=PARSER_OK , size_type col_=0u )
					: type{ type_ }
					, at_col{ col_ }
			{ /* empty */ }
		};


		typedef short int required_int_type;	// O tipo de número requerido
		typedef long long int input_int_type;	// O tipo de número que será interpretado


		/**
		 * @brief      Função que tokeniza um string
		 *
		 * @param[in]  e_    String a ser tokenizada
		 *
		 * @return     O código com o resultado da tokenização
		 */
		ParserResult parse( std::string e_ );

		/**
		 * @brief      Recupera o vector com os tokens da string
		 *             
		 * @return     Vector com os tokens
		 */
		std::vector< Token > get_tokens( void ) const;


		/**
		 * @brief      Construtor padrão do Parser
		 */
		Parser() = default;

		/**
		 * @brief      Destrutor padrão do Parser
		 */
		~Parser() = default;

		/**
		 * @brief      Construtor cópia do Parser deletado
		 *
		 * @param[in]  other  Parser a ser atribuido
		 */
		Parser( const Parser & other ) = delete;


		/**
		 * @brief      Sobrecarga do operador = para o Parser deletado
		 *
		 * @param[in]  other  Parser a ser atribuido
		 *
		 * @return     Novo parser
		 */
		Parser & operator=( const Parser & other ) = delete;

	private:

		static constexpr bool SUCCESS{ true };
		static constexpr bool FAILURE{ false };


		/**
		 * @brief      Enum com os possíveis símbolos que podem ser encontrados
		 */
		enum class terminal_symbol_t
		{
			TS_PLUS,	        //<! "+"
			TS_MINUS,	        //<! "-"
			TS_MOD,             //<! "%"
			TS_SLASH,           //<! "/"
			TS_ASTERISK,        //<! "*"
			TS_EXP,          	//<! "^"
			TS_CLOSING_SCOPE,   //<! ")"
			TS_OPENING_SCOPE,   //<! "("
			TS_ZERO,            //<! "0"
			TS_NON_ZERO_DIGIT,  //<! "1"->"9"
			TS_WS,              //<! white-space
			TS_TAB,             //<! tab
			TS_EOS,             //<! End Of String
			TS_INVALID	        //<! invalid token
		};

		std::string expr;               		// Expressão a ser avaliada
		std::string::iterator it_curr_symb;		// Ponteiro para o char atual dentro da expressão
		std::vector< Token > token_list; 		// Lista com os tokens extraidos da expressão


		/**
		 * @brief      Categoriza o símbolo ( char ) informado
		 *
		 * @param[in]  c_    Char a ser categorizado
		 *
		 * @return     Categora do símbolo
		 */
		terminal_symbol_t lexer( char c_ ) const;

		/**
		 * @brief      Avança do símbolo atual para o próximo
		 */
		void next_symbol( void );

		/**
		 * @brief      Verifica se é o fim da string e se o símbolo atual é da categoria informada
		 *
		 * @param[in]  c_    Categoria do símbolo a ser comparado
		 *
		 * @return     True se não for o fim da string e a catégoria é a mesma da informada, False caso contrário
		 */
		bool peek( terminal_symbol_t c_ ) const;

		/**
		 * @brief      Tenta aceitar um símbolo e avança para o próximo símbolo
		 *
		 * @param[in]  c_    Categoria do símbolo a ser comparado
		 *
		 * @return     True se catégoria é a mesma da informada, False caso contrário
		 */
		bool accept( terminal_symbol_t c_ );

		/**
		 * @brief      Ignora todos os espaços em branco anteriores ao próximo símbolo e tenta aceitá-lo
		 *
		 * @param[in]  c_	 Categoria do símbolo a ser comparado
		 *
		 * @return     True se catégoria é a mesma da informada, False caso contrário
		 */
		bool expect( terminal_symbol_t c_ );


		/**
		 * @brief      Ignora os espaços em branco até chegar em um símbolo novo
		 */
		void skip_ws( void );

		/**
		 * @brief      Verifica se a string acabou
		 *
		 * @return     True se é o fim da string, False caso contrário
		 */
		bool end_input( void ) const;

		/**
		 * @brief      Tenta aceitar um expressão
		 *
		 * @return     Resultado da Tokenização da expressão
		 */
		ParserResult expression();
		
		/**
		 * @brief      Tenta aceitar um termo
		 *
		 * @return     Resultado da Tokenização do termo
		 */
		ParserResult term();
		
		/**
		 * @brief      Tenta aceitar um inteiro
		 *
		 * @return     Resultado da Tokenização do inteiro
		 */
		ParserResult integer();
		
		/**
		 * @brief      Tenta aceitar um número natural
		 *
		 * @return     Resultado da Tokenização do natural
		 */
		ParserResult natural_number();
		
		/**
		 * @brief      Verifica se é um digito diferente de zero
		 *
		 * @return     True se for um digito diferente de zero, False se for zero
		 */
		bool digit_excl_zero();
		
		/**
		 * @brief      Verifica se é um digito
		 *
		 * @return     True se for um digito, False caso contrário
		 */
		bool digit();
};

#endif
