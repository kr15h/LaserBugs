#pragma once

#include "ofMain.h"
#include "FboSource.h"
#include "SharedData.h"

//#define BACKGROUND_BRIGHTNESS 0

namespace laserbugs{

class BugsContainer : public ofx::piMapper::FboSource{
	public:
		BugsContainer();
	
		void setup();
		void update();
		void draw();
	
		void setSharedData(shared_ptr<SharedData> sd);
	
	private:
		shared_ptr<SharedData> _sharedData;
};

} // namespace laserbugs
