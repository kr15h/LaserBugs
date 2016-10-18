#include "ofMain.h"

namespace laserbugs{

class Background{
	public:
		Background();
	
		void setup();
		void update();
		void draw();
	
		void calcGrid();
	
	private:
		unsigned int _numCols;
		unsigned int _numRows;
	
		float _colWidth;
		float _rowHeight;
};

} // namespace laserbugs
