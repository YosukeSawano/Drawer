//
//  isoLine.cpp
//  GravityCurrentSim_v1_0
//
//  Created by 澤野陽介 on 2014/11/21.
//
//

#include "isoLine.h"

isoLine::isoLine(){

}

isoLine::~isoLine(){

}

void isoLine::Cal(){
//	SetGrids(list<Vertices> &ldm);
	SetEdges();
}

void isoLine::SetEdges(){

}

void isoLine::SetEdges(isoLine::GRIDCELL grid, double isolevel){
	int i,squareIndex;
	int edgeTable[16];
	int lineTable[16][4];
	ofVec2f vertlist[4];

	squareIndex = 0;
	if (grid.val[0] < isolevel) squareIndex |= 1;
	if (grid.val[1] < isolevel) squareIndex |= 2;
	if (grid.val[2] < isolevel) squareIndex |= 4;
	if (grid.val[3] < isolevel) squareIndex |= 8;

	if (edgeTable[squareIndex] & 1)
		vertlist[0] =
		VertexInterp(isolevel,grid.p[0],grid.p[1],grid.val[0],grid.val[1]);
	if (edgeTable[squareIndex] & 2)
		vertlist[1] =
		VertexInterp(isolevel,grid.p[1],grid.p[2],grid.val[1],grid.val[2]);
	if (edgeTable[squareIndex] & 4)
		vertlist[2] =
		VertexInterp(isolevel,grid.p[2],grid.p[3],grid.val[2],grid.val[3]);
	if (edgeTable[squareIndex] & 8)
		vertlist[3] =
		VertexInterp(isolevel,grid.p[3],grid.p[0],grid.val[3],grid.val[0]);
	
}

ofVec2f isoLine::VertexInterp(double isolevel, ofVec2f p1, ofVec2f p2, double valp1, double valp2){
	double mu;
	ofVec3f p;

	if (ABS(isolevel-valp1) < 0.00001)
		return(p1);
	if (ABS(isolevel-valp2) < 0.00001)
		return(p2);
	if (ABS(valp1-valp2) < 0.00001)
		return(p1);
	mu = (isolevel - valp1) / (valp2 - valp1);
	p.x = p1.x + mu * (p2.x - p1.x);
	p.y = p1.y + mu * (p2.y - p1.y);

	return(p);
}