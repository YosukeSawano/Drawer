//
//  Panel.h
//  GravityCurrentSim
//
//  Created by 澤野陽介 on 2014/08/16.
//
//

#ifndef __GravityCurrentSim__Panel__
#define __GravityCurrentSim__Panel__

#include <iostream>
#include "nsUI.h"

class Panel : public nsUI{
public:
	Panel();
	Panel(ofParameter<int> &parameter);
	~Panel();
	
public:
	void setup();
	void setup(ofParameter<int> &parameter);
	virtual bool mouseReleased(ofMouseEventArgs & args);
	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);

protected:
	virtual void drawgen();
	virtual void render();
	
private:
	bool b;
	ofRectangle rect;
	
	//prototype
	int foo;
	ofParameter<int> parameter;
};

#endif /* defined(__GravityCurrentSim__Panel__) */
