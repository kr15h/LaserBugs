#include "SharedData.h"

namespace laserbugs{

SharedData::SharedData(){
	debug = false;
	
	appWidth = 900;
	appHeight = 600;
	bugCols = 7;
	bugRows = 5;
	moduleSize = 5; // Size of one bug? Half diameter
	laserRange = 300;
	
	turnAmount = 2.5f;
	bugSize = 7.0f;
	
	oscSender = make_shared<ofxOscSender>();
	oscSender->setup("127.0.0.1", 5678);
}

} // namespace laserbugs
