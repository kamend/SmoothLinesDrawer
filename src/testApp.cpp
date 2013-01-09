#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofEnableSmoothing();
	drawer.initCanvas(ofGetWidth(), ofGetHeight());

	
	drawColor = ofColor(255,0,0,100);
	addNewLine = false;
	addLastPoints = false;
	
	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void testApp::update(){


	if(addNewLine) {
		velocities.push_back(newLineSize);
		drawer.addToLine(newLinePos, newLineSize, drawColor);
		addNewLine = false;
	}
	drawer.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofEnableAlphaBlending();
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	ofBackground(0, 0, 0,255);
	ofSetColor(255,255,255,255);
	drawer.drawFbo.draw(0,0);
	ofSetWindowTitle(ofToString(ofGetFrameRate()));

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == ' ') {
		drawer.bUseSmoothLines = !drawer.bUseSmoothLines;
	}
	if(key == '1') {
		drawColor = ofColor(0,10,255,255);

	}
	
	if(key == '2') {
		drawColor = ofxNamedColors::antiqueBronze;
	}

	if(key == '3') {
		drawColor = ofxNamedColors::alloyOrange;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
//	ofPixels p;
//	drawer.drawFbo.readToPixels(p);
//	ofLog() << p.getColor(x,y);
}



//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	ofVec2f currentPosition = ofVec2f(x,y);
	
	float distance = currentPosition.distance(prevPosition);
	float size;
	if(distance > 1.0) {
	
		size = ofMap(distance, 0, ofGetWidth(), 1, 120);
		size = ofClamp(size, 1,120);
		
		float velocity;
		
		if(velocities.size() > 1) {
			size = size * 0.2f + velocities[velocities.size()-1]*0.8f;
		}
		newLineSize = size;
		newLinePos = ofVec2f(x,y);
		addNewLine = true;

	}
	prevPosition = currentPosition;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	prevPosition = ofVec2f(x,y);
	drawer.beginLine();
	drawer.createLine(ofVec2f(x,y));
	velocities.clear();
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
	ofVec2f currentPosition = ofVec2f(x,y);
	
	float distance = currentPosition.distance(prevPosition);
	float size;
	
	size = ofMap(distance, 0, ofGetWidth(), 20, 60);
	size = ofClamp(size, 20,60);
	
	float velocity;
	
	if(velocities.size() > 1) {
		size = size * 0.2f + velocities[velocities.size()-1]*0.8f;
	}
	newLineSize = size;
	newLinePos = ofVec2f(x,y);
	addNewLine = true;
	
	drawer.closeLine();

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}