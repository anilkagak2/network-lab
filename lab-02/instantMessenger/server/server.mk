#
# server.mk:	Rules to make server along with the backend
# 
# Anil Kag
# a.kag@iitg.ernet.in
#

SRC=src/
BIN=bin/
OBJ=obj/
HINC=../include/message.h

# Make all default
all: im-serverD ims

# Server
# compilation of backend + main
im-serverD: serverD.o smain.o
	g++ $(OBJ)serverD.o $(OBJ)smain.o -o $(BIN)im-serverD 

# as per the assignment server should be named "ims"
ims: server.o smain.o
	g++ $(OBJ)server.o $(OBJ)smain.o -o $(BIN)ims 

# server backend
serverD.o: $(SRC)server.cpp $(SRC)server.h $(HINC)message.h
	g++ -c $(SRC)server.cpp -o $(OBJ)serverD.o -D_DEBUG_

server.o: $(SRC)server.cpp $(SRC)server.h $(HINC)message.h
	g++ -c $(SRC)server.cpp -o $(OBJ)server.o 

# server main function
smain.o: $(SRC)smain.cpp $(SRC)server.h $(HINC)
	g++ -c $(SRC)smain.cpp -o $(OBJ)smain.o

# cleans the object & binaries
clean:
	rm -rf $(OBJ)*.o $(BIN)*
