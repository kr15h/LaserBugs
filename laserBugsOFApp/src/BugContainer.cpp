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
	
	// Create bugs
	float intervalX =
		(float)_sharedData->getAppWidth() /
		((float)_sharedData->getBugCols() + 1.0f);
	float intervalY =
		(float)_sharedData->getAppHeight() /
		((float)_sharedData->getBugRows() + 1.0f);
	for(unsigned int col = 0; col < _sharedData->getBugCols(); ++col){
		for(unsigned int row = 0; row < _sharedData->getBugRows(); ++row){
			ofPoint position;
			position.x = ((float)col + 1.0f) * intervalX;
			position.y = ((float)row + 1.0f) * intervalY;
			
			// TODO: set remaining variables
			Bug * b = new Bug(position, 0.4f, 0.4f, 100, 10, 1000, 100);
			b->setSharedData(_sharedData);
			_bugs.push_back(b);
		}
	}
	
	/*
	Bug(
			ofPoint position,
			float rotationSpeed,
			float movementSpeed,
			int eachLoopTime,
			int shiftTime,
			int blinkTime,
			int laserRange);
	*/
}

void BugContainer::update(){
	if(_sharedData == 0){
		ofLogError("Background::setup", "Please provide SharedData");
		return;
	}
	
	for(unsigned int i = 0; i < _bugs.size(); ++i){
		_bugs[i]->update();
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
	
	for(unsigned int i = 0; i < _bugs.size(); ++i){
		_bugs[i]->draw();
	}
}

void BugContainer::setSharedData(shared_ptr<SharedData> sd){
	_sharedData = sd;
}

} // namespace laserbugs
