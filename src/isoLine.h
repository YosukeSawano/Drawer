//
//  isoLine.h
//  GravityCurrentSim_v1_0
//
//  Created by 澤野陽介 on 2014/11/21.
//
//

#ifndef __GravityCurrentSim_v1_0__isoLine__
#define __GravityCurrentSim_v1_0__isoLine__

#include <iostream>
#include "ofMain.h"
#include "NSSolver.h"
#include "Vertices.h"

class isoLine{
public:
	isoLine();
	~isoLine();
//	ofMesh mesh;
	ofVec2f vert;
	vector<ofVec2f> verts;
//	vector<ofMesh> meshes;
	double isolevel;
//	struct TRIANGLE{
//		ofVec3f p[3];
//	};

	struct GRIDCELL{
		ofVec3f p[4];
		double val[4];
	};
	list<NSSolver::Domain> domain;
	list<isoLine::GRIDCELL> gridcells;
	list<list<isoLine::GRIDCELL> > lg;
	float times;

//	void Cal(list<NSSolver::Domain> &ldm);
//	void Cal(list<Vertices> &ldm);
	void Cal();
//	void Draw(int fn);
	void SetGrids(list<Vertices> &ldm);

private:
	int n1,n2,n3,n01,n02,n03;
//	void SetGrids(list<NSSolver::Domain> &ldm);
	void SetEdges(GRIDCELL grid,double isolevel);
	void SetEdges();
//	void SetLight();
//	void DrawLight();
	ofVec2f VertexInterp(double isolevel, ofVec2f p1, ofVec2f p2, double valp1, double valp2);
//	ofLight light1;
//	ofLight light2;
//	ofLight light3;
//	ofLight light4;
};

#endif /* defined(__GravityCurrentSim_v1_0__isoLine__) */
