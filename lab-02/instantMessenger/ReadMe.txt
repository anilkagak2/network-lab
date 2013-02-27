/*
 * IM-Ak2 1.0
 * ReadMe.txt	: General Overview of the whole IM-Architecture
 *
 * Anil Kag
 * a.kag@iitg.ernet.in
 */

Introduction
	This is a general instant messenger which deals with text messages only.
	Other data embedding like pictures, files etc. are not available in this
	version of IM-Ak2.

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
	    |   |-- ims			Simple IM-server (PORT 5555)
	    |   |-- im-serverD		Same server but this time with more debugging involved
	    |   `-- im-free-port	Picks free port to bind on its own
	    |-- obj
	    |   |-- serverD.o
	    |   |-- server.o
	    |   `-- smain.o
	    |-- server.mk
	    `-- src			Source code for server
		|-- server.cpp
		|-- server.h
		`-- smain.cpp

Assumptions
	- Server runs on a fixed port 5555
	  If the port is in use open server/server.cpp & change the value in #define _PORT_
	  to some appropriate value above 1024
	OR
	  Use ims-free-port  instead of ims & change the port used by client to the ones
	  returned by server. ims-free-port leaves upto OS to pick an available free port

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

		imc> exit
		(Exits the im client program after de-registering from im server)

	@T3
		imc> exit
	@T1
		(Press CTRL+C so that subsequent calls to ims will be able to reuse
		 the port currently in use otherwise they may fail at bind step)

	Type the name strictly without any trailing spaces between name & ':'
	If there are some trailing spaces, the server might not find the user in the 
	registered users list & hence send back a not registered user message.


Class Description
	client		backbone for the client architecture of im
	server		backbone for the server

Bugs
 - 	When two users, both with same name tries to register themselves on
	server, the last one will be able to succeed.
	[Because of the problem description only. Read 3rd point in the im-server description]

TODOs or Enhancements
 -*	Currently the server maintains the user registration table in 
	the form of a list. It can be a problem when the table size 
	grows aribitrarily large
     -> Sol: Use map abstract data type instead of list. This will improve the search time.

 -DONE 	Trailing spaces before & after the user name in "imc>" prompt 
	till the ':' can be a problem for now. Write check for it.

 -* 	Right now only server IP (in dotted decimal notation) will work,
	you can use "localhost" (it's an exception).
     -> Sol: Use getaddrinfo (), it can deal with IP in dotted decimal as well as
	host names.

 -* 	Make sure the server is running before any user sends any message.
     -> Sol: TIMEOUT value for the REGISTRATION ACK (currently in use)

 -*	Handling the CTRL+C signal in client because in this case de-register
	is really required
     -> Sol: add signal handler for SIGINT (CTRL+C) in client main function & when signal
	occurs, quit the client after de-registering from the server

* --> Can be done but that's an overhead for the simplicity of the problem.
