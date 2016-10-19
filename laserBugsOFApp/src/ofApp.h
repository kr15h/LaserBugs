#pragma once

#include "ofMain.h"
#include "ofxPiMapper.h"
#include "SharedData.h"
#include "Background.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
	
		void keyPressed(int key);
	
		ofxPiMapper mapper;
	
		shared_ptr<laserbugs::SharedData> sharedData;
	
		laserbugs::Background background;
};
