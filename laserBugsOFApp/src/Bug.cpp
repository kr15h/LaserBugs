#include "Bug.h"

namespace laserbugs{

Bug::Bug(){
	_sharedData = 0;
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
}

void Bug::setSharedData(shared_ptr<SharedData> sd){
	_sharedData = sd;
}

} // namespace laserbugs
