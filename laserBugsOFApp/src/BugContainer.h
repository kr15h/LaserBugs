#pragma once

#include "ofMain.h"
#include "FboSource.h"
#include "SharedData.h"
#include "Bug.h"

namespace laserbugs{

class BugContainer : public ofx::piMapper::FboSource{
	public:
		BugContainer(shared_ptr<SharedData> sd);
	
		void setup();
		void update();
		void draw();
		void createBugs();
		void start();
		void stop();
		void blinkTimeSlider();
	
	private:
		shared_ptr<SharedData> _sharedData;
	
		vector<Bug *> _bugs;
	
		bool _startFlag;
	
		int _loopTime;
		int _blinkTime;
		int _laserRange;
};

} // namespace laserbugs
