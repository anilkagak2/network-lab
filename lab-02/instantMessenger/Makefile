#
# Makefile:	Rules to make client & server along with
#		their backends
# 
# Anil Kag
# IIT Guwahati
# a.kag@iitg.ernet.in
#


# Server
# compilation of backend + main
im-serverD: serverD.o smain.o
	g++ serverD.o smain.o -o im-serverD 

# as per the assignment server should be named "ims"
ims: server.o smain.o
	g++ server.o smain.o -o ims 

# server backend
serverD.o: server.cpp server.h message.h
	g++ -c server.cpp -o serverD.o -D_DEBUG_

server.o: server.cpp server.h message.h
	g++ -c server.cpp -o server.o 

# server main function
smain.o: smain.cpp server.h message.h
	g++ -c smain.cpp -o smain.o


# 
# Client
# compilation of backend + main
im-clientD: clientD.o cmain.o
	g++ clientD.o cmain.o -o im-clientD

# as per the assignment client should be named "imc"
imc: client.o cmain.o
	g++ client.o cmain.o -o imc

# client backend
clientD.o: client.cpp client.h message.h
	g++ -c client.cpp -o clientD.o -D_DEBUG_

client.o: client.cpp client.h message.h
	g++ -c client.cpp -o client.o

# client main function
cmain.o: cmain.cpp server.h message.h
	g++ -c cmain.cpp -o cmain.o

# cleans the object & binaries
clean:
	rm -rf *.o im-serverD im-clientD ims imc
