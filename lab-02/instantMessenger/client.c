/*
 * client.c:	Function definitions for client
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
	
	/* Initialize the socket. */
	if ( (client_socket = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1 ) {
		perror ("Client: Create Socket ");
		return;
	}

	// May be put in DEBUG #defines
	cout << "Client: Welcome " << user << endl;
	cout << "Client: Socket creation successful\n";

	/* Setup the server's address struct. */
	// clear the struct
	memset (&server, 0, sizeof (server));

	// fill in the required fields
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr (host);
	server.sin_port = htons (atoi (port));

	cout << "Client: Server address structure filled successully\n";

	/* Send Registration message to server. */
	im_message msg;
	int msg_len = sizeof (msg);

	// populate the message
	msg.im_message_type = REGISTRATION_MESSAGE;
	strncpy (msg.from, user, NAME_MEX_LEN);

	// send the registration message to server
	if (sendto (client_socket, msg, msg_len, 0, 
		(struct sockaddr *) &server, sizeof (server)) != msg_len) {
		perror ("Client: Registration Failed");
		return;
	}

	cout << "Client: Registered " << user << "at " << host << endl;

	// TODO:: if required use some kind of registration acknowledgement in response to 
	// client's registration message
}

bool
Client::deregister () {
	if (client_socket == -1) return false;

	/* Send Registration message to server. */
	im_message msg;
	int msg_len = sizeof (msg);

	// populate the message
	msg.im_message_type = DEREGISTRATION_MESSAGE;
	strncpy (msg.from, user, NAME_MEX_LEN);

	// send the registration message to server
	if (sendto (client_socket, msg, msg_len, 0, 
		(struct sockaddr *) &server, sizeof (server)) != msg_len) {
		perror ("Client: Registration Failed");
		return false;
	}

	cout << "Client: De-registered " << user << "at " << host << endl;	
	return true;
}

/* TODO:: Fit deregistration into this. */
/* Deregisters the user. */
Client::~Client () {
	// close the socket if it's valid
	if (client_socket != -1) close (client_socket);
}

/* Main Engine. */
void
Client::start () {
	cout << "Client:: Start successful\n";

	fd_set readfds;
	int rval;

	while (1) {
		// display instant messenger prompt
		cout << "imc> " ;

		/* Create set of file descriptors stdin(0) & client_socket. */
		FD_CLR(client_socket, &readfds);
		FD_CLR(0, &readfds);
		FD_ZERO(&readfds);
		FD_SET(client_socket, &readfds);
		FD_SET(0, &readfds);

		/* We now use select to wait for input at either location. */
		/* We don't need to timeout, so we leave that part NULL.   */
		rval = select(client_socket+1, &readfds, NULL, NULL, NULL);

		// TODO:: Handling the recvfrom in start function but need to shift
		// it's functionality to some stand-alone function
		if ((rval == 1) && (FD_ISSET(client_socket, &readfds))) {
			/* We have socket input waiting, and can now do a recvfrom(). */
			recv_message () 
		}

		if ((rval == 1) && (FD_ISSET(0, &readfds))) {
			/* We have input waiting on standard input, so we can go get it. */
			if (!send_message ()) {
				cerr << "Client: Cannot send message\n";
			}

			string line;
			cin >> line;

			int pos = 0;
			pos = line.find_first_not_of (" \t");
			if (pos != string::npos) line = line.substr (0, pos+1);

			// EXIT
			if (!strncmp (line.c_str (), "exit", 4)) {
				deregister ();
				break;
			}

			else {
				send_message (line) 
			}
		}

		if (rval == -1 && errno == EINTR) continue;

		if (rval == -1) {
			perror ("Client: Select()");
			return;
		}
	}
}

void
Client::recv_message () {
	im_message msg;

	if (recvfrom (client_socket, msg, sizeof (msg), 0, (struct sockaddr *) &server,
					sizeof (server)) != sizeof (msg)) {
			cerr << "Client: Error receiving message to " << msg.to << endl;
		//	return false;
			return;
	}

	cout << msg.from << ": " << msg.message << endl;
}

void
Client::send_message (string line) {
	int pos = line.find_first_of (':');
	if (pos == string::npos) {
		cerr << "No message embedded" << endl;
		return;
		//return false;
	}

	string to = line.substr (0, pos);
	string payload = line.substr (pos+1);

	cout << "TO: " << to << endl;
	cout << "MESG: " << payload <<  endl;

	if (payload.length () > MESSAGE_MAX_LEN-1 ) 
		cerr << "Truncating the message to length " << MESSAGE_MAX_LEN-1 << endl;

	im_message msg;
	msg.im_message_type = INSTANT_MESSAGE;
	strncpy (msg.from, user, NAME_MAX_LEN);
	strncpy (msg.to, to.c_str (), NAME_MAX_LEN);
	strncpy (msg.message, payload.c_str (), MESSAGE_MAX_LEN-1);
	msg.message[payload.length ()] = '\0';

	if (sendto (client_socket, msg, sizeof (msg), 0, (struct sockaddr *) &server,
					sizeof (server)) != sizeof (msg)) {
			cerr << "Client: Error sending message to " << msg.to << endl;
		//	return false;
			return;
	}
}
