#include "Background.h"

namespace laserbugs{

Background::Background(){
	_sharedData = 0;
	_numCols = 0;
	_numRows = 0;
	_colWidth = 0.0f;
	_rowHeight = 0.0f;
}

void Background::setup(){
	if(_sharedData == 0){
		ofLogError("Background::setup", "Please provide SharedData");
		return;
	}

	allocate(_sharedData->getAppWidth(), _sharedData->getAppHeight());
	
	ofTextureData texData;
	texData.width = _sharedData->getAppWidth();
	texData.height = _sharedData->getAppHeight();
	_gridTexture.allocate(texData);
	_cellNumbersTexture.allocate(texData);
	
	_numCols = BACKGROUND_INIT_COLS;
	_numRows = BACKGROUND_INIT_ROWS;
	
	calcGrid();
}

void Background::update(){
	if(_sharedData == 0){
		ofLogError("Background::setup", "Please provide SharedData");
		return;
	}
}

void Background::draw(){
	if(_sharedData == 0){
		ofLogError("Background::setup", "Please provide SharedData");
		return;
	}
	
	_gridTexture.draw(0, 0, getWidth(), getHeight());
	
	if(_sharedData->debug){
		_cellNumbersTexture.draw(0, 0, getWidth(), getHeight());
	}
}

void Background::calcGrid(){
	_colWidth = (float)getWidth() / (float)_numCols;
	_rowHeight = (float)getHeight() / (float)_numRows;
	
	drawGrid();
	drawCellNumbers();
}

void Background::drawGrid(){
	ofFbo fbo;
	fbo.allocate(_sharedData->getAppWidth(), _sharedData->getAppHeight());
	
	fbo.begin();
	ofClear(0);
	
	ofPushStyle();
	ofSetColor(0);
	ofDrawRectangle(0, 0, getWidth(), getHeight());
	for(float y = _rowHeight; y < getHeight(); y += _rowHeight){
		ofSetColor(BACKGROUND_LINE_BRIGHTNESS);
		ofDrawLine(0, y, getWidth(), y);
		for(float x = _colWidth; x < getWidth(); x += _colWidth){
			if(y == _rowHeight){
				ofSetColor(BACKGROUND_LINE_BRIGHTNESS);
				ofDrawLine(x, 0, x, getHeight());
			}
			ofSetColor(0);
			ofDrawCircle(x, y, BACKGROUND_DOT_SIZE + BACKGROUND_DOT_MARGIN);
			ofSetColor(BACKGROUND_DOT_BRIGHTNESS);
			ofDrawCircle(x, y, BACKGROUND_DOT_SIZE);
		}
	}
	ofPopStyle();
	
	fbo.end();
	
	_gridTexture = fbo.getTexture();
}

void Background::drawCellNumbers(){
	ofFbo fbo;
	fbo.allocate(_sharedData->getAppWidth(), _sharedData->getAppHeight());
	
	fbo.begin();
	ofClear(0);
	
	for(int y = 0; y < _numRows; ++y){
		for(int x = 0; x < _numCols; ++x){
			stringstream ss;
			ss << x << ", " << y;
			ofDrawBitmapString(
				ss.str(),
				(float)x * _colWidth + 10,
				(float)y * _rowHeight + 20);
		}
	}
	
	fbo.end();
	
	_cellNumbersTexture = fbo.getTexture();
}

void Background::setSharedData(shared_ptr<SharedData> sd){
	_sharedData = sd;
}

void Background::setNumCols(unsigned int cols){
	_numCols = cols;
	calcGrid();
}

void Background::setNumRows(unsigned int rows){
	_numRows = rows;
	calcGrid();
}

} // namespace laserbugs
