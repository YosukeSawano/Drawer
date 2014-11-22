//
//  isoBase.cpp
//  GravityCurrentSim_v1_0
//
//  Created by 澤野陽介 on 2014/11/21.
//
//

#include "isoBase.h"

isoBase::isoBase(){

}

isoBase::~isoBase(){

}

void isoBase::inputData(){

	string buff;
	ifstream ifs("/Users/keybordman/Desktop/programing/GCSim/data/DFV/100.csv");
	if( !ifs ) {
		cout << "Error:Input data file not found" << endl;
		return;
	}
	ifs>>n1;
	ifs>>n2;
	ifs>>n3;

	domain.AC = new Cell** [n1];
	for (int i = 0; i < n1; i++) {
		domain.AC[i] = new Cell* [n2];
		for (int j = 0; j < n2; j++) {
			domain.AC[i][j] = new Cell[n3];
		}
	}
	for (int i3 = 0; i3 < n3; i3++) {
		for (int i2 = 0; i2 < n2; i2++) {
			for (int i1 = 0; i1 < n1; i1++) {
				domain.AC[i1][i2][i3].I1 = i1;
				domain.AC[i1][i2][i3].I2 = i2;
				domain.AC[i1][i2][i3].I3 = i3;
				domain.AC[i1][i2][i3].Q  = 0.0;
			}
		}
	}
	cout<<"input"<<endl;
	for (int i3=0; i3<n3; i3++) {
		for (int i2=0; i2<n2; i2++) {
			for (int i1=0; i1<n1; i1++) {
				getline(ifs, buff,',');
//				cout<<buff;
				domain.AC[i1][i2][i3].Q=atof(buff.c_str());
			}
//			cout<<endl;
		}
//		cout<<endl;
	}
	cout<<"output"<<endl;
	for (int i3=0; i3<n3; i3++) {
		for (int i2=0; i2<n2; i2++) {
			for (int i1=0; i1<n1; i1++) {
//				cout<<domain.AC[i1][i2][i3].Q;
			}
//			cout<<endl;
		}
//		cout<<endl;
	}
	domains.push_back(domain);
//	for (int i = 0; i < n1; i++) {
//		for (int j = 0; j < n2; j++) {
//			delete[] domain.AC[i][j];
//		}
//		delete[] domain.AC[i];
//	}
//	delete[] domain.AC;
}


void isoBase::SetGrids(){
	SetGrids(domains);
}

void isoBase::SetGrids(list<Domain_> &ldm){
	list<Domain_>::iterator Iterator = ldm.begin();
		//	n1 = (Iterator->N1-1)*times;
		//	n2 = (Iterator->N2-1)*times;
		//	n3 = (Iterator->N3-1)*times;
	int index1=n1-1;
	int index2=n2-1;
	int index3=n3-1;
//	n1 = times*Iterator->domain.AC[index1][0][0].I1;
//	n2 = times*Iterator->domain.AC[0][index2][0].I2;
//	n3 = times*Iterator->domain.AC[0][0][index3].I3;
	n01 = times*Iterator->AC[0][0][0].I1;
	n02 = times*Iterator->AC[0][0][0].I2;
	n03 = times*Iterator->AC[0][0][0].I3;

	for (; Iterator != ldm.end(); ++Iterator) {
//		cout << Iterator->timestep << &Iterator->timestep << "\n";
		for (int i3=0; i3<n3-1; i3++) {
			for (int i2=0; i2<n2-1; i2++) {
				for (int i1=0; i1<n1-1; i1++) {
					GRIDCELL_ gridcell;

					gridcell.val[0]=Iterator->AC[i1][i2+1][i3].Q;
					gridcell.p[0].x=Iterator->AC[i1][i2+1][i3].I1;
					gridcell.p[0].y=Iterator->AC[i1][i2+1][i3].I2;
					gridcell.p[0].z=Iterator->AC[i1][i2+1][i3].I3;

					gridcell.val[1]=Iterator->AC[i1+1][i2+1][i3].Q;
					gridcell.p[1].x=Iterator->AC[i1+1][i2+1][i3].I1;
					gridcell.p[1].y=Iterator->AC[i1+1][i2+1][i3].I2;
					gridcell.p[1].z=Iterator->AC[i1+1][i2+1][i3].I3;

					gridcell.val[2]=Iterator->AC[i1+1][i2][i3].Q;
					gridcell.p[2].x=Iterator->AC[i1+1][i2][i3].I1;
					gridcell.p[2].y=Iterator->AC[i1+1][i2][i3].I2;
					gridcell.p[2].z=Iterator->AC[i1+1][i2][i3].I3;

					gridcell.val[3]=Iterator->AC[i1][i2][i3].Q;
					gridcell.p[3].x=Iterator->AC[i1][i2][i3].I1;
					gridcell.p[3].y=Iterator->AC[i1][i2][i3].I2;
					gridcell.p[3].z=Iterator->AC[i1][i2][i3].I3;

					gridcell.val[4]=Iterator->AC[i1][i2+1][i3+1].Q;
					gridcell.p[4].x=Iterator->AC[i1][i2+1][i3+1].I1;
					gridcell.p[4].y=Iterator->AC[i1][i2+1][i3+1].I2;
					gridcell.p[4].z=Iterator->AC[i1][i2+1][i3+1].I3;

					gridcell.val[5]=Iterator->AC[i1+1][i2+1][i3+1].Q;
					gridcell.p[5].x=Iterator->AC[i1+1][i2+1][i3+1].I1;
					gridcell.p[5].y=Iterator->AC[i1+1][i2+1][i3+1].I2;
					gridcell.p[5].z=Iterator->AC[i1+1][i2+1][i3+1].I3;

					gridcell.val[6]=Iterator->AC[i1+1][i2][i3+1].Q;
					gridcell.p[6].x=Iterator->AC[i1+1][i2][i3+1].I1;
					gridcell.p[6].y=Iterator->AC[i1+1][i2][i3+1].I2;
					gridcell.p[6].z=Iterator->AC[i1+1][i2][i3+1].I3;

					gridcell.val[7]=Iterator->AC[i1][i2][i3+1].Q;
					gridcell.p[7].x=Iterator->AC[i1][i2][i3+1].I1;
					gridcell.p[7].y=Iterator->AC[i1][i2][i3+1].I2;
					gridcell.p[7].z=Iterator->AC[i1][i2][i3+1].I3;

					for (int k = 0; k < 8; k++) {
						gridcell.p[k]*=times;
					}
//					cout << &Iterator->AC[i1][i2][i3];
					cout.precision(2);
					cout << Iterator->AC[i1][i2][i3].Q<<" ";

					gridcells.push_back(gridcell);
				}
				printf("\n");
			}
			printf("\n");
		}
		lg.push_back(gridcells);
		gridcells.clear();
	}

}
