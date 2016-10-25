#pragma once

#define SHARED_DATA_APP_WIDTH 1920
#define SHARED_DATA_APP_HEIGHT 1080

#define SHARED_DATA_BUG_COLS 7
#define SHARED_DATA_BUG_ROWS 7

namespace laserbugs{
	
class SharedData{
	public:
		SharedData();
	
		bool debug;
	
		unsigned int getAppWidth();
		unsigned int getAppHeight();
		unsigned int getBugCols();
		unsigned int getBugRows();
	
	private:
		unsigned int _appWidth;
		unsigned int _appHeight;
		unsigned int _bugCols;
		unsigned int _bugRows;
};

} // namespace laserbugs
