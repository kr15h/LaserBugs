#pragma once

#include "ofMain.h"
#include "SharedData.h"

namespace laserbugs{

class Bug{
	public:
		Bug(
			ofPoint position,
			float rotationSpeed,
			float movementSpeed,
			int eachLoopTime,
			int shiftTime,
			int blinkTime,
			int laserRange);
	
		//(new Rotator(xpos, ypos, rotateSpeed, moveSpeed, eachloopTime, shiftTime, blinkTime, laserRange)
	
		void setup();
		void update();
		void draw();
	
		void setSharedData(shared_ptr<SharedData> sd);
	
	private:
		shared_ptr<SharedData> _sharedData;
	
		ofPoint _position;
	
		float _rotationSpeed;
		float _movementSpeed;
	
		int _eachLoopTime;
		int _shiftTime;
		int _blinkTime;
		int _laserRange;
	
};

} // namespace laserbugs
