im-server: server.o smain.o
	g++ server.o smain.o -o im-server

server.o: server.cpp server.h message.h
	g++ -c server.cpp -o server.o

smain.o: smain.cpp server.h message.h
	g++ -c smain.cpp -o smain.o

im-client: client.o cmain.o
	g++ client.o cmain.o -o im-client

client.o: client.cpp client.h message.h
	g++ -c client.cpp -o client.o

cmain.o: cmain.cpp server.h message.h
	g++ -c cmain.cpp -o cmain.o

clean:
	rm -rf *.o im-server im-client
