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
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE);
	
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
	
	float distance = currentPosition.distance(prevPosition);
	float size;
	if(distance > 1.5) {
	
		size = ofMap(distance, 0, 100, 30, 1);
		size = ofClamp(size, 1,30);
		
		float velocity;
		
		if(velocities.size() > 1) {
			size = size * 0.2f + velocities[velocities.size()-1]*0.8f;
		}
		
		velocities.push_back(size);
		
		drawer.addLine(ofVec2f(x,y), size, ofColor(255,100,100,255));
	}
	prevPosition = currentPosition;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	prevPosition = ofVec2f(x,y);
	drawer.createLine(ofVec2f(x,y));
	velocities.clear();
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