CC = cc
FLAGS = -Wall -Wextra -Werror
SRC_FILES = $(wildcard src/*.c) #guarda una lista con todos los archivos .c este en la carpeta src
OBJ = $(SRC_FILES:.c=.o)
PROGRAM = rush-02

#[make || make all] -> ejecuta la regla de $(PROGRAM): $(SRC_FILES)
all: $(PROGRAM)

#Compila todos los archivos de la lista SRC_FILES y genera el programa
$(PROGRAM): $(SRC_FILES)
	cc $(FLAGS) $(SRC_FILES) -o $(PROGRAM)
#[make clea]
#clean:
#	rm -f $(OBJ)
fclean: #clean
	rm -f $(PROGRAM) #-f $(OBJ)
