#include "ofApp.h"

ofApp::ofApp(shared_ptr<laserbugs::SharedData> sd){
	_sharedData = sd;
}

void ofApp::setup(){
	ofBackground(0);

	background.setSharedData(_sharedData);
	container.setSharedData(_sharedData);
	
	mapper.registerFboSource(background);
	mapper.registerFboSource(container);
	mapper.setup();
	
	container.createBugs();
	container.start();
}

void ofApp::update(){
	mapper.update();
}

void ofApp::draw(){
	mapper.draw();
	
	if(_sharedData->debug){
		stringstream ss;
		ss << "fps: " << ofGetFrameRate();
		ofDrawBitmapStringHighlight(ss.str(), 10, 20, ofColor(0, 100));
	}
}

void ofApp::keyPressed(int key){
	if(key == ' '){
		_sharedData->debug = !_sharedData->debug;
	}else if(key == 'l'){
		container.stop();
		container.createBugs();
		container.start();
	}
}
