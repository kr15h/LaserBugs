#include "SharedData.h"

namespace laserbugs{

SharedData::SharedData(){
	debug = false;

	_appWidth = SHARED_DATA_APP_WIDTH;
	_appHeight = SHARED_DATA_APP_HEIGHT;
	_bugCols = SHARED_DATA_BUG_COLS;
	_bugRows = SHARED_DATA_BUG_ROWS;
}

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

} // namespace laserbugs
