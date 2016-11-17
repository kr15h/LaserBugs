#include "BugContainer.h"

namespace laserbugs{

BugContainer::BugContainer(){
	name = "Bugs Container";

	_sharedData = 0;
	
	_startFlag = false;
	
	_loopTime = 3000;
	_blinkTime = 200;
	_laserRange = 300;
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
}

void BugContainer::update(){
	// TODO: pass shared data in constructor and this won't be needed
	if(_sharedData == 0){
		ofLogError("Background::setup", "Please provide SharedData");
		return;
	}
	
	if(!_startFlag){
		return;
	}
	
	for(unsigned int i = 0; i < _bugs.size(); ++i){
		_bugs[i]->react(_bugs);
		_bugs[i]->collision(_bugs);
		_bugs[i]->selfblinking();
		_bugs[i]->edgeDetection();
		_bugs[i]->movement();
		_bugs[i]->update();
	}
}

void BugContainer::draw(){
	ofClear(0, 0, 0, 0);

	ofPushStyle();
	
	ofSetLineWidth(4);
	ofSetColor(255);
	ofNoFill();
	ofDrawRectangle(10, 10, getWidth() - 20, getHeight() - 20);
	
	ofSetLineWidth(2);
	ofDrawLine(10, 10, getWidth() - 20, getHeight() - 20);
	ofDrawLine(getWidth() - 20, 10, 10, getHeight() - 20);
	
	for(int i = 0; i < _bugs.size(); ++i){
		_bugs[i]->draw();
	}
	
	ofPopStyle();
}

void BugContainer::start(){
	_startFlag = true;
}

void BugContainer::stop(){
	_startFlag = false;
}

void BugContainer::createBugs(){
	_bugs.clear();
	float xInterval =
		(float)_sharedData->getAppWidth() /
		(float)_sharedData->getBugCols() + 1.0f;
	float yInterval =
		(float)_sharedData->getAppHeight() /
		(float)_sharedData->getBugRows() + 1.0f;
		
	for (int i = 0; i < _sharedData->getBugRows(); i++) {
		for (int j = 0; j < _sharedData->getBugCols(); j++) {
			float xpos = ((float)j + 0.5f) * xInterval;
			float ypos = ((float)i + 0.5f) * yInterval;
			ofLogNotice("BugContainer::createBugs")
				<< "xpos: " << xpos
				<< "ypos: " << ypos;
				
			float rotateSpeed = 0.04f;
			float moveSpeed = 0.4f;
			float r = ofRandom(1.0f);
			// println(r);
			// int eachloopTime = int(random(loopTime*range, loopTime));
			int eachloopTime;
				
			if(r < 0.95f) {
				eachloopTime = _loopTime;
			}else{
				// eachloopTime = loopTime/4;
				eachloopTime = _loopTime;
			}
			// int loopTime = 1000;
			int shiftTime = (int)ofRandom((float)_loopTime);
			// float blinkTime = 50; // in milli seconds.
			Bug * b = new Bug(
				ofPoint(xpos, ypos),
				rotateSpeed,
				moveSpeed,
				eachloopTime,
				shiftTime,
				_blinkTime,
				_laserRange);
				
			_bugs.push_back(b);
		}
	}
}

void BugContainer::blinkTimeSlider(){
	for(int i = 0; i < _bugs.size(); ++i){
		_bugs[i]->setBlinkTime(_blinkTime);
	}
}

void BugContainer::setSharedData(shared_ptr<SharedData> sd){
	_sharedData = sd;
}

} // namespace laserbugs
