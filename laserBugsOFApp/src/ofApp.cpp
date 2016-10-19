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
	
	if(sharedData->debug){
		stringstream ss;
		ss << "fps: " << ofGetFrameRate();
		ofDrawBitmapStringHighlight(ss.str(), 10, 20, ofColor(0, 100));
	}
}

void ofApp::keyPressed(int key){
	if(key == ' '){
		sharedData->debug = !sharedData->debug;
	}
}
