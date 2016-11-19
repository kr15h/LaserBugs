#include "ofApp.h"

ofApp::ofApp(shared_ptr<laserbugs::SharedData> sd){
	_sharedData = sd;
}

void ofApp::setup(){
	ofBackground(0);

	background = make_shared<laserbugs::Background>(_sharedData);
	container = make_shared<laserbugs::BugContainer>(_sharedData);
	
	mapper.registerFboSource(background.get());
	mapper.registerFboSource(container.get());
	mapper.setup();
	
	container->createBugs();
	container->start();
	
	ofSetFullscreen(true);
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
		container->stop();
		container->createBugs();
		container->start();
	}else if(key == 'f'){
		ofToggleFullscreen();
	}
}
