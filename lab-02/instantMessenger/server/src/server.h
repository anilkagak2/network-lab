/*
 * server.h:	Header containing structure & function declarations
 *		for server
 * Anil Kag
 * a.kag@iitg.ernet.in
 */

#include "../../include/message.h"
#include <iostream>
#include <list>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

typedef struct user {
	char user_name[256];		// user name
	struct sockaddr_in user_addr;	// user address structure
} user;

/* Server class */
class Server {
	public:
		Server ();
		~Server();

		/* Main Engine. */
	void	start ();

	private:
		int master_socket;	// master socket descriptor
		int port;		// port at which server will bind

		/* Users Table. */
		list<user> users;

		/* Registration & Deregistration. */
	bool 	register_user 		(struct im_message *msg, struct sockaddr_in *client);
	bool 	deregister_user 	(struct im_message *msg, struct sockaddr_in *client);

		/* Sending an IM Instant Message. */
	bool	send_instant_msg	(struct im_message *msg, struct sockaddr_in *client);

		/* Searches user in the table. */
//	user*	search_user 		(const char *user_name);
			// instant message

		/* Sets up the connection. */
	bool	setup_server ();
};
