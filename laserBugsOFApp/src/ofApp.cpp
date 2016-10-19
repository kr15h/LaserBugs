#include "ofApp.h"

namespace laserbugs{

void ofApp::setup(){
	cout << "LaserBugs says hello!" << endl;
	
	mapper.registerFboSource(background);
	mapper.setup();
}

void ofApp::update(){
	mapper.update();
}

void ofApp::draw(){
	mapper.draw();
}

} // namespace laserbugs
