#include "ofMain.h"
#include "ofApp.h"
#include "SharedData.h"

int main(){
	shared_ptr<laserbugs::SharedData> shared =
		make_shared<laserbugs::SharedData>();

	ofSetupOpenGL(1024, 768, OF_WINDOW);
	ofRunApp(new ofApp(shared));
}
