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
#include "GameScene.h"
#include "GameInterface.h"
#include "PConnect.h"
//end

using namespace std;

//------- DECLARATIONS
GameScene *gs;
PConnect *con;
GameInterface *gi;
//------- END DEC'S

int main(int argc, char* argv[]) {
	
	CGFapplication app = CGFapplication();

	gs = new GameScene();
	gi = new GameInterface(gs);

	try {

		con = new PConnect();
		gs->con = con;
		if(!con->socket_connect()){
			throw exception();
		}

		app.init(&argc, argv);
		app.setScene(gs);
		app.setInterface(gi);
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