//
//  nsUI.cpp
//  GravityCurrentSim
//
//  Created by 澤野陽介 on 2014/08/12.
//
//

#include "nsUI.h"
#include "Panel.h"

nsUI::nsUI(){
	lt = ofVec2f(10, 15);
	ysum=lt.y;
	fontL.loadFont("OpenSans-Regular.ttf", 16);
	fontM.loadFont("OpenSans-Regular.ttf", 11);
	fontS.loadFont("OpenSans-Regular.ttf", 9);
	bdrawframe=true;
	ofAddListener(ofEvents().draw, this, &nsUI::Render);
}
nsUI::~nsUI(){
	ofRemoveListener(ofEvents().draw, this, &nsUI::Render);
}

void nsUI::add(ofParameter<int> &parameter){
	hoge = dynamic_cast<nsUIBase*>(new Panel(parameter));
	collection.push_back(hoge);
	p.y+=SPASE;
}

void nsUI::Render(ofEventArgs & args){
	render();
	for(int i = 0; i < (int)collection.size(); i++){
		cout<<i<<"  "<<&p.y<<endl;
		
		collection[i]->draw();
		
//		cout<<this<<endl;
	}

}

void nsUI::render(){
	drawgen();
	cout<<"nsUIrender"<<endl;
//	collection[0]->draw();
}

void nsUI::drawgen(){
	drawframe(p.x, p.y, w, h);
	drawheader(p.x, p.y, w, h);
	addspacer();
//	addparam("N1");
//	addparam("N2");
//	addparam("N2");

	
}

void nsUI::drawframe(int x, int y, int w, int h){
	if (bdrawframe) {
		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(BORDER_COLOR);
		ofRect(x, y, w, h);
		ofSetColor(BODY_COLOR);
		ofRect(x+thickness, y+thickness, w-2*thickness, h-2*thickness);
		ofPopStyle();
	}
}

void nsUI::drawheader(int x, int y,int w, int h){
	if (bdrawframe) {
		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(HEADER_COLOR);
		ofRect(x, y, w, y+h_header);
		ofPopStyle();
	}
	drawheaderName((w+x)/2, h_header+y);
	sethight(h_header);
}

void nsUI::drawheaderName(int x, int y){
	string headerName = "NSSolver";
	int length=fontL.stringWidth(headerName);
	int hight =fontL.stringHeight(headerName);
	ofSetColor(LETTER_COLOR);
	fontL.drawString(headerName, x-length/2, y+hight/2);
}

void nsUI::addspacer(){
	ofSetColor(SPACER_COLOR);
	ofSetLineWidth(thickness);
	int y=gethight();
	ofLine(p.x+SPASE, p.y+y, p.x+w-SPASE, p.y+y);
	sethight(y);
}

void nsUI::addparam(string paramName){
	int y=gethight();
	int length=fontM.stringWidth(paramName);
	int hight =fontM.stringHeight(paramName);
	int cx=(w+lt.x)/2;
	int cy=y+2*hight;
	int x=lt.x+SPASE+SUBSPASE;
	ofSetColor(LETTER_COLOR);
	fontM.drawString(paramName, cx-length/2, cy);
	ofSetColor(BUTTON_COLOR);
	ofRect(x, y+hight, hight, hight);
	sethight(y+hight);
}
void nsUI::drawButton(){
	int y=gethight();

}