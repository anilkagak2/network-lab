#include "client.h"

int 
main (int argc, char *argv[]) {
	if (argc < 4) {
		cout << "Wrong number of arguments\n";
		cout << "./" << argv[0] << " user_name server_addr port" << endl;
		return 1;
	}

	Client client(argv[1], argv[2], argv[3]);
	client.start ();

	return 0;
}
