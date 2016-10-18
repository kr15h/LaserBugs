#include "Background.h"

namespace laserbugs{

Background::Background(){
	_numCols = 0;
	_numRows = 0;
	_colWidth = 0.0f;
	_rowHeight = 0.0f;
}

void Background::setup(){
	ofBackground(0);
	_numCols = 10;
	_numRows = 8;
	calcGrid();
}

void Background::update(){
	calcGrid();
}

void Background::draw(){
	ofPushStyle();
	ofSetColor(255);
	for(float y = _rowHeight; y < ofGetHeight(); y += _rowHeight){
		for(float x = _colWidth; x < ofGetWidth(); x += _colWidth){
			ofDrawCircle(x, y, 5);
		}
	}
	ofPopStyle();
}

void Background::calcGrid(){
	_colWidth = (float)ofGetWidth() / (float)_numCols;
	_rowHeight = (float)ofGetHeight() / (float)_numRows;
}

} // namespace laserbugs
