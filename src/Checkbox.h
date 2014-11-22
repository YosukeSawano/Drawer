//
//  Checkbox.h
//  GravityCurrentSim
//
//  Created by 澤野陽介 on 2014/08/14.
//
//

#ifndef __GravityCurrentSim__Checkbox__
#define __GravityCurrentSim__Checkbox__

#include <iostream>
#include "ofParameterGroup.h"
#include "nsUI.h"
class Checkbox : public nsUI
{
public:
	Checkbox();
	~Checkbox();
public:
	void draw(ofEventArgs & args);

	virtual bool mouseReleased(ofMouseEventArgs & args);
	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);


private:
	bool b;
	ofRectangle rect;
	ofPath p;
	ofEvent<void> triggerEvent;
	void valueChanged(bool & v);

};

#endif /* defined(__GravityCurrentSim__Checkbox__) */
