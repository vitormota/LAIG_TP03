/*
--------------------------
----- LAIG TP 03 ---------
--------------------------
*/

//Windows specific includes
#if _WIN32
#include <CGFapplication.h>
#endif
//end

//general includes (c++ standard)
#include <iostream>

//project includes
#include "GameScene.h"
//end

using namespace std;

//------- DECLARATIONS
GameScene gs;
//------- END DEC'S

int main(int argc, char* argv[]) {
	
	CGFapplication app = CGFapplication();
	
	try {
		app.init(&argc, argv);
		
		app.setScene(&gs);
		app.setInterface(new CGFinterface());
		
		app.run();
	}
	catch(GLexception& ex) {
		cout << "Erro: " << ex.what();
		return -1;
	}
	catch(exception& ex) {
		cout << "Erro inesperado: " << ex.what();
		return -1;
	}
	
	return 0;
}