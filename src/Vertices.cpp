
//
//  Vertices.cpp
//  GravityCurrentSim
//
//  Created by 澤野陽介 on 2014/07/15.
//
//

#include "Vertices.h"

Vertices::Vertices(){}
Vertices::~Vertices(){
//	for (int i = 0; i < N1; i++) {
//        for (int j = 0; j < N2; j++) {
//            delete[] domain.AC[i][j];
//        }
//        delete[] domain.AC[i];
//    }
//    delete[] domain.AC;
}
void Vertices::MakeDomains(std::string name, int n1, int n2, int n3, int ts){
	domain.Name=name;
	N1=n1;
	N2=n2;
	N3=n3;
	timestep=ts;
	domain.AC = new Cell** [N1];
    for (int i = 0; i < N1; i++) {
        domain.AC[i] = new Cell* [N2];
        for (int j = 0; j < N2; j++) {
            domain.AC[i][j] = new Cell[N3];
        }
    }
    for (int i3 = 0; i3 < N3; i3++) {
        for (int i2 = 0; i2 < N2; i2++) {
            for (int i1 = 0; i1 < N1; i1++) {
                domain.AC[i1][i2][i3].I1 = i1;
                domain.AC[i1][i2][i3].I2 = i2;
                domain.AC[i1][i2][i3].I3 = i3;
				domain.AC[i1][i2][i3].Q  = 0.0;
            }
        }
    }
}