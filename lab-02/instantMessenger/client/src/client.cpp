/*
 * IM-Ak2 1.0
 * client.cpp:	Function definitions for client
 *
 * Anil Kag
 * a.kag@iitg.ernet.in
 */

#include "client.h"

/* Registers user at host:port. */
Client::Client (char *cuser, char *chost, char *port) {
	client_socket = -1;	// Initial value of socket
	strncpy (user, cuser, NAME_MAX_LEN);
	strncpy (host, chost, NAME_MAX_LEN);
	
	/* Initiiaze the signal handler. */
//	set_sigaction ();

	/* Initialize the socket. */
	if ( (client_socket = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1 ) {
		perror ("Client: Create Socket ");
		return;
	}

	// May be put in DEBUG #defines
#ifdef _DEBUG_
	cout << "Client: Welcome " << user << endl;
	cout << "Client: Socket creation successful\n";
#endif

	/* Setup the server's address struct. */
	// clear the struct
	memset (&server, 0, sizeof (server));

	// fill in the required fields
	server.sin_family = AF_INET;
//	server.sin_addr.s_addr = inet_addr (host);
	inet_aton (host, &server.sin_addr);
	server.sin_port = htons (atoi (port));

#ifdef _DEBUG_
	cout << "Client: Server address structure filled successully\n";
	cout << "Client: server=" << host << " port=" << port << endl;
#endif

	/* Send Registration message to server. */
	im_message msg;
	int msg_len = sizeof (msg);

	// populate the message
	msg.type = REGISTRATION_MESSAGE;
	strncpy (msg.from, user, NAME_MAX_LEN);

	// send the registration message to server
	int ret_chars = sendto (client_socket, &msg, msg_len, 0, 
		(struct sockaddr *) &server, sizeof (server));

	if (ret_chars == -1) {
		perror ("Client: Registration Message cannot be sent");
		return;
	}

#ifdef _DEBUG_ 
	cout << "Client: registration message length " << msg_len ;
	cout << " chars sent = " << ret_chars << endl;
#endif

	// Registration acknowledgement in response to 
	// client's registration message
/*	memset (&msg, 0, sizeof (msg));
	socklen_t len;

	fd_set socks;
	struct timeval t;

	// Create set of file descriptors stdin(0) & client_socket. 
	// this CLR is really required.. no clue why though.. :P
	FD_CLR(client_socket, &socks);
	FD_ZERO(&socks);
	FD_SET(client_socket, &socks);
	t.tv_sec = 20;	// wait for 20 seconds
	t.tv_usec = 0;	// wait for 0 micro-seconds
	int ret = select(client_socket + 1, &socks, NULL, NULL, &t);
	if (ret < 0) {
		perror ("select in constructor failed\n");
		exit (1);
	}
	else if (ret)
	{
		if (recvfrom (client_socket, &msg, sizeof (msg), 0, (struct sockaddr *) &server,
					&len) < 0) {
			perror ("Client: No registration acknowledgment received");
			exit(1);
		}
		if (msg.type == REGISTRATION_MESSAGE) {
			cout << "Client: Registered " << user << " at " << host << endl;
			cout << msg.from << ": " << msg.message << endl;
			//		cout.flush ();
			return;
		}
	} else {
		cout << "Client: Registration NOT ACKNOWLEDGED by Server & TIMEOUT occurred" << endl;
		exit (1);
	} 
*/
	// set registration flag
	registered = true;
//	cout << "came to end.. DONT know about the server's registration ack" << endl;
}

/*
// CTRL+C Handler. De-registers the user
void
Client::sig_handler (int signum) {
	stopFlag = 1;
}

// sets up the signal handling struct
void
Client::set_sigaction () {
	struct sigaction sigAct;
	memset(&sigAct, 0, sizeof(sigAct));
	sigAct.sa_handler = sig_handler;
	sigaction(SIGINT, &sigAct, 0);
}*/

/* Sends DEREGISTRATION message to server. */
bool
Client::deregister () {
	if (client_socket == -1) return false;

	/* Send Registration message to server. */
	im_message msg;
	int msg_len = sizeof (msg);

	// populate the message
	msg.type = DEREGISTRATION_MESSAGE;
	strncpy (msg.from, user, NAME_MAX_LEN);

	// send the registration message to server
	if (sendto (client_socket, &msg, msg_len, 0, 
				(struct sockaddr *) &server, sizeof (server)) != msg_len) {
		perror ("Client: De-Registration Failed");
		return false;
	}
#ifdef _DEBUG_
	cout << "Client: De-registered " << user << " at " << host << endl;
#endif
	// clear the registered flag
	registered = false;
	return true;
}

/* Deregisters the user. */
Client::~Client () {
	if (registered) {
		int rv = deregister ();
#ifdef _DEBUG_
		if (rv)		 	cout << "De-Registration Successful" << endl;
		else 			cout << "De-Registration Failed" << endl;
#endif
	}

	// close the socket if it's valid
	if (client_socket != -1) close (client_socket);
	cout << "Good Bye " << user << " :) "<< endl;
}

// trim from start
inline string &
Client::ltrim(string &s) {
	s.erase (s.begin (), find_if (s.begin (), s.end (), not1 (ptr_fun<int, int> (isspace)) ) );
	return s;
}

// trim from end
inline string &
Client::rtrim(string &s) {
	s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
	return s;
}

// trim from both ends
inline string &
Client::trim(string &s) {
	return ltrim(rtrim(s));
}	


/* Main Engine. */
void
Client::start () {
#ifdef _DEBUG_
	cout << "Client: Start successful\n";
	cout.flush ();
#endif

	fd_set readfds;
	int rval;

	cout << "\tIM-Ak2 1.0 " << endl;
	cout << "\tAuthor : Anil Kag " << endl;
	cout << "\tWelcome " << user << " :) " << endl;
	while (1) {
		// display instant messenger prompt
		//cout << "imc> " << endl;
		cout << "imc> " ;
		cout.flush ();

		/* Create set of file descriptors stdin(0) & client_socket. */
		FD_CLR(client_socket, &readfds);
		FD_CLR(0, &readfds);
		FD_ZERO(&readfds);
		FD_SET(client_socket, &readfds);
		FD_SET(0, &readfds);

		/* We now use select to wait for input at either location. */
		/* We don't need to timeout, so we leave that part NULL.   */
		rval = select(client_socket+1, &readfds, NULL, NULL, NULL);

		if ((rval == 1) && (FD_ISSET(client_socket, &readfds))) {
#ifdef _DEBUG_ 
			cout << "Client: going to receive message" << endl;
#endif
			/* We have socket input waiting, and can now do a recvfrom(). */
			recv_message (); 
		}

		if ((rval == 1) && (FD_ISSET(0, &readfds))) {
			/* We have input waiting on standard input, so we can go get it. */
#ifdef _DEBUG_ 
			cout << "Client: going to send message" << endl;
#endif
			string line;
			getline (cin, line);

#ifdef _DEBUG_ 
			cout << "Client: Message is --> " << line << endl;
#endif

			// HOUSE KEEPING THINGS
#ifdef _DEBUG_ 
			cout << "line before trimming: " << line << endl;
#endif
			line = trim (line);
#ifdef _DEBUG_
			cout << "line after trimming: " << line << endl;
#endif

			// EXIT
			if (!strncmp (line.c_str (), "exit", 4)) {
				deregister ();
				break;
			}

			else {
				send_message (line); 
			}
		}

		if (rval == -1) {
			perror ("Client: Select()");
			return;
		}
	}
}

/* Receives message from the server & decodes it. */
void
Client::recv_message () {
	im_message msg;
	socklen_t len;

	memset (&msg, 0, sizeof (msg));
	if (recvfrom (client_socket, &msg, sizeof (msg), 0, (struct sockaddr *) &server,
				//&len) != sizeof (msg)) {
		&len) < 0) {
			cerr << "Client: Error receiving message from " << msg.from << endl;
			//	return false;
			return;
		}

#ifdef _DEBUG_
	cout << "Type of message is " << msg.type << " is addresses to " << msg.to << endl;
#endif
	cout << msg.from << ": " << msg.message << endl;
}

/* Parses the message given by user.
 * Extracts the receiver's name & forms the instant message in order to sent to the server.
 */
void
Client::send_message (string line) {
	int pos = line.find_first_of (':');
	if (pos == string::npos) {
#ifdef _DEBUG_
		cerr << "No message embedded" << endl;
#endif
		return;
	}

	string to = line.substr (0, pos);
	string payload = line.substr (pos+1);

	// remove trailing spaces from the user name
	to = rtrim (to);

#ifdef _DEBUG_
	cout << "TO: " << to << endl;
	cout << "MESG: " << payload <<  endl;
#endif

	if (payload.length () > MESSAGE_MAX_LEN-1 ) 
		cerr << "Truncating the message to length " << MESSAGE_MAX_LEN-1 << endl;

	im_message msg;
	memset (&msg, 0, sizeof (msg));

	msg.type = INSTANT_MESSAGE;
	strncpy (msg.from, user, NAME_MAX_LEN);
	strncpy (msg.to, to.c_str (), NAME_MAX_LEN);
	strncpy (msg.message, payload.c_str (), MESSAGE_MAX_LEN-1);
	msg.message[payload.length ()] = '\0';

	if (sendto (client_socket, &msg, sizeof (msg), 0, (struct sockaddr *) &server,
				sizeof (server)) != sizeof (msg)) {
		cerr << "Client: Error sending message to " << msg.to << endl;
		return;
	}
}

