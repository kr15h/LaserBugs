#pragma once

#include "ofxOsc.h"

namespace laserbugs{
	
class SharedData{
	public:
		SharedData(){
			debug = false;

			_appWidth = 1920;
			_appHeight = 1080;
			_bugCols = 8;
			_bugRows = 5;
			_moduleSize = 5; // Size of one bug? Half diameter
			_laserRange = 300;
			
			_turnAmount = 2.416f;
		
			_oscSender = make_shared<ofxOscSender>();
			_oscSender->setup("127.0.0.1", 5678);
		}
	
		bool debug;
	
		unsigned int getAppWidth();
		unsigned int getAppHeight();
		unsigned int getBugCols();
		unsigned int getBugRows();
		unsigned int getModuleSize();
		unsigned int getLaserRange();
	
		float getTurnAmount();
	
		shared_ptr<ofxOscSender> getOscSender();
	
	private:
		unsigned int _appWidth;
		unsigned int _appHeight;
		unsigned int _bugCols;
		unsigned int _bugRows;
		unsigned int _moduleSize;
		unsigned int _laserRange;
	
		float _turnAmount;
	
		shared_ptr<ofxOscSender> _oscSender;
};

} // namespace laserbugs
