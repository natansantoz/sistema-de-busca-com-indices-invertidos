##############################################################################
# Exemplo de makefile para um projeto em linguagem C++...
# Para simplificar, todos os arquivos estão em um mesmo diretório
##############################################################################
# Arquivo principal: main.cpp
##############################################################################
#                       Arquivos auxiliares do projeto
##############################################################################
# Registro.cpp: depende de Buffer.h 
# Buffer.cpp: depende de Buffer.h Registro.h e  
# : depende de Buffer.h  e Registro.h 
##############################################################################

# definindo as variáveis do projeto (MAIN é o nome do arquivo principal, que contem a função main, sem a extensao)
MAIN 	:= main
# objetos a serem gerados na compilação
OBJECTS := $(MAIN).o Registro.o Buffer.o Indice.o IndiceInvertido.o IndiceInvertidoManager.o
# (para C, pode-se usar o padrão ISO 2011 (c11) ou 2018 (c18), dependendo do seu compilador...
# usar -std=c98 no lugar do c++11, c++17, c++20, etc.. 

FLAGS 	:= -g -Wall -Wextra -std=c++17 -pedantic-errors


# habilitar a depuração
#DEBUG :=  -g

# necessário apenas quando se incluir  a biblioteca <math.h> em algum arquivo fonte no projeto
MATH 	:= -lm

# definição do compilador: para C usar o gcc, por exemplo
CC		:= g++

ifeq ($(OS), Windows_NT)
    OUTPUTMAIN := $(MAIN).exe
    RM_COMMAND := cmd /c del /f /q
    SLASH := \
    CLEAN_COMMAND_DAT := $(RM_COMMAND) *.dat
    CLEAN_COMMAND_O := $(RM_COMMAND) *.o
	CLEAN_DAT_FORCE := $(RM_COMMAND) /f /q *.dat

else
    OUTPUTMAIN := $(MAIN).out
    RM_COMMAND := rm
    SLASH := /
    CLEAN_COMMAND_DAT := $(RM_COMMAND) *.dat 
	CLEAN_COMMAND_O := $(RM_COMMAND) *.o
	
	
endif


# ponto de compilação principal
all: main.exe
	@echo Compilado com sucesso.

# gerando o arquivo executavel
main.exe: $(OBJECTS)  
	$(CC) $(FLAGS) $(OBJECTS) -o $(OUTPUTMAIN) $(MATH)
	
# gerando o arquivo objeto da função principal... 
main.o: $(MAIN).cpp  Registro.h Buffer.h
	$(CC) $(FLAGS) -c $(MAIN).cpp
	
# gerando o arquivo objeto Registro.o
Registro.o: Registro.cpp Registro.h
	$(CC) $(FLAGS) -c Registro.cpp

# gerando o arquivo objeto Buffer.o
Buffer.o: Buffer.cpp Buffer.h Registro.h 
	$(CC) $(FLAGS) -c Buffer.cpp

# gerando o arquivo objeto Indice.o
Indice.o: Indice.cpp Indice.h 
	$(CC) $(FLAGS) -c Indice.cpp

# gerando o arquivo objeto Indice.o
IndiceInvertido.o: IndiceInvertido.cpp IndiceInvertido.h 
	$(CC) $(FLAGS) -c IndiceInvertido.cpp

# gerando o arquivo objeto Indice.o
IndiceInvertidoManager.o: IndiceInvertidoManager.cpp IndiceInvertidoManager.h 
	$(CC) $(FLAGS) -c IndiceInvertidoManager.cpp


clean:
	-$(RM_COMMAND) $(OBJECTS)
	-$(RM_COMMAND) $(OUTPUTMAIN)
	-$(CLEAN_COMMAND_DAT)
	-$(CLEAN_COMMAND_O)
	-$(RM_COMMAND) main
	-$(RM_COMMAND) Menu
	-$(CLEAN_DAT_FORCE)
	
	@echo Cleanup finalizado.

run: all
	./$(OUTPUTMAIN)


build:
	$(CC) $(FLAGS)  $(MAIN).cpp Registro.cpp Buffer.cpp Indice.cpp IndiceInvertido.cpp IndiceInvertidoManager.cpp -o $(OUTPUTMAIN) $(MATH)

