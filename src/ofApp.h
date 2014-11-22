#pragma once

#include "ofMain.h"
#include "NSSolver.h"
#include "isoSurface.h"
#include "isoLine.h"
#include "isoBase.h"
#include "Vertices.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	ofEasyCam cam;
	
	NSSolver nssolver;
	isoSurface isosurface;
	isoBase isobase;
	std::list<Vertices> ldm;
	
	bool bcal;
	int w,h;
	int N1, N2, N3;
	int camdis;
	vector<string>*ps;
	vector<string>s;
	
	
};
