//
//  isoBase.h
//  GravityCurrentSim_v1_0
//
//  Created by 澤野陽介 on 2014/11/21.
//
//

#ifndef __GravityCurrentSim_v1_0__isoBase__
#define __GravityCurrentSim_v1_0__isoBase__

#include <iostream>
#include <stdio.h>
#include <fstream.h>
#include "ofMain.h"
#include "Vertices.h"

class isoBase{
public:
	isoBase();
	~isoBase();

	struct Cell{
		int I1;
		int I2;
		int I3;
		int IX;
		double IX1;
		double IX2;
		double IX3;
		double Q;
		double B;
		double G1;
		double G2;
		double G3;
		double D1;
		double D2;
		double D3;
	};
	struct Domain_{
		std::string Name;
		Cell ***AC;
	};

	struct GRIDCELL_{
		ofVec3f p[8];
		double val[8];
	};

	void inputData();
	void SetGrids();
	void SetGrids(list<isoBase::Domain_> &ldm);
private:
	float times;
	int n1,n2,n3,n01,n02,n03;

	int timestep;
	int N1;
	int N2;
	int N3;
	int endT;
	int outT;
	int ts;
	double d1;
	double d2;
	double d3;
	double LN1;
	double LN2;
	double LN3;
	double DV;
	double U1;
	double U2;
	double U3;
	double dT;
	double Re;
	double Fn;
	double Rs;
	double DR;
	double RF;
	double ED;
	double DM;
	double XS;
	int NN;
	int MT;
	double dRho;
	enum plane{
		N1_N2,
		N2_N3,
		N1_N3
	};
	enum Scheme{
		centered,
		upwind,
		quick,
		piecewiselinear,
		lowdis_multidim
	};
	enum Scheme SH;
	enum RDistribution{
		Box,
		Wave
	};
	enum RDistribution RD;


//		//test
//	struct test{
//		int a;
//	};
//	std::list<isoBase::test> tests;
		//
	Domain_ domain;
	list<isoBase::Domain_> domains;
	list<isoBase::GRIDCELL_> gridcells;
	list<list<isoBase::GRIDCELL_> > lg;
};

#endif /* defined(__GravityCurrentSim_v1_0__isoBase__) */
