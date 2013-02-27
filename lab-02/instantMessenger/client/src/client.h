/*
 * client.h:	Header containing structure & function declarations
 *		for client
 * Anil Kag
 * a.kag@iitg.ernet.in
 */

#include "../../include/message.h"
#include <iostream>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

using namespace std;

/* Client Class. */
class Client {
	public:
		/* Registers user at host:port. */
		Client 	(char *user, char *host, char *port);

		/* Deregisters the user. */
		~Client ();

		/* Main Engine. */
	void	start ();

		/* CTRL+C Handler. */
	void 	sig_handler (int signum);

	private:
	int			client_socket;  		// client UDP socket
	struct 	sockaddr_in 	server;				// server address structure
	char			user[NAME_MAX_LEN];		// user name
	char			host[NAME_MAX_LEN];		// server name
	bool 			registered;			// is user registered?

//	sig_atomic_t		stopFlag;			// atomic variable for signal handling
//	void			set_sigaction ();		// sets up the signal handling struct

	bool			deregister ();
	void			recv_message ();		// recvfrom server
	void 			send_message (string line);	// send message to server

	inline string &		ltrim(string &s);		// trim from start
	inline string &		rtrim(string &s);		// trim from end
	inline string &		trim(string &s);		// trim from both ends
};
