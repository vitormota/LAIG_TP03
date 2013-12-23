/*
--------------------------
----- LAIG TP 03 ---------
--------------------------
*/

//Windows specific includes
#if _WIN32
#include <GL\glew.h>
#include <GL\glui.h>
#include <CGFapplication.h>
#endif
//end

//general includes (c++ standard)
#include <iostream>

//project includes
#include "PickScene.h"
#include "PickInterface.h"
#include "PConnect.h"
//end

using namespace std;

//------- DECLARATIONS
PickScene ps;
PConnect *con;
//------- END DEC'S

int main(int argc, char* argv[]) {
	
	CGFapplication app = CGFapplication();

	try {

		con = new PConnect();
		con->socket_connect();

		app.init(&argc, argv);
		
		app.setScene(&ps);
		app.setInterface(new PickInterface());
		
		app.run();
	}
	catch(GLexception& ex) {
		con->quit();
		cout << "Erro: " << ex.what();
		return -1;
	}
	catch(exception& ex) {
		con->quit();
		cout << "Erro inesperado: " << ex.what();
		return -1;
	}
	
	return 0;
}