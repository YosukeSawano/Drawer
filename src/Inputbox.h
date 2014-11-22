//
//  Inputbox.h
//  GravityCurrentSim
//
//  Created by 澤野陽介 on 2014/08/09.
//
//

#ifndef __GravityCurrentSim__Inputbox__
#define __GravityCurrentSim__Inputbox__

#include <iostream>
#include "ofMain.h"
#include "nsUI.h"

class Inputbox : public nsUI{
public:
	Inputbox();

public:
	void typeKey(int key);
	void drawText(ofPoint p);
//	void render();
	void setValue();
	void generateDraw();
private:
	string text;
	string subtext;

	int	 position;
	int	 cursorx, cursory;
	ofTrueTypeFont font;
};

#endif /* defined(__GravityCurrentSim__Inputbox__) */
