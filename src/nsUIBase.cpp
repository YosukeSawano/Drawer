//
//  nsUIBase.cpp
//  GravityCurrentSim
//
//  Created by 澤野陽介 on 2014/08/15.
//
//

#include "nsUIBase.h"

nsUIBase::nsUIBase(){
	p = lt = ofVec2f(10, 15);
	p.x=getX();
	fontL.loadFont("OpenSans-Regular.ttf", 16);
	fontM.loadFont("OpenSans-Regular.ttf", 11);
	fontS.loadFont("OpenSans-Regular.ttf", 9);
	bdrawframe=false;
//	ofAddListener(ofEvents().draw, this, &nsUIBase::draw);
}
nsUIBase::~nsUIBase(){
//	ofRemoveListener(ofEvents().draw, this, &nsUIBase::draw);
}

void nsUIBase::draw(){
	render();
}

int nsUIBase::gethight(){
	return p.y+SPASE;
}

int nsUIBase::getX(){
	return p.x+SPASE+SUBSPASE;
}

int nsUIBase::getY(){
	return p.y+SPASE;
}

void nsUIBase::sethight(int y){
	p.y = y+SPASE;
}

string nsUIBase::getparamName(ofParameter<int> param){
	return param.getName();
}