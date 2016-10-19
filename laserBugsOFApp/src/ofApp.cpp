#include "ofApp.h"

void ofApp::setup(){
	cout << "LaserBugs says hello!" << endl;
	
	ofBackground(0);
	
	sharedData = make_shared<laserbugs::SharedData>();
	background.setSharedData(sharedData);
	
	mapper.registerFboSource(background);
	mapper.setup();
}

void ofApp::update(){
	mapper.update();
}

void ofApp::draw(){
	mapper.draw();
}
