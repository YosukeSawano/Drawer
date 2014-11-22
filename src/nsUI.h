//
//  nsUI.h
//  GravityCurrentSim
//
//  Created by 澤野陽介 on 2014/08/12.
//
//

#ifndef __GravityCurrentSim__nsUI__
#define __GravityCurrentSim__nsUI__

#include <iostream>
#include "ofMain.h"
#include "ofEvents.h"
#include "nsUIBase.h"
class nsUI : public nsUIBase{
public:
	nsUI();
	~nsUI();
public:

	void add(ofParameter<int> &parameter);

protected:
	virtual void drawgen();
	virtual void render();
	void Render(ofEventArgs & args);
private:
	void drawheaderName(int x, int y);
	void addspacer();
	void addparam(string parmaName);
	void drawframe(int x, int y,int w, int h);
	void drawheader(int x, int y,int w, int h);
	void drawButton();

private:
	nsUIBase* hoge;

};

#endif /* defined(__GravityCurrentSim__nsUI__) */
