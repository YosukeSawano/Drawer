//
//  Vertices.h
//  GravityCurrentSim
//
//  Created by 澤野陽介 on 2014/07/15.
//
//

#ifndef __GravityCurrentSim__Vertices__
#define __GravityCurrentSim__Vertices__

#include <iostream>

class Vertices{
public:
	int timestep;
	int N1, N2, N3;
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
	struct Domain{
        std::string Name;
        Cell ***AC;
    };
	Domain domain;
	Vertices();
	~Vertices();
	void MakeDomains(std::string name, int n1, int n2, int n3, int ts);
	void SetValue();
private:
	
};



#endif /* defined(__GravityCurrentSim__Vertices__) */
