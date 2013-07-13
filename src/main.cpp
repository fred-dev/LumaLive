#include "ofMain.h"
#include "testApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(600,400, OF_FULLSCREEN);			// <-------- setup the GL context
	ofRunApp(new testApp);
}
