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

//MAC OSX specific includes
#ifdef __APPLE__
#include <GL/glew.h>
#include <GL/glui.h>
#include "CGFapplication.h"
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
PickScene *ps;
PConnect *con;
PickInterface *pi;
//------- END DEC'S

int main(int argc, char* argv[]) {
	
	CGFapplication app = CGFapplication();

	ps = new PickScene();
	pi = new PickInterface(ps);

	try {

#if _WIN32

		con = new PConnect();
		ps->con = con;
		if(!con->socket_connect()){
			throw exception();
		}
#endif

		app.init(&argc, argv);
		app.setScene(ps);
		app.setInterface(pi);
		app.run();
	}
	catch(GLexception& ex) {
		con->quit();
		cout << "Erro: " << ex.what();
		cin.get();
		return -1;
	}
	catch(exception& ex) {
		con->quit();
		cout << "Erro inesperado: " << ex.what();
		cin.get();
		return -1;
	}
	
	con->quit();
	return 0;
}