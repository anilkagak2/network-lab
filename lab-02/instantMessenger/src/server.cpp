/*
 * server.cpp:	Definitions of server class function
 *	
 * Anil Kag
 * a.kag@iitg.ernet.in
 */

#include "server.h"

#define _PORT_ 5555

/* Initialize the server parameters. */
Server::Server () {
	master_socket = -1;	// Initial value
	port = _PORT_;		// Port number
}

/* Release the resources occupied (if any) by server. */
Server::~Server () {
	// close the master socket if it's valid
	if (master_socket != -1) close (master_socket);
}

/* Allocates master socket & binds it to the IP address. */
bool 
Server::setup_server () {
	struct sockaddr_in localhost;
	int sock_len = sizeof (localhost);

	// Create UDP socket
	// Best error --> ever found :P 
	//if ( (master_socket = (PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
	if ( (master_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror ("Server: Create Master Socket");
		return false;		// couldn't create master socket
	}

	cout << "Master socket " << master_socket << endl;
	// clear the struct
	memset (&localhost, 0, sizeof (localhost));

	// fill in the required fields
	localhost.sin_family = AF_INET;
	localhost.sin_addr.s_addr = htonl(INADDR_ANY);
//	inet_aton ("localhost", &localhost.sin_addr);
	localhost.sin_port = htons (port);

	// bind the socket to an address:port
	if (bind (master_socket, (struct sockaddr *) &localhost, sizeof (localhost)) < 0) {
		// Error occurred while binding to localhost
		close (master_socket);
		perror ("Server: Bind to localhost");
		return false;
	}

	char host[1024], service[1024];
	int rv = getnameinfo ((struct sockaddr *) &localhost, sizeof localhost, host, sizeof host, service, sizeof service, 0);
	if (rv != 0) perror ("Getnameinfo failed");
	else cout << "host " << host << " service " << service << endl;

	cout << "Server: Bind Complete\n";
	return true;
}


/* Main Engine.
 * Coordinates all the required setup, sending & receiving from users
 */
void
Server::start () {
	// client address structure (to be recieved)
	struct sockaddr_in client;
	socklen_t client_len = sizeof (struct sockaddr_storage);

	// message structure
	im_message msg;
	size_t msg_len = sizeof (msg);

	cout << "Server: Starting... \n";

	// Setup server
	if (!setup_server ()) {
		cerr << "Server: Setup Failed\n ";
		return;
	}

	cout << "Server: Start Successful\n";

	// receive message from users
	while (1) {
		int len = 0;
		memset (&msg, 0, sizeof (msg));

		len = recvfrom (master_socket, &msg, msg_len, 0,
			(struct sockaddr *) &client, &client_len);

		if (len == -1) continue;
		else if (len != msg_len) {
			// Some problem in the message sent
			cerr << "Some problem occurred in sending the message\n";
		}
		
		else {
			char host[NI_MAXHOST], service[NI_MAXSERV];
			int s = getnameinfo ((struct sockaddr *) &client, client_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);

			if (s == 0)
				printf ("Received %d bytes from %s:%s\n",
						len, host, service);
			else 
				fprintf (stderr, "getaddrnameinfo: %s\n", gai_strerror (s));

			// check the type of message received
			if (msg.type == REGISTRATION_MESSAGE) {
				// register the user
				if (!register_user (&msg, &client)) 
					cerr << "Server: cannot register the user " << msg.from << endl;
				else 	cout << "Server: registered user " << msg.from << endl;
			}

			else if (msg.type == DEREGISTRATION_MESSAGE) {
				// deregister the user
				if (!deregister_user (&msg, &client)) 
					cerr << "Server: cannot deregister the user " << msg.from << endl;
				else 	cout << "Server: deregistered user " << msg.from << endl;
			}

			else {
				// instant message
				if (!send_instant_msg (&msg, &client)) {
					cerr << "Server: cannot send message from " << msg.from;
			       		cerr << "to " << msg.to << endl;
				} else {
					cout << "Server:  message sent from " << msg.from;
			       		cout << "to " << msg.to << endl;
				}
			 }
		}
	}
}

/* Registration & Deregistration. */
bool
Server::register_user (struct im_message *msg, struct sockaddr_in *client) {
	// search for it in the list
	list<user>::iterator it;
	for (it = users.begin (); it != users.end (); it++) {
		// already registered 
		if (strncmp (it->user_name, msg->from, NAME_MAX_LEN) == 0) {
			// check the address, if changed then update it
			struct sockaddr_in *s = &it->user_addr;
			if ((s->sin_addr.s_addr != client->sin_addr.s_addr) ||
			   (it->user_addr.sin_port != client->sin_port)) {
			       	it->user_addr.sin_addr.s_addr = client->sin_addr.s_addr;
				it->user_addr.sin_port = client->sin_port;
			} // PLEASE CHECK WHETHER THE TABLE IS REALLY CHANGED HERE.. ITS A VALID DOUBT

			return true;
		}
	}

	// prepare the new entry to be inserted in the list
	user unew;
	strncpy (unew.user_name, msg->from, NAME_MAX_LEN);
	memcpy (&unew.user_addr, client, sizeof (sockaddr_in));

	// Yet to think about the usage of list for maintaining the list of users
	users.push_back (unew);

	im_message ack_msg;
	string msg_content;
	socklen_t len = sizeof (struct sockaddr);

	msg_content = "User ";
	msg_content += msg->from;
	msg_content += " registered successfully\n";

	strncpy (ack_msg.to, msg->from, NAME_MAX_LEN);
	strncpy (ack_msg.from, "Server", NAME_MAX_LEN);
	strncpy (ack_msg.message, msg_content.c_str (), 1024);
	ack_msg.type = REGISTRATION_MESSAGE;

	if (sendto (master_socket, &ack_msg, sizeof (ack_msg), 0, (struct sockaddr *) &unew.user_addr,
				len) != sizeof (ack_msg)) 
		cerr << "Server: Error sending back \"REGISTRATION ACK\" message to " << msg->from << endl;

	return true;
}

/* De-registers a user by removing it's record from the users list. */
bool
Server::deregister_user	(struct im_message *msg, struct sockaddr_in *client) {
	// search for it in the list
	list<user>::iterator it;
	for (it = users.begin (); it != users.end (); it++) { 
		if (strncmp (it->user_name, msg->from, NAME_MAX_LEN) == 0) {
			// removing the user from table
			users.erase (it);
			cout << "Server: DeRegistered " << msg->from << endl;
			return true;
		}
	}

	return false;
}

/* Sending an IM Instant Message. */
bool
Server::send_instant_msg (struct im_message *msg, struct sockaddr_in *client) {
	// search for it in the list
	list<user>::iterator it;
	for (it = users.begin (); it != users.end (); it++) { 
		// Beware it should be msg->to not from :)
		if (strncmp (it->user_name, msg->to, NAME_MAX_LEN) == 0) {
			break;
		}
	}

	// receiver not found
	if (it == users.end ()) {
		im_message ack_msg;
		string msg_content;

		msg_content = "User ";
		msg_content += msg->to;
		msg_content += " not registered";

		strncpy (ack_msg.to, msg->from, NAME_MAX_LEN);
		strncpy (ack_msg.from, "Server", NAME_MAX_LEN);
		strncpy (ack_msg.message, msg_content.c_str (), 1024);

		if (sendto (master_socket, &ack_msg, sizeof (ack_msg), 0, (struct sockaddr *) client,
					sizeof (*client)) != sizeof (ack_msg)) 
			cerr << "Server: Error sending back \"NOT REGISTERED\" ack message to " << msg->from << endl;
		return false;
	}

	else {
#ifdef _DEBUG_ 
		cout << "Server: going to send message to " << msg->to;
		cout << " From " << msg->from << "Message " << msg->message << endl;
		cout << "it->user_name is " << it->user_name << endl;
#endif
		struct sockaddr_in sin = it->user_addr;
		// sizeof (msg) = 4 & sizeof (*msg) = actual message size
		if (sendto (master_socket, msg, sizeof (*msg), 0, (struct sockaddr *) &sin,
					sizeof (sin)) != sizeof (*msg)) {
			cerr << "Server: Error sending message to " << msg->to << endl;
			return false;
		}
#ifdef _DEBUG_ 
		cout << "message successfully sent to " << msg->to << " from " << msg->from << endl;
		cout << "message was " << msg->message << endl;
#endif
		return true;
	}
}

/* TODO: How to fit this into the class requirement. */
/* Searches user in the table. */
/*user*
  Server::search_user (const char *user_name) {
  if (users.empty ()) return NULL;

  list<user>::iterator it;
  for (it = users.begin (); it != users.end (); it++) {
  if (strncmp (it->user_name, user_name, NAME_MAX_LEN) == 0) return it;
  }
  }
 */
