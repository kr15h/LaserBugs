#pragma once

#include "ofxOsc.h"

#define SHARED_DATA_APP_WIDTH 1920
#define SHARED_DATA_APP_HEIGHT 1080

#define SHARED_DATA_BUG_COLS 7
#define SHARED_DATA_BUG_ROWS 7

namespace laserbugs{
	
class SharedData{
	public:
		SharedData(){
			debug = false;

			_appWidth = SHARED_DATA_APP_WIDTH;
			_appHeight = SHARED_DATA_APP_HEIGHT;
			_bugCols = SHARED_DATA_BUG_COLS;
			_bugRows = SHARED_DATA_BUG_ROWS;
			
			_turnAmount = 2.416f;
		
			_oscSender = make_shared<ofxOscSender>();
			_oscSender->setup("127.0.0.1", 5678);
		}
	
		bool debug;
	
		unsigned int getAppWidth();
		unsigned int getAppHeight();
		unsigned int getBugCols();
		unsigned int getBugRows();
	
		float getTurnAmount();
	
		shared_ptr<ofxOscSender> getOscSender();
	
	private:
		unsigned int _appWidth;
		unsigned int _appHeight;
		unsigned int _bugCols;
		unsigned int _bugRows;
	
		float _turnAmount;
	
		shared_ptr<ofxOscSender> _oscSender;
};

} // namespace laserbugs
