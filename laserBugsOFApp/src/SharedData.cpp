#pragma once

#include "SharedData.h"

namespace laserbugs{

unsigned int SharedData::getAppWidth(){
	return _appWidth;
}

unsigned int SharedData::getAppHeight(){
	return _appHeight;
}

unsigned int SharedData::getBugCols(){
	return _bugCols;
}

unsigned int SharedData::getBugRows(){
	return _bugRows;
}

float SharedData::getTurnAmount(){
	return _turnAmount;
}

shared_ptr<ofxOscSender> SharedData::getOscSender(){
	return _oscSender;
}

} // namespace laserbugs
