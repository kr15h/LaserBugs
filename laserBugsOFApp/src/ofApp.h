#pragma once

#include "ofMain.h"
#include "Background.h"

namespace laserbugs{

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
	
		shared_ptr<Background> _background;
};

} // namespace laserbugs
