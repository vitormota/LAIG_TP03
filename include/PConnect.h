#ifndef PCONNECT_H
#define PCONNECT_H

#if _WIN32
#include <WinSock2.h>
#endif

#ifdef __APPLE__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include <string>

class PConnect{

public:

	PConnect();
	~PConnect();

	bool socket_connect();
	bool play(int xi, int yi, int xf, int yf, std::string board, std::string &newBoard);
	void calculatePlay(std::string player, std::string board, int &xi, int &yi, int &xf, int &yf, std::string &newBoard);
	bool gameEnd(std::string board);
	std::string get_init_board();

	void quit();

protected:

	int port;
	char* address;
	bool connected;
#ifdef _WIN32
	SOCKET opened_socket;
#else
	int fd;
	struct sockaddr_in server_addr;
#endif
	void send_message(char *msg, int len);
	void receive_message(char *msg);

	bool validAnswer(char *ans);
	std::string parse_board(char *ans,int pos);
};

#endif
