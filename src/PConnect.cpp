#include "PConnect.h"
#include <cstdio>
#include <iostream>

using namespace std;

PConnect::PConnect():connected(false){
	this->port = 60070;
	this->address = "127.0.0.1";
}

PConnect::~PConnect(){
    
#ifdef _WIN32
	if(connect) quit();
#else
	if(connected){
		close(fd);
	}
#endif
}

void PConnect::calculatePlay(string player, string board, int &xi, int &yi, int &xf, int &yf, string &newBoard)
{
	if( !connect )
		return;
	string s;
    
	s= "calculate(" + player + ", " + board + ").\n";
    
	send_message((char*)s.c_str(),s.length());
    
	char ans[500];
	receive_message(ans);
    
	//Verificar se resposta valida
	if( !validAnswer(ans) )
	{
		xi=0;
		yi=0;
		xf=0;
		yf=0;
		newBoard="invalid";
		return;
	}
    
	xi = ans[3]-48;
	yi = ans[5]-48;
	xf = ans[7]-48;
	yf = ans[9]-48;
	xi--;
	yi--;
	xf--;
	yf--;
    
	newBoard = parse_board(ans,11);
}

bool PConnect::gameEnd(string board){
	if( !connect )
		return false;
    
	string msg;
    
	msg = "game_end(" + board + ").\n";
	send_message((char*)msg.c_str(),msg.length());
	char ans[500];
	receive_message(ans);
    
	if( validAnswer(ans) )
		return true;
    
	return false;
}

void PConnect::quit()
{
	if( !connected )
		return;
	cout << "Asking prolog to quit" << endl;
	char buff[] = "bye.\n";
	send_message(buff, 6);
	char ans[128];
	receive_message(ans);
	connected = false;
#ifndef _WIN32
	close(fd);
#endif
}

bool PConnect::socket_connect(){
#ifdef _WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)
		printf("Client: Error at WSAStartup().\n");
	else
		printf("Client: WSAStartup() is OK.\n");
    
	// Create a socket.
	opened_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (opened_socket == INVALID_SOCKET) {
		printf("Client: socket() - Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return false;
	}
	else
        printf("Client: socket() is OK.\n");
    
	// Connect to a server.
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	// Just test using the localhost, you can try other IP address
	clientService.sin_addr.s_addr = inet_addr(address);
	clientService.sin_port = htons(port);
    
	if (connect(opened_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		printf("Client: connect() - Failed to connect.\n");
		WSACleanup();
		return false;
	}
	else {
        printf("Client: connect() is OK.\n");
        printf("Client: Can start sending and receiving data...\n");
	}
    
	// Send and receive data.
	printf("Connected\n");
	connected= true;
	return true;
#else
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(this->address);
	server_addr.sin_port = htons(this->port);
	fd= socket(AF_INET, SOCK_STREAM, 0);
	if(fd<0){
        printf("ERROR Creating socket\n");
        exit(-1);
	}
	if(connect(fd,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        
		this->connected=false;
        printf("Connection error\n");
        exit(-1);
    }else{
		this->connected=true;
        printf("Client: connect() is ok\n");
        return true;
    }
	return false;
    
    
#endif
    
}


void PConnect::send_message(char *msg, int len){
#ifndef _WIN32
	size_t sent=write(this->fd, msg, len+1);
	if(sent==0 && len!=0){
		printf("Client: send() error .\n" );
	}
#else
	int bytesSent = send(opened_socket, msg, len, 0);
	if(bytesSent == SOCKET_ERROR)
		printf("Client: send() error %ld.\n", WSAGetLastError());
#endif
}

void PConnect::receive_message(char *msg) {
#ifndef _WIN32
	int pos=0;
	char c;
	while(read(fd,&c,1)>0){
		if(c=='\n'){
			break;
		}
		msg[pos]=c;
		pos++;
	}
	msg[pos]=0;
#else
	int bytesRecv = SOCKET_ERROR;
	int pos = 0;
	while (true) {
		recv(opened_socket, &msg[pos], 1, 0);
		if (msg[pos] == '\n')
			break;
		pos++;
	}
	msg[pos] = 0;
#endif
}

bool PConnect::play(int xi, int yi, int xf, int yf, string board, string &newBoard)
{
	if( !connected )
		return false;
    
	string snd_msg("execute(");
	snd_msg+= to_string(xi+1);
	snd_msg+= "-" + to_string(yi+1);
	snd_msg+= "," + to_string(xf+1);
	snd_msg+= "-" + to_string(yf+1);
	snd_msg+= "," + board + ").\n";
    
	send_message((char*)snd_msg.c_str(),snd_msg.length());
	char ans[500];
	receive_message(ans);
    
	if( validAnswer(ans) )
	{
		newBoard = parse_board(ans,3);
		return true;
	}
    
	return false;
}

string PConnect::get_init_board(){
	if( !connected )
		return "invalid";
    
	send_message("initialize.\n",12);
    
	char ans[350];
	receive_message(ans);
    
	//Verifica se resposta valida
    
	if( !validAnswer(ans) )
		return "invalid";
    
	return parse_board(ans,3);
}

bool PConnect::validAnswer(char *ans)
{
	//Verifica se 'ok'
	if( ans[0] == 'o' && ans[1] == 'k' )
		return true;
    
	return false;
}

string PConnect::parse_board(char *ans,int pos){
	char board[350];
	unsigned int j=0;
    
#ifndef _WIN32
	for(unsigned int i=pos; i < strlen(ans) -2; i++,j++)
		board[j]=ans[i];
	
    
	board[j] = '\0';
#else
	for(unsigned int i=pos; i < strlen(ans) -3; i++,j++)
		board[j]=ans[i];
	
	
	board[j] = '\0';
#endif
	return string(board);
}
