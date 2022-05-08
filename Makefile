#Vars
CC=g++
CFLAGS=-I.
DEPS = scanner.h parser.h testTree.h safeExit.h Assembler.h
OBJ = main.o scanner.o parser.o testTree.o safeExit.o Assembler.o
BIN = genCode

#Make Target Object files w/ suffix rules
%.o: %.cpp $(DEPS)
	$(CC) -g -std=c++11 -c -o $@ $< $(CFLAGS)

#making the bin
$(BIN): $(OBJ)
	$(CC) -g -std=c++11 -o $@ $^ $(CFLAGS)

#Clean
clean: 
	rm $(OBJ) $(BIN)