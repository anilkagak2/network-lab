Building
	First of all change directory to the place where you extracted the source code
	$ make clean
	$ make

Source
	.
	|-- client			Contains client side of the IM (Instant Messenger)
	|   |-- bin			Binaries 
	|   |   |-- imc				No debugging symbols (less couts)
	|   |   `-- im-clientD			Debugging enabled
	|   |-- client.mk
	|   |-- obj			Object Files
	|   |   |-- clientD.o
	|   |   |-- client.o
	|   |   `-- cmain.o
	|   `-- src			Source code for client
	|       |-- client.cpp
	|       |-- client.h
	|       `-- cmain.cpp
	|-- include			Message type declaration header shared by client & server
	|   `-- message.h
	|-- Makefile			Makefile for both the server & client
	|-- overview.txt
	|-- ReadMe.txt			ReadMe file  for the IM
	`-- server			Contains server side of the IM (Instant Messenger)
	    |-- bin
	    |   |-- ims
	    |   `-- im-serverD
	    |-- obj
	    |   |-- serverD.o
	    |   |-- server.o
	    |   `-- smain.o
	    |-- server.mk
	    `-- src
		|-- server.cpp
		|-- server.h
		`-- smain.cpp


Steps to run the server & client
	Port used by server is 5555 (make sure the port is free)
	Open three terminals T1, T2, T3
	@T1
		$ cd server/bin
		$ ./ims	
		wait till "Server successfully started comes"
	@T2
		$ cd client/bin
		$ ./imc anil localhost 5555
		imc> prompt will appear after invoking this command
	@T3
		$ cd client/bin
		$ ./imc rahul localhost 5555

	@T2
		imc> rahul: hi.. what's up?
		(rahul will get this message as forwarded by server)

	Type the name strictly without any trailing spaces between name & ':'
	If there are some trailing spaces, the server might not find the user in the 
	registered users list & hence send back a not registered user message.


Class Description
	client		backbone for the client architecture of im
	server		backbone for the server

Bugs
 - 	When two users both with same name tries to register themselves on
	server, the last one will be able to succeed.

TODOs or Enhancements
 -	Currently the server maintains the user registration table in 
	the form of a list. It can be a problem when the table size 
	grows aribitrarily large
	Remedy: Use map instead of list. This will improve the search
	period.

 -DONE 	Trailing spaces before & after the user name in "imc>" prompt 
	till the ':' can be a problem for now. Write check for it.


 -* 	Right now only server IP (in dotted decimal notation) will work,
	you can use "localhost" (it's an exception).
	Remedy: Use getaddrinfo ()

 -* 	Make sure the server is running before any user sends any message.
	Remedy: TIMEOUT value for the REGISTRATION ACK (currently in use)

 -*	Handling the CTRL+C signal in client because in this case de-register
	is really required
