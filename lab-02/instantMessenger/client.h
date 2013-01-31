/*
 * client.h:	Header containing structure & function declarations
 *		for client
 * Anil Kag
 * a.kag@iitg.ernet.in
 */

#include "message.h"
#include <iostream>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <inetdb.h>
#include <unistd.h>

#define NAME_MAX_LEN 	256
#define MESSAGE_MAX_LEN 1024

/* Client Class. */
class Client {
	public:
		/* Registers user at host:port. */
		Client 	(char *user, char *host, char *port);

		/* Deregisters the user. */
		~Client ();

		/* Main Engine. */
	void	start ();

	private:
	int			client_socket;  	// client UDP socket
	struct 	sockaddr_in 	server;			// server address structure
	char			user[NAME_MAX_LEN];	// user name
	char			host[NAME_MAX_LEN];	// server name

	bool	deregister ();
	void	recv_message ();	// recvfrom server
	void 	send_message ();	// send message to server
};
