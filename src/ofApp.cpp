#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0,0,0);
	ofEnableAlphaBlending();
	ofDisableArbTex();
	ofEnableSmoothing();
	ofEnableAntiAliasing();
//	cam.setAutoDistance(true);
	cam.setDistance(5000);
	
	ofSetFrameRate(10);
	bcal=false;
	w=ofGetWindowWidth()/2;
	h=ofGetWindowHeight()/2;
	nssolver.SetConditions();

}

//--------------------------------------------------------------
void ofApp::exit()
{

}

//--------------------------------------------------------------
void ofApp::update(){
	cam.setDistance(camdis);
}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.begin();
	if (bcal) {
		isosurface.Draw(ofGetFrameNum());
	}
	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key=='c') {
		nssolver.nssolver(ldm);
		isosurface.Cal(ldm);
		std::list<Vertices>::iterator Itelator=ldm.begin();
		N1 = isosurface.times * Itelator->N1;
		camdis=2*N1;
		bcal=true;
	}
	if (key=='t') {
		isobase.inputData();
		isobase.SetGrids();
	}
	if (key==OF_KEY_DOWN) {
		camdis+=50;
	}
	if (key==OF_KEY_UP) {
		camdis-=50;
	}
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
