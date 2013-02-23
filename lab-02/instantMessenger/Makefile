#
# Makefile:	Rules to make client & server along with
#		their backends
# 
# Anil Kag
# IIT Guwahati
# a.kag@iitg.ernet.in
#

SRC=src/
BIN=bin/
OBJ=obj/

# Make all default
all: im-serverD im-clientD ims imc

# Server
# compilation of backend + main
im-serverD: serverD.o smain.o
	g++ $(OBJ)serverD.o $(OBJ)smain.o -o $(BIN)im-serverD 

# as per the assignment server should be named "ims"
ims: server.o smain.o
	g++ $(OBJ)server.o $(OBJ)smain.o -o $(BIN)ims 

# server backend
serverD.o: $(SRC)server.cpp $(SRC)server.h $(SRC)message.h
	g++ -c $(SRC)server.cpp -o $(OBJ)serverD.o -D_DEBUG_

server.o: $(SRC)server.cpp $(SRC)server.h $(SRC)message.h
	g++ -c $(SRC)server.cpp -o $(OBJ)server.o 

# server main function
smain.o: $(SRC)smain.cpp $(SRC)server.h $(SRC)message.h
	g++ -c $(SRC)smain.cpp -o $(OBJ)smain.o


# 
# Client
# compilation of backend + main
im-clientD: clientD.o cmain.o
	g++ $(OBJ)clientD.o $(OBJ)cmain.o -o $(BIN)im-clientD

# as per the assignment client should be named "imc"
imc: client.o cmain.o
	g++ $(OBJ)client.o $(OBJ)cmain.o -o $(BIN)imc

# client backend
clientD.o: $(SRC)client.cpp $(SRC)client.h $(SRC)message.h
	g++ -c $(SRC)client.cpp -o $(OBJ)clientD.o -D_DEBUG_

client.o: $(SRC)client.cpp $(SRC)client.h $(SRC)message.h
	g++ -c $(SRC)client.cpp -o $(OBJ)client.o

# client main function
cmain.o: $(SRC)cmain.cpp $(SRC)server.h $(SRC)message.h
	g++ -c $(SRC)cmain.cpp -o $(OBJ)cmain.o

# cleans the object & binaries
clean:
	rm -rf $(OBJ)*.o $(BIN)*
