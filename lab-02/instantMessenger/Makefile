#
# Makefile:	Rules to make client & server along with
#		their backends
# 
# Anil Kag
# a.kag@iitg.ernet.in
#

SSRC=server/src/
SBIN=server/bin/
SOBJ=server/obj/

CSRC=client/src/
CBIN=client/bin/
COBJ=client/obj/

INC=include/message.h

# Make all default
all: im-serverD im-clientD ims imc ims-free-port

# Server
# compilation of backend + main
im-serverD: serverD.o smain.o
	g++ $(SOBJ)serverD.o $(SOBJ)smain.o -o $(SBIN)im-serverD 

# as per the assignment server should be named "ims"
ims: server.o smain.o
	g++ $(SOBJ)server.o $(SOBJ)smain.o -o $(SBIN)ims 

# "ims-free-port" picks an available free port for the server
ims-free-port: serverFP.o smain.o
	g++ $(SOBJ)serverFP.o $(SOBJ)smain.o -o $(SBIN)ims-free-port -D_AUTO_PICK_PORT_

# server backend for free port one
serverFP.o: $(SSRC)server.cpp $(SSRC)server.h $(INC)
	g++ -c $(SSRC)server.cpp -o $(SOBJ)serverFP.o -D_AUTO_PICK_PORT_

# server backend
serverD.o: $(SSRC)server.cpp $(SSRC)server.h $(INC)
	g++ -c $(SSRC)server.cpp -o $(SOBJ)serverD.o -D_DEBUG_

server.o: $(SSRC)server.cpp $(SSRC)server.h $(INC)
	g++ -c $(SSRC)server.cpp -o $(SOBJ)server.o 

# server main function
smain.o: $(SSRC)smain.cpp $(SSRC)server.h $(INC)
	g++ -c $(SSRC)smain.cpp -o $(SOBJ)smain.o


# 
# Client
# compilation of backend + main
im-clientD: clientD.o cmain.o
	g++ $(COBJ)clientD.o $(COBJ)cmain.o -o $(CBIN)im-clientD

# as per the assignment client should be named "imc"
imc: client.o cmain.o
	g++ $(COBJ)client.o $(COBJ)cmain.o -o $(CBIN)imc

# client backend
clientD.o: $(CSRC)client.cpp $(CSRC)client.h $(INC)
	g++ -c $(CSRC)client.cpp -o $(COBJ)clientD.o -D_DEBUG_

client.o: $(CSRC)client.cpp $(CSRC)client.h $(INC)
	g++ -c $(CSRC)client.cpp -o $(COBJ)client.o

# client main function
cmain.o: $(CSRC)cmain.cpp $(CSRC)client.h $(INC)
	g++ -c $(CSRC)cmain.cpp -o $(COBJ)cmain.o

# cleans the object & binaries
clean:
	rm -rf $(SOBJ)*.o $(COBJ)*.o $(SBIN)* $(CBIN)*
