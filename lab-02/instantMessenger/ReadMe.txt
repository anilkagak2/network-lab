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
	This program has been tested on Linux 3.2.0-38.

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

	- im client starts only after im server is up & running [see Bugs for "why?"]

Steps to run the server & client
	Before coming to this you should build your binaries for IM.
	Port used by server is 5555 (make sure the port is free)
	Open three terminals T1, T2, T3 [Assuming $ is the prompt]
	@T1
		$ cd server/bin
		$ ./ims	
		(Output will contain Messages as the server starts &
		a final message "Server Started Successfully" tells that
		the server is up & running [wait till this point before typing
		anything on T2 or T3] )
	@T2
		$ cd client/bin
		$ ./imc anil localhost 5555
		(Output will contain welcome message for anil & the imc> prompt )
	@T3
		$ cd client/bin
		$ ./imc rahul localhost 5555
		(Output will contain welcome message for rahul & the imc> prompt )

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


Program Description
	client		client's main function is in cmain.cpp.
			It creates an instance of client class with arguments given by user 
			as commandline arguments & calls client's start method.

			Client, on creation initializes it's client_socket & initializes the
			server's address with the help of host & port given as argument by 
			the main function.

			After successful creation of the socket & initialization of server 
			address, client sends server a message of the type REGISTRATION_MESSAGE
			& server on receiving this message adds user's data in the users table
			& sends an acknowledgement to the user.

			When the start method is called, IM shows a welcome screen to the user
			& gives "imc>" prompt. User types in the messages in strict format
			Format being 
				receiver: message
			receiver is the name of the user with whom he/she wants to send the 
			message

			After receiving the message from user, it parses the message so as to
			extract the user_name & the message body & send it to the server using
			the server address filled in at the start time & again shows the prompt.

			When a message comes to the user, program prints the message to the screen
			in the format
				sender: message & gives back the prompt.
			This continues in a loop until user types "exit" on the prompt.
			On seeing "exit", program frees up the resources allocated to the client,
			de-registers the user & says "Good Bye".

	server		server's main function is in smain.cpp.
			It creates an instance of server class calls server's start method.

			Server, on creation initializes it's master_socket & binds itself to 
			the host on which it is running. Simple server binds itself to port 5555
			& ims-free-port finds out a free port & binds to that.

			After success of initialization phase of the server, it starts listening on 
			the master_socket. On receiving a message, it parses the message to handle
			these cases:
			1) REGISTRATION_MESSAGE
				update users table via including the user's name in it(if doesn't exists)
				otherwise update the address info

			2) DEREGISTRATION_MESSAGE
				deregister the user from the table(if it exists)
				if exists then remove only if the current ip:port matches with those
				available in the table [otherwise bug will creep in because of the problem
				discussed in Bugs]

			3) INSTANT_MESSAGE
				forwards the message to the receiver if he/she is in the table
				otherwise send back the sender an error message

			This continues in a loop until user types "exit" on the prompt.
			For closing the server, press CTRL+C (it'll free the resources allocated to
			the server)


Bugs
 - 	When two users, both with same name tries to register themselves on
	server, the last one will be able to succeed.
	[Because of the problem description only. Read 3rd point in the im-server description]
     -> Sol: At the time of registration, client waits for the server to send an acknowledgement
	if the user exists in the database, then server can send a negative reply & this user
	will exit the program or retry with another user name.
	[But implementing this would voilate the problem statement, which explicitly requires to 
	incorporate the user's new address into the table]

 - 	When client starts running before server starts, then the client keeps on sending the input
	taken from the user without even notifying that registration is not complete.
     -> Sol: Wait after sending the REGISTRATION_MESSAGE, for the ACK. If within MAXTIME the ACK 
	is received then continue the execution else raise TIMEOUT error & exit.
	This solution is implemented right now except for the fact that the code is commented (in 
	client constructor) & server's table management has to change a little bit i.e. raise an
	error if a user with existing name tries to register instead of updating the address in the
	table.



TODOs or Enhancements
 -*	Currently the server maintains the user registration table in 
	the form of a list. It can be a problem when the table size 
	grows aribitrarily large
     -> Sol: Use map abstract data type instead of list. This will improve the search time.

 -DONE 	Trailing spaces before & after the user name in "imc>" prompt 
	till the ':' can be a problem for now. Write check for it.

 -* 	Right now only server IP (in dotted decimal notation) will work,
	you can use "localhost" (it's an exception) at client end (to be given as host on cmd).
     -> Sol: Use getaddrinfo (), it can deal with IP in dotted decimal as well as
	host names.

 -* 	Make sure the server is running before any user sends any message.
     -> Sol: TIMEOUT value for the REGISTRATION ACK (currently in use)

 -*	Handling the CTRL+C signal in client because in this case de-register
	is really required, otherwise server will keep entries of those users too, which have
 	left without notifying the server
     -> Sol: add signal handler for SIGINT (CTRL+C) in client main function & when signal
	occurs, quit the client after de-registering from the server

NOTE: all -* --> Can be done but that's an overhead for the simplicity of the problem.


