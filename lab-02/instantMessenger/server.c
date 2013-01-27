/*
 * server.c:	Definitions of server class function
 *	
 * Anil Kag
 * a.kag@iitg.ernet.in
 */

#include "server.h"

Server::Server () {
	master_socket = -1;	// Initial value
	port = 5555;		// Port number
}

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
	if ( (master_socket = (PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		perror ("Server: Create Master Socket");
		return false;		// couldn't create master socket
	}

	// clear the struct
	memset (&localhost, 0, sizeof (localhost));

	// fill in the required fields
	localhost.sin_family = AF_INET;
	localhost.sin_addr.s_addr = htonl (INADDR_ANY);
	localhost.sin_port = htons (port);

	// bind the socket to an address:port
	if (bind (master_socket, (struct sockaddr *) &localhost, sock_len) == -1) {
		// Error occurred while binding to localhost
		close (master_socket);
		perror ("Server: Bind to localhost");
		return false;
	}

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
	int client_len = sizeof (client);

	// message structure
	im_message msg;
	int msg_len = sizeof (msg);

	// Setup server
	if (!setup_server ()) {
		cerr << "Server: Setup Failed\n ";
		return;
	}

	// receive messages from users
	while (1) {
		int len = 0;
		len = recvfrom (master_socket, msg, msg_len, 0,
			(struct sockaddr *) &client, client_len);

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
			if (msg.im_message_type == REGISTRATION_MESSAGE) {
				// register the user
				if (!register_user (msg, &client)) 
					cerr << "Server: cannot register the user " << msg.from << endl;
				else 	cout << "Server: registered user " << msg.from << endl;
			}

			else if (msg.im_message_type == DEREGISTRATION_MESSAGE) {
				// deregister the user
				if (!deregister_user (msg, &client)) 
					cerr << "Server: cannot deregister the user " << msg.from << endl;
				else 	cout << "Server: deregistered user " << msg.from << endl;
			}

			else {
				// instant message
				if (!send_instant_msg (msg, &client)) 
					cerr << "Server: cannot send message from " << msg.from;
			       		cerr << "to " << msg.to << endl;
				else 	cout << "Server:  message sent from " << msg.from;
			       		cout << "to " << msg.to << endl;
			 }
		}
	}
}

/* Registration & Deregistration. */
bool
Server::register_user (struct im_message *msg, struct sockaddr_in *client) {
	// search for it in the list
	list<user>::iterator it;
	for (it = users.begin (); it != end (); it++) {
		// already registered 
		if (strncmp (*it->user_name, msg->from, NAME_MAX_LEN) == 0) {
			// check the address, if changed then update it
			if ((*it->user_addr.sin_addr != client->sin_addr) ||
			   (*it->user_addr.sin_port != client->sin_port)) {
			       	*it->user_addr.sin_addr = client->sin_addr;
				*it->user_addr.sin_port = client->sin_port;
			}

			return true;
		}
	}

	// prepare the new entry to be inserted in the list
	user new;
	strncpy (new.user_name, msg->from, NAME_MAX_LEN);
	memcpy (&new.user_addr, msg->from, sizeof (msg->user_name));

	// Yet to think about the usage of list for maintaining the list of users
	users.push_back (new);
	return true;
}

bool
Server::deregister_user	(struct im_message *msg, struct sockaddr_in *client) {
	// search for it in the list
	list<user>::iterator it;
	for (it = users.begin (); it != end (); it++) { 
		if (strncmp (*it->user_name, msg->from, NAME_MAX_LEN) == 0) {
			// removing the user from table
			users.erase (it);
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
	for (it = users.begin (); it != end (); it++) { 
		if (strncmp (*it->user_name, msg->from, NAME_MAX_LEN) == 0) {
			break;
		}
	}

	// receiver not found
	if (it == users.end ()) {
		im_message ack_msg;
		string msg_content;

		msg_content = "User ";
		msg_content += msg.to;
		msg_content += " not registered";

		strncpy (ack_msg->to, msg->from, NAME_MAX_LEN);
		strncpy (ack_msg->from, "Server", NAME_MAX_LEN);
		strncpy (ack_msg->msg, msg_content.c_str (), 1024);

		if (sendto (master_socket, ack_msg, sizeof (ack_msg), 0, (struct sockaddr *) client,
					sizeof (*client)) != sizeof (ack_msg)) 
			cerr << "Server: Error sending back \"NOT REGISTERED\" ack message to " << msg.from << endl;
		return false;
	}

	else {
		struct sockaddr_in sin = *it->user_addr;
		if (sendto (master_socket, msg, sizeof (msg), 0, (struct sockaddr *) &sin,
					sizeof (sin)) != sizeof (msg)) {
			cerr << "Server: Error sending message to " << msg.to << endl;
			return false;
		}

		return true;
	}
}

/* TODO: How to fit this into the class requirement. */
/* Searches user in the table. */
user*
Server::search_user (const char *user_name) {
	if (users.empty ()) return NULL;

	list<user>::iterator it;
	for (it = users.begin (); it != end (); it++) {
		if (strncmp (*it->user_name, user_name, NAME_MAX_LEN) == 0) return it;
	}
}

