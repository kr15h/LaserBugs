#pragma once

#include "ofMain.h"
#include "FboSource.h"
#include "SharedData.h"
#include "Bug.h"

namespace laserbugs{

class BugContainer : public ofx::piMapper::FboSource{
	public:
		BugContainer();
	
		void setup();
		void update();
		void draw();
	
		void setSharedData(shared_ptr<SharedData> sd);
	
	private:
		shared_ptr<SharedData> _sharedData;
	
		vector<Bug *> _bugs;
};

} // namespace laserbugs
