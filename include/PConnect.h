#ifndef PCONNECT_H
#define PCONNECT_H

#if _WIN32
#include <WinSock2.h>
#endif

#include <string>

class PConnect{

public:

	PConnect();
	~PConnect();
	
	bool socket_connect();
	bool play(int xi, int yi, int xf, int yf, std::string board, std::string &newBoard);

	std::string get_init_board();

	void quit();

protected:

	int port;
	char* address;
	bool connected;
	SOCKET opened_socket;

	void send_message(char *msg, int len);
	void receive_message(char *msg);

	bool validAnswer(char *ans);
	std::string parse_board(char *ans,int pos);
};

#endif 