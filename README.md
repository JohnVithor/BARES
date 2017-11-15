# BARES
## Basic ARithmetic Expression Evaluator based on Stacks

- Aluno: João Vítor Venceslau Coelho
- Professor: Selan Rodrigues dos Santos 

## Objetivo
O objetivo deste exercı́cio de programação é utilizar as estrutura de dados pilha e fila no contexto de uma aplicação real. A aplicação a ser desenvolvida é um avaliador de expressões aritméticas simples.

### Descrição
O programa pode receber expressões durante a execução, assim o usúario deve digitar cada expressão no terminal, ou pode utilizar um arquivo contendo uma ou mais expressões, uma por linha. O programa irá, avaliar cada expressão e imprimir seu respectivo resultado na saı́da padrão, std::cout , ou em um arquivo texto de resultados informado pelo usuário.

### Erros que foram tratados

1.Integer constant out of range beginning at column (n)!: O operando que se inicia na coluna n está fora da faixa permitida.

    Ex.: 1000000 − 2, coluna 1.
  
2.Missing <term> at column (n)!: A partir da coluna n está faltando o resto da expressão .
  
    Ex.: 2 +, coluna 4.
  
3.Extraneous symbol after valid expression found at column (n)!: Existe um sı́mbolo qualquer (válido ou não) que foi encontrado depois que uma expressão completa foi validada, na coluna n.

    Ex.: 2 = 3, coluna 3; ou 2 + 3 4, coluna 7.
  
4.Ill formed integer at column (n)!: Contante inteira iniciada na coluna n possui sı́mbolo inválido em sua composição.

    Ex.: − 5, coluna 1; ou ! 3, coluna 1; ou ) 2 + 4, coluna 1.
  
5.Missing closing ”)”at column (n)!: Está faltando um parêntese de fechamento ‘)’ para um parêntese de abertura ‘(’ correspondente, na coluna n.

    Ex.: ((2 % 3) ∗ 8, coluna 13.
  
6.Unexpected end of expression at column (n)!: Caso receba uma linha contendo apenas espaços, cujo final é encontrado na coluna n.

    Ex.:   , coluna 4 ou    (, coluna 4;
  
7.Division by zero!: Houve divisão cujo quociente é zero.

    Ex.: 3/(1 − 1); ou 10/(3 ∗ 3ˆ−2). Nestes casos não é preciso informar a coluna.

8.Numeric overflow error!: Acontece quando uma operação dentro da expressão ou a expressao inteira estoura o limite das constantes numéricas.

    Ex.: 20 ∗ 20000. Nestes casos não é preciso informar a coluna.

## Compilação
Para a compilação do BARES utilize o comando 'make' no terminal do Linux.
Para gerar a documentação digite 'make doxy' no terminal.

## Executar o programa
A forma geral de execução do programa é

	$./bin/bares
Para digitar as expressões durante a execução do programa, ao final digite "q" ou "p" para mostrar o resultados

ou
		
	$./bin/bares < arquivo_entrada > [arquivo_saida]

Para utilizar arquivos com as expressões e outro com os resultados obtidos.
