//
//  Panel.cpp
//  GravityCurrentSim
//
//  Created by 澤野陽介 on 2014/08/16.
//
//

#include "Panel.h"

Panel::Panel(){
	ofRegisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);
	b=false;
}

Panel::Panel(ofParameter<int> &parameter){
	ofRegisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);
	setup(parameter);
	b=false;
}

Panel::~Panel(){
	ofUnregisterMouseEvents(this,OF_EVENT_ORDER_BEFORE_APP);
}

void Panel::setup(){
	// proto
	b=false;
	foo=0;
	parameter.setName("test2");
	parameter.set(foo);
	add(parameter);
	add(parameter);
}

void Panel::setup(ofParameter<int> &parameter){
	drawgen();
}

void Panel::render(){
	drawgen();
	cout << "Panelrender" << endl;
}

void Panel::drawgen(){
	cout << "Paneldrawgen" << endl;
	if (b) {
		cout << "B-true" << endl;
		string paramName = getparamName(parameter);
//		int y=gethight();
//		int length=fontM.stringWidth(paramName);
		int hight =fontM.stringHeight(paramName);
//		int cx=(w+lt.x)/2;
//		int cy=y+2*hight;
//		int x=lt.x+SPASE+SUBSPASE;
		ofSetColor(LETTER_COLOR);
//		fontM.drawString(paramName, cx-length/2, cy);
		ofSetColor(BUTTON_COLOR);
		ofRect(p.x, p.y+hight, hight, hight);
//		sethight(y+hight);

	}
}

bool Panel::mousePressed(ofMouseEventArgs &args){
	cout << "mousePressed" << endl;
	b=true;
	return true;
}
bool Panel::mouseDragged(ofMouseEventArgs &args){
	cout << "mouseDragged" << endl;
	return true;
}
bool Panel::mouseMoved(ofMouseEventArgs &args){
	cout << "mouseMoved" << endl;
	return true;
}
bool Panel::mouseReleased(ofMouseEventArgs &args){
	cout << "mouseRereased" << endl;
	b=false;
	return true;
}