//
//  isoSurface.h
//  GravityCurrentSim
//
//  Created by 澤野陽介 on 2014/06/30.
//
//

#ifndef __GravityCurrentSim__isoSurface__
#define __GravityCurrentSim__isoSurface__

#include <iostream>
#include "ofMain.h"
#include "NSSolver.h"
#include "Vertices.h"

class isoSurface{
public:
	isoSurface();
	ofMesh mesh;
	vector<ofMesh> meshes;
	double isolevel;
	struct TRIANGLE{
		ofVec3f p[3];
	};
	
	struct GRIDCELL{
		ofVec3f p[8];
		double val[8];
	};
	list<NSSolver::Domain> domain;
	list<isoSurface::GRIDCELL> gridcells;
	list<list<isoSurface::GRIDCELL> > lg;
	float times;
	
	void Cal(list<NSSolver::Domain> &ldm);
	void Cal(list<Vertices> &ldm);
	void Draw(int fn);
	void SetGrids(list<Vertices> &ldm);

private:
	int n1,n2,n3,n01,n02,n03;
	void SetGrids(list<NSSolver::Domain> &ldm);
	int SetPolygons(GRIDCELL grid,double isolevel);
	void SetPolygons();
	void SetLight();
	void DrawLight();
	ofVec3f VertexInterp(double isolevel, ofVec3f p1, ofVec3f p2, double valp1, double valp2);
	ofLight light1;
	ofLight light2;
	ofLight light3;
	ofLight light4;
};


#endif /* defined(__GravityCurrentSim__isoSurface__) */
