/*
 * smain.cpp:	Main routine that uses the client class instance for im-server
 *
 * Anil Kag
 * a.kag@iitg.ernet.in
 */

#include "server.h"

int 
main (int argc, char *argv[]) {
	Server server;
	server.start ();

	return 0;
}
