/**
 * @file stack.hpp
 * @brief      Declaração e implementação dos métodos e atributos da classe
 *             jv::stack
 * @details    
 *
 * @author     João Vítor Venceslau Coelho
 * @since      22/10/2017
 * @date       11/11/2017
 */

#ifndef STACK_H
#define STACK_H

#include <list>

namespace jv
{
	template < typename T >
	class stack
	{
		public:
			typedef T value_type;
			typedef std::size_t size_type;
			typedef value_type& reference;
			typedef const value_type& const_reference;

		private:
			size_type m_top;
			size_type m_capacity;
			std::list< value_type > m_storage;

		public:

			/**
			 * @brief      Construtor padrão da stack
			 *
			 * @param[in]  size  Tamanho inicial da stack, caso nenhum valor
			 *                   seja indicado, 1 será o tamanho da stack
			 */
			stack( size_type size = 1)
				: m_top( 0 )
				, m_capacity( size )
				, m_storage()
				{ /* Vazio */ }

			/**
			 * @brief      Destrutor padrão da stack
			 */
			~stack() = default;

			/**
			 * @brief      Adiciona um elemento ao topo da stack
			 *
			 * @param[in]  valor  Elemento a ser adicionado
			 */
			void push ( const_reference valor );

			/**
			 * @brief      Remove o elemento do topo da stack
			 */
			void pop ( void );

			/**
			 * @brief      Verifica qual é o elemento no topo da stack
			 *
			 * @return     Elemento no topo da stack
			 */
			const_reference top ( void ) const;

			/**
			 * @brief      Remove todos os elementos da stack
			 */
			void clear ( void );

			/**
			 * @brief      Altera a capacidade da stack para o valor informado
			 *
			 * @param[in]  capacidade  Nova capacidade da stack
			 */
			void resize ( size_type capacidade );

			/**
			 * @brief      Informa qual é o tamanho ( número de elementos ) da
			 *             stack
			 *
			 * @return     Tamanho da stack
			 */
			size_type size ( void ) const;

			/**
			 * @brief      Informa qual é o tamanho máximo atualmente permitido
			 *             na ( número de elementos ) stack
			 *
			 * @return     Capacidade da stack
			 */
			size_type capacity ( void ) const;

			/**
			 * @brief      Informa se a stack está vazia
			 *
			 * @return     true se ela estiver vazia, false caso contrário
			 */
			bool empty ( void ) const;

			/**
			 * @brief      Informa se a stack está cheia
			 *
			 * @return     true se ela estiver cheia, false caso contrário
			 */
			bool full ( void ) const;

	};

	typedef std::size_t size_type;

	template < typename value_type >
	using reference = value_type&;

	template < typename value_type >
	using const_reference = const value_type&;

	template < typename value_type >
	void stack< value_type >::push ( const_reference valor )
	{
		if ( full() )
		{
			throw std::out_of_range( "push() -> a pilha está cheia!" );
		}
		m_storage.push_back( valor );
		++m_top;

	}

	template < typename value_type >
	void stack< value_type >::pop ( void )
	{
		if ( empty() )
		{
			throw std::out_of_range( "pop() -> a pilha está vazia!" );
		}
		m_storage.pop_back();
		--m_top;
	}

	template < typename value_type >
	const_reference< value_type > stack< value_type >::top ( void ) const
	{
		if ( empty() )
		{
			throw std::out_of_range( "top() -> a pilha está vazia!" );
		}
		return m_storage.back();
	}

	template < typename value_type >
	void stack< value_type >::clear ( void )
	{ 
		m_storage.clear();
		m_top = 0;
	}

	template < typename value_type >
	void stack< value_type >::resize ( size_type capacidade )
	{
		while ( m_capacity > capacidade )
		{
			pop();
		}
		m_capacity = capacidade;
	}

	template < typename value_type >
	size_type stack< value_type >::size ( void ) const
	{
		return m_top; 
	}

	template < typename value_type >
	size_type stack< value_type >::capacity ( void ) const
	{
		return m_capacity; 
	}

	template < typename value_type >
	bool stack< value_type >::empty ( void ) const
	{ 
		return  m_top == 0;
	}

	template < typename value_type >
	bool stack< value_type >::full ( void ) const
	{
		return m_top == m_capacity;
	}
}

#endif