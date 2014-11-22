//
//  Inputbox.cpp
//  GravityCurrentSim
//
//  Created by 澤野陽介 on 2014/08/09.
//
//

#include "Inputbox.h"
#include <string.h>

//#include <boost/lexical_cast.hpp>

Inputbox::Inputbox(){
	text="";
	position=0;
	cursorx=0;
	cursory=0;
	font.loadFont("OpenSans-Regular.ttf", 13);
}

void Inputbox::drawText(ofPoint p){
	cursorx=font.stringWidth(subtext)+2;
	float height=font.stringHeight(subtext);
	ofSetColor(255, 255, 255);
	font.drawString(text, p.x, p.y);
	ofPushStyle();
	float timefrac=255.0f*sin(3.0f*ofGetElapsedTimef());
	ofSetColor(timefrac, timefrac, timefrac);
	ofSetLineWidth(1.0f);
	ofLine(cursorx+p.x, cursory+p.y, cursorx+p.x, cursory+p.y-height);
	ofPopStyle();
}
void Inputbox::typeKey(int key){
	if (key >= 32 && key <= 126) {
		text.insert(text.begin()+position, key);
		subtext.insert(subtext.begin()+position, key);
		position++;
	}
	if (key==OF_KEY_BACKSPACE) {
		if (position>0) {
			text.erase(text.begin()+position-1);
			position--;
		}
	}
//	if (key==OF_KEY_RETURN) {
//		text.insert(text.begin()+position, '\n');
//		position++;
//	}
	if (key==OF_KEY_DEL) {
		if (text.size()>position) {
			text.erase(text.begin()+position);
		}
	}
	if (key==OF_KEY_LEFT) {
		if (position>0) {
			subtext.erase(subtext.begin()+position-1);
			position--;
		}
	}
	if (key==OF_KEY_RIGHT) {
		if (position<text.size()) {
			char s=text.at(position);
			subtext.append(&s);
			position++;
		}
	}
}