#include "SharedData.h"

namespace laserbugs{

SharedData::SharedData(){
	debug = false;

	_appWidth = SHARED_DATA_APP_WIDTH;
	_appHeight = SHARED_DATA_APP_HEIGHT;
}

unsigned int SharedData::getAppWidth(){
	return _appWidth;
}

unsigned int SharedData::getAppHeight(){
	return _appHeight;
}

} // namespace laserbugs
