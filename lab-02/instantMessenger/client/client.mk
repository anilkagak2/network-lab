#
# client.mk:	Rules to make client & the backend
# 
# Anil Kag
# a.kag@iitg.ernet.in
#

SRC=src/
BIN=bin/
OBJ=obj/
HINC=../include/message.h

# Make all default
all: im-clientD imc

# 
# Client
# compilation of backend + main
im-clientD: clientD.o cmain.o
	g++ $(OBJ)clientD.o $(OBJ)cmain.o -o $(BIN)im-clientD

# as per the assignment client should be named "imc"
imc: client.o cmain.o
	g++ $(OBJ)client.o $(OBJ)cmain.o -o $(BIN)imc

# client backend
clientD.o: $(SRC)client.cpp $(SRC)client.h $(HINC)
	g++ -c $(SRC)client.cpp -o $(OBJ)clientD.o -D_DEBUG_

client.o: $(SRC)client.cpp $(SRC)client.h $(HINC)
	g++ -c $(SRC)client.cpp -o $(OBJ)client.o

# client main function
cmain.o: $(SRC)cmain.cpp $(SRC)server.h $(HINC)
	g++ -c $(SRC)cmain.cpp -o $(OBJ)cmain.o

# cleans the object & binaries
clean:
	rm -rf $(OBJ)*.o $(BIN)*
