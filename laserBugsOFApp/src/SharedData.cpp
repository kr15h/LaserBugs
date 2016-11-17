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

unsigned int SharedData::getModuleSize(){
	return _moduleSize;
}

unsigned int SharedData::getLaserRange(){
	return _laserRange;
}

float SharedData::getTurnAmount(){
	return 2.416f;
}

shared_ptr<ofxOscSender> SharedData::getOscSender(){
	return _oscSender;
}

} // namespace laserbugs
