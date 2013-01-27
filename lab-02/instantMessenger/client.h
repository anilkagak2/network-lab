/*
 * server.h:	Header containing structure & function declarations
 *		for server
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
#include <arpa/inet.h>
#include <inetdb.h>
#include <unistd.h>

#define NAME_MAX_LEN 256

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
		/* Sets up the connection. */
	bool 	connect ();
};
