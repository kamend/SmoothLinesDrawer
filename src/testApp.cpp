#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofEnableSmoothing();


	
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	ofEnableAlphaBlending();
	ofBackground(0);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	drawer.drawAll();


}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == ' ') {
		drawer.bUseSmoothLines = !drawer.bUseSmoothLines;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	ofVec2f currentPosition = ofVec2f(x,y);
	
	float velocity = currentPosition.distance(prevPosition);
	velocity = ofMap(velocity, 0, 100, 5, 1);
	velocity = ofClamp(velocity, 1,5);
	
	drawer.addLine(ofVec2f(x,y), velocity, ofColor(255,100,100));
	prevPosition = currentPosition;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	prevPosition = ofVec2f(x,y);
	drawer.createLine(ofVec2f(x,y));
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
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