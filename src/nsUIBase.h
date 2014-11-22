//
//  nsUIBase.h
//  GravityCurrentSim
//
//  Created by 澤野陽介 on 2014/08/15.
//
//

#ifndef __GravityCurrentSim__nsUIBase__
#define __GravityCurrentSim__nsUIBase__

#include <iostream>
#include "ofMain.h"

#define LETTER_COLOR ofColor(255, 255, 255)
#define SPACER_COLOR ofColor(255, 255, 255)
#define BACKGROUND_COLOR ofColor(0, 0, 0, 255)
#define BODY_COLOR ofColor(0, 0, 0, 255)
#define BORDER_COLOR ofColor(255, 255, 255, 50)
#define HEADER_COLOR ofColor(255, 255, 255, 50)
#define BUTTON_COLOR ofColor(255, 255, 255, 200)
#define BUTTON_HEIGHT 11;
#define SPASE 10
#define SUBSPASE 5

static int thickness = 2;
static int w = 180;
static int h = 400;
static int h_header = 30;
static int h_button = 20;

class nsUIBase{
public:
	nsUIBase();
	~nsUIBase();

public:
	void draw();
	
protected:
	virtual void drawgen()=0;
	virtual void render()=0;
	int gethight();
	int getX();
	int getY();
	void sethight(int y);
	
	string getparamName(ofParameter<int> param);
protected:
	ofTrueTypeFont fontL;
	ofTrueTypeFont fontM;
	ofTrueTypeFont fontS;
	ofVec2f lt;
	ofVec2f p;
	bool bdrawframe;
	int ysum;
	vector<nsUIBase *> collection;
};

#endif /* defined(__GravityCurrentSim__nsUIBase__) */
