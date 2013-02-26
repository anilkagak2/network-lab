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


Test
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


Class Description
	client		backbone for the client architecture of im
	server		backbone for the server

Bugs

TODOs
