#include "ofApp.h"

namespace laserbugs{

void ofApp::setup(){
	cout << "LaserBugs says hello!" << endl;
	
	_background = make_shared<Background>();
	_background->setup();
}

void ofApp::update(){
	_background->update();
}

void ofApp::draw(){
	_background->draw();
}

} // namespace laserbugs
