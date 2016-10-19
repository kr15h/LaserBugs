#pragma once

#define SHARED_DATA_APP_WIDTH 1920
#define SHARED_DATA_APP_HEIGHT 1080

namespace laserbugs{
	
class SharedData{
	public:
		SharedData();
	
		unsigned int getAppWidth();
		unsigned int getAppHeight();
	
	private:
		unsigned int _appWidth;
		unsigned int _appHeight;
};

} // namespace laserbugs
