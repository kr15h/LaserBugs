#pragma once

#include "ofMain.h"
#include "ofxPiMapper.h"
#include "Background.h"

namespace laserbugs{

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
	
		ofxPiMapper mapper;
	
		laserbugs::Background background;
};

} // namespace laserbugs
