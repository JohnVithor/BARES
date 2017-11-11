# Makefile baseado no exemplo dado pelo Professor Silvio Costa Sampaio

# $@ nome do alvo (target)
# $^ lista com os nomes de todos os pre-requisitos sem duplicatas
# $< nome do primeiro pre-requisito

# Comandos do sistema operacional
RM = rm -rf

# Compilador
CC=g++

# Variaveis para os subdiretorios
LIB_DIR=./lib
INC_DIR=./include
SRC_DIR=./src
OBJ_DIR=./build
BIN_DIR=./bin
DOC_DIR=./doc
TEST_DIR=./test

# Opcoes de compilacao
CFLAGS = -Wall -pedantic -ansi -std=c++1y

.PHONY: all clean distclean doxy

all: dir bares

debug: CFLAGS += -g -O0 -pg
debug: dir bares

bares: $(OBJ_DIR)/parser.o $(OBJ_DIR)/evaluator.o $(OBJ_DIR)/bares.o
	@echo "============="
	@echo "Ligando o alvo $@"
	@echo "============="
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $^
	@echo "+++ [Executavel bares criado em $(BIN_DIR)] +++"
	@echo "============="


$(OBJ_DIR)/parser.o: $(SRC_DIR)/parser.cpp $(INC_DIR)/parser.hpp $(INC_DIR)/token.hpp
	$(CC) -c $(CFLAGS) -lm -I$(INC_DIR)/ -o $@ $<

$(OBJ_DIR)/evaluator.o: $(SRC_DIR)/evaluator.cpp $(INC_DIR)/evaluator.hpp $(INC_DIR)/stack.hpp
	$(CC) -c $(CFLAGS) -lm -I$(INC_DIR)/ -o $@ $<

$(OBJ_DIR)/bares.o: $(SRC_DIR)/bares.cpp
	$(CC) -c $(CFLAGS) -lm -I$(INC_DIR)/ -o $@ $<

doxy:
	$(RM) $(DOC_DIR)/*
	doxygen Doxyfile

dir:
	mkdir -p bin build doc
	
valgrind:
	valgrind -v --leak-check=full --show-reachable=yes ./bin/bares


clean: dir
	$(RM) $(BIN_DIR)/*
	$(RM) $(OBJ_DIR)/*
	$(RM) $(DOC_DIR)/*

# FIM do Makefile
