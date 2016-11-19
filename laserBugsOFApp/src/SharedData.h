#pragma once

#include "ofxOsc.h"

namespace laserbugs{
	
class SharedData{
	public:
		SharedData();
	
		bool debug;
	
		int appWidth;
		int appHeight;
		int bugCols;
		int bugRows;
		int moduleSize;
		int laserRange;
	
		float turnAmount;
		float bugSize;
	
		shared_ptr<ofxOscSender> oscSender;
};

} // namespace laserbugs
