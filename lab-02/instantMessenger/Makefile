#
# Makefile:	Rules to make client & server along with
#		their backends
#	
# Anil Kag
# a.kag@iitg.ernet.in
#


# Server
# compilation of backend + main
im-serverD: serverD.o smain.o
	g++ serverD.o smain.o -o im-serverD 

im-server: server.o smain.o
	g++ server.o smain.o -o im-server 

# server backend
serverD.o: server.cpp server.h message.h
	g++ -c server.cpp -o serverD.o -D_DEBUG_

server.o: server.cpp server.h message.h
	g++ -c server.cpp -o server.o 

# main function
smain.o: smain.cpp server.h message.h
	g++ -c smain.cpp -o smain.o


# 
# Client
# compilation of backend + main
im-clientD: clientD.o cmain.o
	g++ clientD.o cmain.o -o im-clientD

im-client: client.o cmain.o
	g++ client.o cmain.o -o im-client

# client backend
clientD.o: client.cpp client.h message.h
	g++ -c client.cpp -o clientD.o -D_DEBUG_

client.o: client.cpp client.h message.h
	g++ -c client.cpp -o client.o

# main function
cmain.o: cmain.cpp server.h message.h
	g++ -c cmain.cpp -o cmain.o

clean:
	rm -rf *.o im-server im-client
