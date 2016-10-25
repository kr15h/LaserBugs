#include "BugContainer.h"

namespace laserbugs{

BugContainer::BugContainer(){
	name = "Bugs Container";

	_sharedData = 0;
}

void BugContainer::setup(){
	if(_sharedData == 0){
		ofLogError("Background::setup", "Please provide SharedData");
		return;
	}

	allocate(_sharedData->getAppWidth(), _sharedData->getAppHeight());
}

void BugContainer::update(){
	if(_sharedData == 0){
		ofLogError("Background::setup", "Please provide SharedData");
		return;
	}
}

void BugContainer::draw(){
	if(_sharedData == 0){
		ofLogError("Background::setup", "Please provide SharedData");
		return;
	}
	
	ofPushStyle();
	
	ofSetLineWidth(4);
	ofSetColor(255);
	ofNoFill();
	ofDrawRectangle(10, 10, getWidth() - 20, getHeight() - 20);
	
	ofSetLineWidth(2);
	ofDrawLine(10, 10, getWidth() - 20, getHeight() - 20);
	ofDrawLine(getWidth() - 20, 10, 10, getHeight() - 20);
	
	ofPopStyle();
}

void BugContainer::setSharedData(shared_ptr<SharedData> sd){
	_sharedData = sd;
}

} // namespace laserbugs
