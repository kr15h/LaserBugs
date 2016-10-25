#pragma once

#include "ofMain.h"
#include "SharedData.h"

namespace laserbugs{

class Bug{
	public:
		Bug();
	
		void setup();
		void update();
		void draw();
	
		void setSharedData(shared_ptr<SharedData> sd);
	
	private:
		shared_ptr<SharedData> _sharedData;
};

} // namespace laserbugs
