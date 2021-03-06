#pragma once

#include "ofMain.h"
#include "ofxPiMapper.h"
#include "SharedData.h"
#include "Background.h"
#include "BugContainer.h"

class ofApp : public ofBaseApp{
	public:
		ofApp(shared_ptr<laserbugs::SharedData> sd);
	
		void setup();
		void update();
		void draw();
	
		void keyPressed(int key);
	
		ofxPiMapper mapper;
	
		shared_ptr<laserbugs::Background> background;
		shared_ptr<laserbugs::BugContainer> container;
	
	private:
		shared_ptr<laserbugs::SharedData> _sharedData;
};
