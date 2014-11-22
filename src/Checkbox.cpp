//
//  Checkbox.cpp
//  GravityCurrentSim
//
//  Created by 澤野陽介 on 2014/08/14.
//
//

#include "Checkbox.h"
#include "ofGraphics.h"

Checkbox::Checkbox(){
	b=false;
	ofRegisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);
	ofAddListener(ofEvents().draw, this, &Checkbox::draw);
//	draw();
}
Checkbox::~Checkbox(){
	ofUnregisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);
	ofRemoveListener(ofEvents().draw, this, &Checkbox::draw);
}

void Checkbox::draw(ofEventArgs & args){
	if (b) {
		ofSetColor(255, 255, 255);
		ofRect(400, 400, 100, 100);
	}
	cout << "draw" << endl;
}

bool Checkbox::mousePressed(ofMouseEventArgs &args){
	cout << "mousePressed" << endl;
	b=true;
	return true;
}
bool Checkbox::mouseDragged(ofMouseEventArgs &args){
	cout << "mouseDragged" << endl;
	return true;
}
bool Checkbox::mouseMoved(ofMouseEventArgs &args){
	cout << "mouseMoved" << endl;
	return true;
}
bool Checkbox::mouseReleased(ofMouseEventArgs &args){
	cout << "mouseRereased" << endl;
	b=false;
	return true;
}