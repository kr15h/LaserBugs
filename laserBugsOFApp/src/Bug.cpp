#include "Bug.h"

namespace laserbugs{

Bug::Bug(
	ofPoint position,
	float rotationSpeed,
	float movementSpeed,
	int eachLoopTime,
	int shiftTime,
	int blinkTime,
	int laserRange){
	
	_sharedData = 0;
	
	_position = position;
	_rotationSpeed = rotationSpeed;
	_movementSpeed = movementSpeed;
	_eachLoopTime = eachLoopTime;
	_shiftTime = shiftTime;
	_blinkTime = blinkTime;
	_laserRange = laserRange;
}

void Bug::setup(){
	if(_sharedData == 0){
		ofLogError("Bug::setup", "Please provide SharedData");
		return;
	}
}

void Bug::update(){
	if(_sharedData == 0){
		ofLogError("Bug::setup", "Please provide SharedData");
		return;
	}
}

void Bug::draw(){
	if(_sharedData == 0){
		ofLogError("Bug::setup", "Please provide SharedData");
		return;
	}
	
	ofPushStyle();
	ofSetColor(0, 255, 255);
	ofDrawCircle(_position.x, _position.y, 5);
	ofPopStyle();
}

void Bug::setSharedData(shared_ptr<SharedData> sd){
	_sharedData = sd;
}

} // namespace laserbugs
