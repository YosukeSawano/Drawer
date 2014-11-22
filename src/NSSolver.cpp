//
//  NSSolver.cpp
//  MyCFD
//
//  Created by 澤野 陽介 on 2014/03/05.
//  Copyright (c) 2014年 澤野 陽介. All rights reserved.
//

#include "NSSolver.h"
#include <stdio.h>
#include <boost/foreach.hpp>
#include <math.h>
#define foreach BOOST_FOREACH

using namespace std;

NSSolver::NSSolver()
{
	bMakeDomain=false;
    cout << "Constructer of NSSolver was called." << endl;
}
NSSolver::~NSSolver()
{
    cout << "Destructer of NSSolver was called." << endl;
	if (bMakeDomain) {
		DeleteDomain(&DFV, N1, N2, N3);
		DeleteDomain(&DPS, N1, N2, N3);
		DeleteDomain(&DU1, N1, N2, N3);
		DeleteDomain(&DU2, N1, N2, N3);
		DeleteDomain(&DU3, N1, N2, N3);
		DeleteGrids(&GPS, GPS.N1, GPS.N2, GPS.N3);
		DeleteGrids(&GU1, GU1.N1, GU1.N2, GU1.N3);
		DeleteGrids(&GU2, GU2.N1, GU2.N2, GU2.N3);
		DeleteGrids(&GU3, GU3.N1, GU3.N2, GU3.N3);
		DeleteGrids(&GFV, GFV.N1, GFV.N2, GFV.N3);
	}
}
void NSSolver::SetConditions(){
	SetIniN();
}
void NSSolver::nssolver(std::list<Vertices> &ldm)
{
	SetDomain();
    SetGridsofWE();
    ConnectDomains();
    SetFluidCellofNS();
	SetZero();
    SetBoundary();
    SetInitialConditionofRho();
    NonSlip();
    AverageSurface();
    SetSurfaceVelosity();
    WritetoConsoleofWE();
	std::list<Vertices> lverts;
	Vertices vertex;
	vertex.MakeDomains("DFV", N1, N2, N3, 0);
	PushbackVertsObj(vertex);
	lverts.push_back(vertex);
    for (ts = 1; ts < endT; ts++) {
        xDR();
        AdvectionofNS();
        Diffusion();
        xDVofNS();
        DivergenceofNS();
        Gravity();
        Pressure();
        NonSlip();
        AverageSurface();
        SetSurfaceVelosity();
        if (ts % outT == 0) {
            WritetoConsoleofWE();
        }
		Vertices vertex2;
		vertex2.MakeDomains("DFV", N1, N2, N3, ts);
		PushbackVertsObj(vertex2);
		lverts.push_back(vertex2);
//		OutputDomains(ldm);
    }
	ldm=lverts;
//    plot();
}
void NSSolver::WETest()
{
    SetIniW();
    SetDomain();
    SetGridsofWE();
    ConnectDomains();
    SetFluidCellofWE();
    SetInitialConditionofRho();
    SetVelocity();
    AverageSurface();
    SetSurfaceVelosity();
    WritetoConsoleofWE();
    for (ts = 1; ts < endT; ts++) {
        AdvectionofWE();
        xDVofWE();
        DivergenceofWE();
        dDVofWE();
        if (ts % outT == 0) {
            WritetoConsoleofWE();
        }
    }
}
void NSSolver::PushbackVertsObj(Vertices &vertex){
	vertex.timestep=ts;
	for (int i3 = 0; i3 < N3; i3++) {
        for (int i2 = 0; i2 < N2; i2++) {
            for (int i1 = 0; i1 < N1; i1++) {
                vertex.domain.AC[i1][i2][i3].I1 = DFV.AC[i1][i2][i3].I1;
                vertex.domain.AC[i1][i2][i3].I2 = DFV.AC[i1][i2][i3].I2;
				vertex.domain.AC[i1][i2][i3].I3 = DFV.AC[i1][i2][i3].I3;
				vertex.domain.AC[i1][i2][i3].Q  = DFV.AC[i1][i2][i3].Q;
            }
        }
    }
}
void NSSolver::OutputDomains(std::list<Domain> &OutDomains){
	foreach(Domain * dm, LDA){
		dm->timestep=ts;
	}
	Domain copydm=DFV;
	OutDomains.push_back(copydm);
}
void NSSolver::SetIniN()
{
    N1 = 12;
    N2 = 12;
    N3 = 11;
    LN1 = 1.0;
    LN2 = 1.0;
    LN3 = 1.0;
    d1 = LN1 / (N1 - 2);
    d2 = LN2 / (N2 - 2);
    d3 = LN3 / (N3 - 1);
    DV = d1 * d2 * d3;
    ts = 0;
    endT = 10;
    outT = 1;
    dT = 0.01;
    Re = 3300.0;
    Rs = 3.7E+6;
    Fn = 1.0;
    DR = 0.01;
    RF = 0.8;
    ED = 0.001;
    MT = 500;
    NN = N1 * N2 * N3;
    DM = RF / (2.0 * dT * (1.0 / (d1 * d1) * 1.0 / (d2 * d2) + 1.0 / (d3 * d3)));
//    SH = lowdis_multidim;
	SH = upwind;
    RD = Box;
}
void NSSolver::SetIniW()
{
    N1 = 10;
    N2 = 10;
    N3 = 1;
    LN1 = 1.0;
    LN2 = 1.0;
    LN3 = 1.0;
    d1 = LN1 / N1;
    d2 = LN2 / N2;
    d3 = LN3 / N3;
    DV = d1 * d2 * d3;
    U1 = 1.0;
    U2 = 1.0;
    U3 = 0.0;
    endT = 100;
    outT = 1;
    dT = 0.05;
    SH = lowdis_multidim;
    RD = Box;
}
void NSSolver::SetDomain()
{
	bMakeDomain=true;
    MakeDomain("DFV", &DFV, N1, N2, N3);
    MakeDomain("DPS", &DPS, N1, N2, N3);
    MakeDomain("DU1", &DU1, N1, N2, N3);
    MakeDomain("DU2", &DU2, N1, N2, N3);
    MakeDomain("DU3", &DU3, N1, N2, N3);
    MakeListofDomain();
}
void NSSolver::MakeDomain(string Name, NSSolver::Domain* dm, int n1, int n2,
                          int n3)
{
    dm->Name = Name;
	dm->N1 = N1;
	dm->N2 = N2;
	dm->N3 = N3;
    dm->AC = new Cell** [n1];
    for (int i = 0; i < n1; i++) {
        dm->AC[i] = new Cell* [n2];
        for (int j = 0; j < n2; j++) {
            dm->AC[i][j] = new Cell[n3];
        }
    }
    for (int i3 = 0; i3 < n3; i3++) {
        for (int i2 = 0; i2 < n2; i2++) {
            for (int i1 = 0; i1 < n1; i1++) {
                dm->AC[i1][i2][i3].I1 = i1;
                dm->AC[i1][i2][i3].I2 = i2;
                dm->AC[i1][i2][i3].I3 = i3;
                dm->LC.push_back(&dm->AC[i1][i2][i3]);
            }
        }
    }
    foreach(Cell * c, dm->LC)
    {
        int I1O = c->I1;
        int I2O = c->I2;
        int I3O = c->I3;
        int I1P = (I1O + 1 + n1) % n1;
        int I1M = (I1O - 1 + n1) % n1;
        int I2P = (I2O + 1 + n2) % n2;
        int I2M = (I2O - 1 + n2) % n2;
        int I3P = (I3O + 1 + n3) % n3;
        int I3M = (I3O - 1 + n3) % n3;
        c->C10 = &dm->AC[I1M][I2O][I3O];
        c->C11 = &dm->AC[I1P][I2O][I3O];
        c->C20 = &dm->AC[I1O][I2M][I3O];
        c->C21 = &dm->AC[I1O][I2P][I3O];
        c->C30 = &dm->AC[I1O][I2O][I3M];
        c->C31 = &dm->AC[I1O][I2O][I3P];
        c->S10 = &dm->AC[I1O][I2O][I3O].ss10;
        c->S11 = &dm->AC[I1O][I2O][I3O].ss11;
        c->S20 = &dm->AC[I1O][I2O][I3O].ss20;
        c->S21 = &dm->AC[I1O][I2O][I3O].ss21;
        c->S30 = &dm->AC[I1O][I2O][I3O].ss30;
        c->S31 = &dm->AC[I1O][I2O][I3O].ss31;
        c->C10->S11 = &dm->AC[I1O][I2O][I3O].ss10;
        c->C11->S10 = &dm->AC[I1O][I2O][I3O].ss11;
        c->C20->S21 = &dm->AC[I1O][I2O][I3O].ss20;
        c->C21->S20 = &dm->AC[I1O][I2O][I3O].ss21;
        c->C30->S31 = &dm->AC[I1O][I2O][I3O].ss30;
        c->C31->S30 = &dm->AC[I1O][I2O][I3O].ss31;
    }
}
void NSSolver::MakeListofDomain()
{
    LDA.push_back(&DPS);
    LDA.push_back(&DFV);
    LDA.push_back(&DU1);
    LDA.push_back(&DU2);
    LDA.push_back(&DU3);
    LDV.push_back(&DU1);
    LDV.push_back(&DU2);
    LDV.push_back(&DU3);
    LDF.push_back(&DU1);
    LDF.push_back(&DU2);
    LDF.push_back(&DU3);
    LDF.push_back(&DFV);
}
void NSSolver::SetGridsofNS()
{
    MakeGrids("GPS", &GPS, &DPS, 0, 1, N1 - 1, 0, N2 - 1, 1, N3 - 1);
    MakeGrids("GU1", &GU1, &DU1, 1, 1, N1 - 1, 0, N2 - 1, 1, N3 - 1);
    MakeGrids("GU2", &GU2, &DU2, 2, 1, N1 - 1, 0, N2 - 1, 1, N3 - 1);
    MakeGrids("GU3", &GU3, &DU3, 3, 1, N1 - 1, 0, N2 - 1, 1, N3 - 1);
    MakeGrids("GFV", &GFV, &DFV, 4, 1, N1 - 1, 1, N2 - 1, 1, N3 - 1);
    MakeListofGrids();
}
void NSSolver::SetGridsofWE()
{
    MakeGrids("GPS", &GPS, &DPS, 0, 0, N1, 0, N2, 0, N3);
    MakeGrids("GU1", &GU1, &DU1, 1, 0, N1, 0, N2, 0, N3);
    MakeGrids("GU2", &GU2, &DU2, 2, 0, N1, 0, N2, 0, N3);
    MakeGrids("GU3", &GU3, &DU3, 3, 0, N1, 0, N2, 0, N3);
    MakeGrids("GFV", &GFV, &DFV, 4, 0, N1, 0, N2, 0, N3);
    MakeListofGrids();
}
void NSSolver::MakeGrids(std::string Name, NSSolver::Grids* gs,
                         NSSolver::Domain* dm, int p, int n10, int n11, int n20,
                         int n21, int n30, int n31)
{
    gs->Name = Name;
    gs->P = p;
    gs->N1 = n11 - n10 + 1;
    gs->N2 = n21 - n20 + 1;
    gs->N3 = n31 - n30 + 1;
    gs->AG = new Grids** [gs->N1];
    for (int i = 0; i < gs->N1; i++) {
        gs->AG[i] = new Grids* [gs->N2];
        for (int j = 0; j < gs->N2; j++) {
            gs->AG[i][j] = new Grids[gs->N3];
        }
    }
    for (int i3 = 0; i3 < gs->N3; i3++) {
        for (int i2 = 0; i2 < gs->N2; i2++) {
            for (int i1 = 0; i1 < gs->N1; i1++) {
                gs->AG[i1][i2][i3].I1 = i1;
                gs->AG[i1][i2][i3].I2 = i2;
                gs->AG[i1][i2][i3].I3 = i3;
                gs->LG.push_back(&gs->AG[i1][i2][i3]);
            }
        }
    }
}
void NSSolver::MakeListofGrids()
{
    LGA.push_back(GPS);
    LGA.push_back(GU1);
    LGA.push_back(GU2);
    LGA.push_back(GU3);
    LGA.push_back(GFV);
}
void NSSolver::ConnectDomains()
{
    for (int i3 = 0; i3 < N3; i3++) {
        for (int i2 = 0; i2 < N2; i2++) {
            for (int i1 = 0; i1 < N1; i1++) {
                //                CPS = &DPS.AC[i1][i2][i3];
                //                CU1 = &DU1.AC[i1][i2][i3];
                //                CU2 = &DU2.AC[i1][i2][i3];
                //                CU3 = &DU3.AC[i1][i2][i3];
                //                CFV = &DFV.AC[i1][i2][i3];

                DPS.AC[i1][i2][i3].CPS = &DPS.AC[i1][i2][i3];
                DPS.AC[i1][i2][i3].CU1 = &DU1.AC[i1][i2][i3];
                DPS.AC[i1][i2][i3].CU2 = &DU2.AC[i1][i2][i3];
                DPS.AC[i1][i2][i3].CU3 = &DU3.AC[i1][i2][i3];
                DPS.AC[i1][i2][i3].CFV = &DFV.AC[i1][i2][i3];
                DU1.AC[i1][i2][i3].CPS = &DPS.AC[i1][i2][i3];
                DU1.AC[i1][i2][i3].CU1 = &DU1.AC[i1][i2][i3];
                DU1.AC[i1][i2][i3].CU2 = &DU2.AC[i1][i2][i3];
                DU1.AC[i1][i2][i3].CU3 = &DU3.AC[i1][i2][i3];
                DU1.AC[i1][i2][i3].CFV = &DFV.AC[i1][i2][i3];
                DU2.AC[i1][i2][i3].CPS = &DPS.AC[i1][i2][i3];
                DU2.AC[i1][i2][i3].CU1 = &DU1.AC[i1][i2][i3];
                DU2.AC[i1][i2][i3].CU2 = &DU2.AC[i1][i2][i3];
                DU2.AC[i1][i2][i3].CU3 = &DU3.AC[i1][i2][i3];
                DU2.AC[i1][i2][i3].CFV = &DFV.AC[i1][i2][i3];
                DU3.AC[i1][i2][i3].CPS = &DPS.AC[i1][i2][i3];
                DU3.AC[i1][i2][i3].CU1 = &DU1.AC[i1][i2][i3];
                DU3.AC[i1][i2][i3].CU2 = &DU2.AC[i1][i2][i3];
                DU3.AC[i1][i2][i3].CU3 = &DU3.AC[i1][i2][i3];
                DU3.AC[i1][i2][i3].CFV = &DFV.AC[i1][i2][i3];
                DFV.AC[i1][i2][i3].CPS = &DPS.AC[i1][i2][i3];
                DFV.AC[i1][i2][i3].CU1 = &DU1.AC[i1][i2][i3];
                DFV.AC[i1][i2][i3].CU2 = &DU2.AC[i1][i2][i3];
                DFV.AC[i1][i2][i3].CU3 = &DU3.AC[i1][i2][i3];
                DFV.AC[i1][i2][i3].CFV = &DFV.AC[i1][i2][i3];

                //                DPS.AC[i1][i2][i3].CPS->CPS = CPS;
                //                DPS.AC[i1][i2][i3].CPS->CU1 = CU1;
                //                DPS.AC[i1][i2][i3].CPS->CU2 = CU2;
                //                DPS.AC[i1][i2][i3].CPS->CU3 = CU3;
                //                DPS.AC[i1][i2][i3].CPS->CFV = CFV;
                //                DU1.AC[i1][i2][i3].CU1->CPS = CPS;
                //                DU1.AC[i1][i2][i3].CU1->CU1 = CU1;
                //                DU1.AC[i1][i2][i3].CU1->CU2 = CU2;
                //                DU1.AC[i1][i2][i3].CU1->CU3 = CU3;
                //                DU1.AC[i1][i2][i3].CU1->CFV = CFV;
                //                DU2.AC[i1][i2][i3].CU2->CPS = CPS;
                //                DU2.AC[i1][i2][i3].CU2->CU1 = CU1;
                //                DU2.AC[i1][i2][i3].CU2->CU2 = CU2;
                //                DU2.AC[i1][i2][i3].CU2->CU3 = CU3;
                //                DU2.AC[i1][i2][i3].CU2->CFV = CFV;
                //                DU3.AC[i1][i2][i3].CU3->CPS = CPS;
                //                DU3.AC[i1][i2][i3].CU3->CU1 = CU1;
                //                DU3.AC[i1][i2][i3].CU3->CU2 = CU2;
                //                DU3.AC[i1][i2][i3].CU3->CU3 = CU3;
                //                DU3.AC[i1][i2][i3].CU3->CFV = CFV;
                //                DFV.AC[i1][i2][i3].CFV->CPS = CPS;
                //                DFV.AC[i1][i2][i3].CFV->CU1 = CU1;
                //                DFV.AC[i1][i2][i3].CFV->CU2 = CU2;
                //                DFV.AC[i1][i2][i3].CFV->CU3 = CU3;
                //                DFV.AC[i1][i2][i3].CFV->CFV = CFV;

                //                CPS->CPS = CPS;
                //                CPS->CU1 = CU1;
                //                CPS->CU2 = CU2;
                //                CPS->CU3 = CU3;
                //                CPS->CFV = CFV;
                //                CU1->CPS = CPS;
                //                CU1->CU1 = CU1;
                //                CU1->CU2 = CU2;
                //                CU1->CU3 = CU3;
                //                CU1->CFV = CFV;
                //                CU2->CPS = CPS;
                //                CU2->CU1 = CU1;
                //                CU2->CU2 = CU2;
                //                CU2->CU3 = CU3;
                //                CU2->CFV = CFV;
                //                CU3->CPS = CPS;
                //                CU3->CU1 = CU1;
                //                CU3->CU2 = CU2;
                //                CU3->CU3 = CU3;
                //                CU3->CFV = CFV;
                //                CFV->CPS = CPS;
                //                CFV->CU1 = CU1;
                //                CFV->CU2 = CU2;
                //                CFV->CU3 = CU3;
                //                CFV->CFV = CFV;
            }
        }
    }
}
void NSSolver::SetFluidCellofWE()
{
    foreach(Domain * dm, LDA)
    {
        foreach(Cell * c, dm->LC)
        {
            dm->LFC.push_back(c);
        }
    }
}
void NSSolver::SetFluidCellofNS()
{
    FluidCell(&DPS, 1, N1 - 2, 1, N2 - 2, 1, N3 - 2);
    FluidCell(&DU1, 2, N1 - 2, 1, N2 - 2, 1, N3 - 2);
    FluidCell(&DU2, 1, N1 - 2, 2, N2 - 2, 1, N3 - 2);
    FluidCell(&DU3, 1, N1 - 2, 1, N2 - 2, 2, N3 - 2);
    FluidCell(&DFV, 1, N1 - 2, 1, N2 - 2, 1, N3 - 2);
}
void NSSolver::SetBoundary()
{

    BNeumann(&DPS, 0, 1, N2_N3);
    BNeumann(&DPS, N1 - 1, N1 - 2, N2_N3);
    BNeumann(&DPS, 0, 1, N1_N3);
    BNeumann(&DPS, N2 - 1, N2 - 2, N1_N3);
    BNeumann(&DPS, 0, 1, N1_N2);
    BNeumann(&DPS, N3 - 1, N3 - 2, N1_N2);

    BDirichlet(&DU1, 1, N2_N3);
    BDirichlet(&DU1, N1 - 1, N2_N3);
    BNeumann(&DU1, 0, 1, N1_N3);
    BNeumann(&DU1, N2 - 1, N2 - 2, N1_N3);
    BNeumann(&DU1, 0, 1, N1_N2);
    BNeumann(&DU1, N3 - 1, N3 - 2, N1_N2);

    BNeumann(&DU2, 0, 1, N2_N3);
    BNeumann(&DU2, N1 - 1, N1 - 2, N2_N3);
    BDirichlet(&DU2, 1, N1_N3);
    BDirichlet(&DU2, N2 - 1, N1_N3);
    BNeumann(&DU2, 0, 1, N1_N2);
    BNeumann(&DU2, N3 - 1, N3 - 2, N1_N2);

    BNeumann(&DU3, 0, 1, N2_N3);
    BNeumann(&DU3, N1 - 1, N1 - 2, N2_N3);
    BNeumann(&DU3, 0, 1, N1_N3);
    BNeumann(&DU3, N2 - 1, N2 - 2, N1_N3);
    BDirichlet(&DU3, 1, N1_N2);
    BDirichlet(&DU3, N3 - 1, N1_N2);

    BNeumann(&DFV, 0, 1, N2_N3);
    BNeumann(&DFV, N1 - 1, N1 - 2, N2_N3);
    BNeumann(&DFV, 0, 1, N1_N3);
    BNeumann(&DFV, N2 - 1, N2 - 2, N1_N3);
    BNeumann(&DFV, 0, 1, N1_N2);
    BNeumann(&DFV, N3 - 1, N3 - 2, N1_N2);
}
void NSSolver::SetZero(){
	foreach(Domain * dm, LDA)
    {
        foreach(Cell * c, dm->LC)
        {
			c->IX = 0;
            c->Q  = 0.0;
			c->B  = 0.0;
			c->G1 = 0.0;
			c->G2 = 0.0;
			c->G3 = 0.0;
			c->IX1= 0.0;
			c->IX2= 0.0;
			c->IX3= 0.0;
        }
    }
}
//void NSSolver::SetZero(NSSolver::Domain *dm){
//    foreach(Cell *c,dm->LC){
//        c->Q=0.0;
//    }
//}
void NSSolver::NonSlip()
{
    ImposeNC(&DU1);
    ImposeDC(&DU1);
    ImposeNC(&DU2);
    ImposeDC(&DU2);
    ImposeNC(&DU3);
    ImposeDC(&DU3);
    ImposeZC(&DFV);
    ImposeZC(&DPS);
}
void NSSolver::FluidCell(NSSolver::Domain* dm, int IF10, int IF11, int IF20,
                         int IF21, int IF30, int IF31)
{
    for (int i3 = IF30; i3 <= IF31; i3++) {
        for (int i2 = IF20; i2 <= IF21; i2++) {
            for (int i1 = IF10; i1 <= IF11; i1++) {
                dm->LFC.push_back(&dm->AC[i1][i2][i3]);
            }
        }
    }
}
void NSSolver::BNeumann(NSSolver::Domain* dm, int IB3, int IF3, enum plane ID)
{
    switch (ID) {
    case N1_N2:
        for (int i2 = 0; i2 < N2; i2++) {
            for (int i1 = 0; i1 < N1; i1++) {
                bN.CB = &dm->AC[i1][i2][IB3];
                bN.CF = &dm->AC[i1][i2][IF3];
                dm->LNBC.push_back(&bN);
            }
        }
        break;
    case N1_N3:
        for (int i3 = 0; i3 < N3; i3++) {
            for (int i1 = 0; i1 < N1; i1++) {
                bN.CB = &dm->AC[i1][IB3][i3];
                bN.CF = &dm->AC[i1][IF3][i3];
                dm->LNBC.push_back(&bN);
            }
        }
        break;
    case N2_N3:
        for (int i3 = 0; i3 < N3; i3++) {
            for (int i2 = 0; i2 < N1; i2++) {
                bN.CB = &dm->AC[IB3][i2][i3];
                bN.CF = &dm->AC[IF3][i2][i3];
                dm->LNBC.push_back(&bN);
            }
        }
        break;
    default:
        break;
    }
}
void NSSolver::BDirichlet(NSSolver::Domain* dm, int ID3, enum plane ID)
{
    switch (ID) {
    case N1_N2:
        for (int i2 = 0; i2 < N2; i2++) {
            for (int i1 = 0; i1 < N1; i1++) {
                bD.CD = &dm->AC[i1][i2][ID3];
                dm->LDBC.push_back(&bD);
            }
        }
        break;
    case N1_N3:
        for (int i3 = 0; i3 < N3; i3++) {
            for (int i1 = 0; i1 < N1; i1++) {
                bD.CD = &dm->AC[i1][ID3][i3];
                dm->LDBC.push_back(&bD);
            }
        }
        break;
    case N2_N3:
        for (int i3 = 0; i3 < N3; i3++) {
            for (int i2 = 0; i2 < N1; i2++) {
                bD.CD = &dm->AC[ID3][i2][i3];
                dm->LDBC.push_back(&bD);
            }
        }
        break;
    default:
        break;
    }
}
void NSSolver::ImposeNC(NSSolver::Domain* dm)
{
    foreach(NBoundary * bN, dm->LNBC)
    {
        bN->CB->Q = -bN->CF->Q;
    }
}
void NSSolver::ImposeDC(NSSolver::Domain* dm)
{
    foreach(DBoundary * bD, dm->LDBC)
    {
        bD->CD->Q = 0.0;
    }
}
void NSSolver::ImposeZC(NSSolver::Domain* dm)
{
    foreach(NBoundary * bN, dm->LNBC)
    {
        bN->CB->Q = bN->CF->Q;
    }
}
void NSSolver::SetInitialConditionofRho()
{
    foreach(Domain * dm, LDA)
    {
        foreach(Cell * c, dm->LFC)
        {
            c->Q = 0.0;
			c->B = 0.0;
			c->G1= 0.0;
			c->G2= 0.0;
			c->G3= 0.0;
        }
    }
    switch (RD) {
    case Box:

        foreach(Cell * c, DFV.LFC)
        {
            if (3 * N1 / 8 <= c->I1 && c->I1 <= 5 * N1 / 8) {
                if (3 * N2 / 8 <= c->I2 && c->I2 <= 5 * N2 / 8) {
                    c->Q = 1.0;
                } else
                    c->Q = 0.0;
            } else
                c->Q = 0.0;
        }

        break;
    case Wave:
        foreach(Cell * c, DFV.LFC)
        {
            c->Q = 1.0 * sin(2.0 * M_PI * (double)c->I1 / (double)N1);
        }
        break;
	case Exchange:
			foreach(Cell * c, DFV.LFC){
				if(N1/3<=c->I1){
					c->Q=1.0;
				}
				else{
					c->Q=0.0;
				}
			}
		break;
    default:
        break;
    }
}
void NSSolver::SetVelocity()
{
    foreach(Cell * c, DU1.LFC)
    {
        c->Q = U1;
    }
    foreach(Cell * c, DU2.LFC)
    {
        c->Q = U2;
    }
    foreach(Cell * c, DU3.LFC)
    {
        c->Q = U3;
    }
}
void NSSolver::AverageSurface()
{
    foreach(Domain * dm, LDA)
    {
        foreach(Cell * c, dm->LC)
        {
            c->S10->Q = (c->Q + c->C10->Q) * 0.5;
            c->S20->Q = (c->Q + c->C20->Q) * 0.5;
            c->S30->Q = (c->Q + c->C30->Q) * 0.5;
        }
    }
}
void NSSolver::SetSurfaceVelosity()
{
    foreach(Cell * c, DU1.LC)
    {
        c->S10->V = c->CU1->S10->Q;
        c->S20->V = c->CU2->S10->Q;
        c->S30->V = c->CU3->S10->Q;
    }
    foreach(Cell * c, DU2.LC)
    {
        c->S10->V = c->CU1->S20->Q;
        c->S20->V = c->CU2->S20->Q;
        c->S30->V = c->CU3->S20->Q;
    }
    foreach(Cell * c, DU3.LC)
    {
        c->S10->V = c->CU1->S30->Q;
        c->S20->V = c->CU2->S30->Q;
        c->S30->V = c->CU3->S30->Q;
    }
    foreach(Cell * c, DPS.LC)
    {
        c->S10->V = c->CU1->Q;
        c->S20->V = c->CU2->Q;
        c->S30->V = c->CU3->Q;
    }
    foreach(Cell * c, DFV.LC)
    {
        c->S10->V = c->CU1->Q;
        c->S20->V = c->CU2->Q;
        c->S30->V = c->CU3->Q;
    }
}
void NSSolver::WritetoConsoleofWE()
{
    WritetoConsole(&DFV);
}
void NSSolver::WritetoConsole(NSSolver::Domain* dm)
{
    printf("ts=%d\n", ts);
    for (int i3 = 0; i3 < N3; i3++) {
        for (int i2 = 0; i2 < N2; i2++) {
            for (int i1 = 0; i1 < N1; i1++) {
                printf("%f ", dm->AC[i1][i2][i3].Q);
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("\n");
}
void NSSolver::AdvectionofWE()
{
    Advection(&DFV);
}
void NSSolver::AdvectionofNS()
{
    Advection(&DFV);
    foreach(Domain * dm, LDV)
    {
        Advection(dm);
    }
}
void NSSolver::Advection(NSSolver::Domain* dm)
{
    switch (SH) {
    case centered:
        Center(dm);
        break;
    case upwind:
        Upwind(dm);
        break;
    case quick:
        Quick(dm);
        break;
    case piecewiselinear:
        Gradient(dm);
        PieceWiseLinear(dm);
    case lowdis_multidim:
        Gradient(dm);
        InterfaceLocation(dm);
        InterfaceGradient(dm);
        DirectioFreePiecewiseLinear(dm);
        ClearFlux(dm);
        break;
    default:
        break;
    }
}
void NSSolver::Center(NSSolver::Domain* dm)
{
    foreach(Cell * c, dm->LC)
    {
        c->S10->F = c->S10->V * (c->Q + c->C10->Q) * 0.5;
        c->S20->F = c->S20->V * (c->Q + c->C20->Q) * 0.5;
        c->S30->F = c->S30->V * (c->Q + c->C30->Q) * 0.5;
    }
}
void NSSolver::Upwind(NSSolver::Domain* dm)
{
    Center(dm);
    foreach(Cell * c, dm->LC)
    {
        c->S10->F -= fabs(c->S10->V) * (c->Q - c->C10->Q) * 0.5;
        c->S20->F -= fabs(c->S20->V) * (c->Q - c->C20->Q) * 0.5;
        c->S30->F -= fabs(c->S30->V) * (c->Q - c->C30->Q) * 0.5;
    }
}
void NSSolver::Quick(NSSolver::Domain* dm)
{
    foreach(Cell * c, dm->LC)
    {
        c->S10->F = (c->S10->V * (-c->C11->Q + 9.0 * c->Q + 9.0 * c->C10->Q - c->C10->C10->Q) - fabs(c->S10->V) * (c->C11->Q + 3.0 * c->Q - 3.0 * c->C10->Q - c->C10->C10->Q)) / 16.0;
        c->S20->F = (c->S20->V * (-c->C21->Q + 9.0 * c->Q + 9.0 * c->C20->Q - c->C20->C20->Q) - fabs(c->S20->V) * (c->C21->Q + 3.0 * c->Q - 3.0 * c->C20->Q - c->C20->C20->Q)) / 16.0;
        c->S30->F = (c->S30->V * (-c->C31->Q + 9.0 * c->Q + 9.0 * c->C30->Q - c->C30->C30->Q) - fabs(c->S30->V) * (c->C31->Q + 3.0 * c->Q - 3.0 * c->C30->Q - c->C30->C30->Q)) / 16.0;
    }
}
void NSSolver::Gradient(NSSolver::Domain* dm)
{
    foreach(Cell * c, dm->LC)
    {
        c->G1 = (c->C11->Q - c->C10->Q) * 0.5 / d1;
        c->G2 = (c->C21->Q - c->C20->Q) * 0.5 / d2;
        c->G3 = (c->C31->Q - c->C30->Q) * 0.5 / d3;
        c->S10->G = (c->Q - c->C10->Q) / d1;
        c->S20->G = (c->Q - c->C20->Q) / d2;
        c->S30->G = (c->Q - c->C30->Q) / d3;
        c->D1 = (c->C11->Q - 2.0 * c->Q + c->C10->Q) / (d1 * d1);
        c->D2 = (c->C21->Q - 2.0 * c->Q + c->C20->Q) / (d2 * d2);
        c->D3 = (c->C31->Q - 2.0 * c->Q + c->C30->Q) / (d3 * d3);
    }
}
void NSSolver::PieceWiseLinear(NSSolver::Domain* dm)
{
    foreach(Cell * c, dm->LC)
    {
        if (c->S10->V >= 0.0) {
            c->S10->F = c->S10->V * (c->C10->Q + c->C10->G1 * (d1 - c->S10->V * dT) * 0.5);
        } else
            c->S10->F = c->S10->V * (c->Q - c->G1 * (d1 + c->S10->V * dT) * 0.5);
        if (c->S20->V >= 0.0) {
            c->S20->F = c->S20->V * (c->C20->Q + c->C20->G2 * (d2 - c->S20->V * dT) * 0.5);
        } else
            c->S20->F = c->S20->V * (c->Q - c->G2 * (d2 + c->S20->V * dT) * 0.5);
        if (c->S30->V >= 0.0) {
            c->S30->F = c->S30->V * (c->C30->Q + c->C30->G3 * (d3 - c->S30->V * dT) * 0.5);
        } else
            c->S30->F = c->S30->V * (c->Q - c->G3 * (d3 + c->S30->V * dT) * 0.5);
    }
}
void NSSolver::InterfaceLocation(NSSolver::Domain* dm)
{
    double tH = 1.0;
    foreach(Cell * c, dm->LFC)
    {
        c->IX = 0;
        c->IX1 = -100.0;
        c->IX2 = -100.0;
        c->IX3 = -100.0;
    }
    foreach(Cell * c, dm->LFC)
    {
        tH = M_PI / (8.0 * d1);
        if (fabs(c->G1) > tH) {
            if (((fabs(c->S10->G) > tH) && (fabs(c->C10->G1) <= fabs(c->G1))) || ((fabs(c->S11->G) > tH) && (fabs(c->G1) >= fabs(c->C11->G1)))) {
                dm->LCI1.push_back(c);
                c->IX1 = c->S11->G / (c->S10->G + c->S11->G) - 0.5;
                c->IX++;
            }
        }
        tH = M_PI / (8.0 * d2);
        if (fabs(c->G2) > tH) {
            if (((fabs(c->S20->G) > tH) && (fabs(c->C20->G2) <= fabs(c->G2))) || ((fabs(c->S21->G) > tH) && (fabs(c->G2) >= fabs(c->C21->G2)))) {
                dm->LCI2.push_back(c);
                c->IX2 = c->S21->G / (c->S20->G + c->S21->G) - 0.5;
                c->IX++;
            }
        }
        tH = M_PI / (8.0 * d3);
        if (fabs(c->G3) > tH) {
            if (((fabs(c->S30->G) > tH) && (fabs(c->C30->G3) <= fabs(c->G3))) || ((fabs(c->S31->G) > tH) && (fabs(c->G3) >= fabs(c->C31->G3)))) {
                dm->LCI3.push_back(c);
                c->IX3 = c->S31->G / (c->S30->G + c->S31->G) - 0.5;
                c->IX++;
            }
        }
        if (c->IX > 0) {
            dm->LCI.push_back(c);
        }
    }
    foreach(Cell * c, dm->LFC)
    {
        if (c->IX != 1) {
            double d12 = (d1 + d2) / pow(2.0, 0.5);
            double a40 = (c->Q - c->C10->C20->Q) / d12;
            double a41 = (c->C11->C21->Q - c->Q) / d12;
            double g4 = (c->C11->C21->Q - c->C10->C20->Q) * 0.5 / d12;
            double g40 = (c->Q - c->C10->C20->C10->C20->Q) * 0.5 / d12;
            double g41 = (c->C11->C21->C11->C21->Q - c->Q) * 0.5 / d12;
            double a50 = (c->Q - c->C11->C20->Q) / d12;
            double a51 = (c->C10->C21->Q - c->Q) / d12;
            double g5 = (c->C10->C21->Q - c->C11->C20->Q) * 0.5 / d12;
            double g50 = (c->Q - c->C11->C20->C11->C20->Q) * 0.5 / d12;
            double g51 = (c->C10->C21->C10->C21->Q - c->Q) * 0.5 / d12;

            double d13 = (d1 + d3) / pow(2.0, 0.5);
            double a60 = (c->Q - c->C10->C30->Q) / d13;
            double a61 = (c->C11->C31->Q - c->Q) / d13;
            double g6 = (c->C11->C31->Q - c->C10->C30->Q) * 0.5 / d13;
            double g60 = (c->Q - c->C10->C30->C10->C30->Q) * 0.5 / d13;
            double g61 = (c->C11->C31->C11->C31->Q - c->Q) * 0.5 / d13;
            double a70 = (c->Q - c->C11->C30->Q) / d13;
            double a71 = (c->C10->C31->Q - c->Q) / d13;
            double g7 = (c->C10->C31->Q - c->C11->C30->Q) * 0.5 / d13;
            double g70 = (c->Q - c->C11->C30->C11->C30->Q) * 0.5 / d13;
            double g71 = (c->C10->C31->C10->C31->Q - c->Q) * 0.5 / d13;

            double d23 = (d2 + d3) / pow(2.0, 0.5);
            double a80 = (c->Q - c->C20->C30->Q) / d23;
            double a81 = (c->C21->C31->Q - c->Q) / d23;
            double g8 = (c->C21->C31->Q - c->C20->C30->Q) * 0.5 / d23;
            double g80 = (c->Q - c->C20->C30->C20->C30->Q) * 0.5 / d23;
            double g81 = (c->C21->C31->C21->C31->Q - c->Q) * 0.5 / d23;
            double a90 = (c->Q - c->C21->C30->Q) / d23;
            double a91 = (c->C20->C31->Q - c->Q) / d23;
            double g9 = (c->C20->C31->Q - c->C21->C30->Q) * 0.5 / d23;
            double g90 = (c->Q - c->C21->C30->C21->C30->Q) * 0.5 / d23;
            double g91 = (c->C20->C31->C20->C31->Q - c->Q) * 0.5 / d23;

            tH = M_PI / (8.0 * d12);
            if ((fabs(g4) > tH) && ((fabs(a40) > tH && (fabs(g40) <= fabs(g4))) || ((fabs(a41) > tH) && (fabs(g4) >= fabs(g41))))) {
                double ix = 0.0;
                if ((fabs(c->C11->C21->S10->G) < 1.0 / tH) && (fabs(c->C11->C21->S20->G) < 1.0 / tH)) {
                    ix = a41 / (a40 + a41);
                    IP(c->G1, c->G2, ix, c->IX1, c->IX2);
                    dm->LCI1.push_back(c);
                    dm->LCI2.push_back(c);
                    c->IX = 2;
                }
                if ((fabs(c->C10->C20->S11->G) < 1.0 / tH) && (fabs(c->C10->C20->S21->G) < 1.0 / tH)) {
                    ix = a40 / (a40 + a41);
                    IP(c->G1, c->G2, ix, c->IX1, c->IX2);
                    c->IX1 *= -1.0;
                    c->IX2 *= -1.0;
                    dm->LCI1.push_back(c);
                    dm->LCI2.push_back(c);
                    c->IX = 2;
                }
            }
            if ((fabs(g5) > tH) && ((fabs(a50) > tH && (fabs(g50) <= fabs(g5))) || ((fabs(a51) > tH) && (fabs(g5) >= fabs(g51))))) {
                double ix = 0.0;
                if ((fabs(c->C10->C21->S11->G) < 1.0 / tH) && (fabs(c->C10->C21->S20->G) < 1.0 / tH)) {
                    ix = a51 / (a50 + a51);
                    IP(c->G1, c->G2, ix, c->IX1, c->IX2);
                    c->IX1 *= -1.0;
                    dm->LCI1.push_back(c);
                    dm->LCI2.push_back(c);
                    c->IX = 2;
                }
                if ((fabs(c->C11->C20->S10->G) < 1.0 / tH) && (fabs(c->C11->C20->S21->G) < 1.0 / tH)) {
                    ix = a50 / (a50 + a51);
                    IP(c->G1, c->G2, ix, c->IX1, c->IX2);
                    c->IX2 *= -1.0;
                    dm->LCI1.push_back(c);
                    dm->LCI2.push_back(c);
                    c->IX = 2;
                }
            }
            tH = M_PI / (8.0 * d13);
            if ((fabs(g6) > tH) && ((fabs(a60) > tH && (fabs(g60) <= fabs(g6))) || ((fabs(a61) > tH) && (fabs(g6) >= fabs(g61))))) {
                double ix = 0.0;
                if ((fabs(c->C11->C31->S10->G) < 1.0 / tH) && (fabs(c->C11->C31->S30->G) < 1.0 / tH)) {
                    ix = a61 / (a60 + a61);
                    IP(c->G1, c->G3, ix, c->IX1, c->IX3);
                    dm->LCI1.push_back(c);
                    dm->LCI3.push_back(c);
                    c->IX = 3;
                }
                if ((fabs(c->C10->C30->S11->G) < 1.0 / tH) && (fabs(c->C10->C30->S31->G) < 1.0 / tH)) {
                    ix = a60 / (a60 + a61);
                    IP(c->G1, c->G3, ix, c->IX1, c->IX3);
                    c->IX1 *= -1.0;
                    c->IX3 *= -1.0;
                    dm->LCI1.push_back(c);
                    dm->LCI3.push_back(c);
                    c->IX = 3;
                }
            }
            if ((fabs(g7) > tH) && ((fabs(a70) > tH && (fabs(g70) <= fabs(g7))) || ((fabs(a71) > tH) && (fabs(g7) >= fabs(g71))))) {
                double ix = 0.0;
                if ((fabs(c->C10->C31->S11->G) < 1.0 / tH) && (fabs(c->C10->C31->S30->G) < 1.0 / tH)) {
                    ix = a71 / (a70 + a71);
                    IP(c->G1, c->G3, ix, c->IX1, c->IX3);
                    c->IX1 *= -1.0;
                    dm->LCI1.push_back(c);
                    dm->LCI3.push_back(c);
                    c->IX = 3;
                }
                if ((fabs(c->C11->C30->S10->G) < 1.0 / tH) && (fabs(c->C11->C30->S31->G) < 1.0 / tH)) {
                    ix = a70 / (a70 + a71);
                    IP(c->G1, c->G3, ix, c->IX1, c->IX3);
                    c->IX3 *= -1.0;
                    dm->LCI1.push_back(c);
                    dm->LCI3.push_back(c);
                    c->IX = 3;
                }
            }
            tH = M_PI / (8.0 * d23);
            if ((fabs(g8) > tH) && ((fabs(a80) > tH && (fabs(g80) <= fabs(g8))) || ((fabs(a81) > tH) && (fabs(g8) >= fabs(g81))))) {
                double ix = 0.0;
                if ((fabs(c->C21->C31->S20->G) < 1.0 / tH) && (fabs(c->C21->C31->S30->G) < 1.0 / tH)) {
                    ix = a81 / (a40 + a81);
                    IP(c->G2, c->G3, ix, c->IX2, c->IX3);
                    dm->LCI2.push_back(c);
                    dm->LCI3.push_back(c);
                    c->IX = 1;
                }
                if ((fabs(c->C20->C30->S21->G) < 1.0 / tH) && (fabs(c->C20->C30->S31->G) < 1.0 / tH)) {
                    ix = a80 / (a80 + a81);
                    IP(c->G2, c->G3, ix, c->IX2, c->IX3);
                    c->IX2 *= -1.0;
                    c->IX3 *= -1.0;
                    dm->LCI2.push_back(c);
                    dm->LCI3.push_back(c);
                    c->IX = 1;
                }
            }
            if ((fabs(g9) > tH) && ((fabs(a90) > tH && (fabs(g90) <= fabs(g9))) || ((fabs(a91) > tH) && (fabs(g9) >= fabs(g91))))) {
                double ix = 0.0;
                if ((fabs(c->C20->C31->S21->G) < 1.0 / tH) && (fabs(c->C20->C31->S30->G) < 1.0 / tH)) {
                    ix = a91 / (a90 + a91);
                    IP(c->G2, c->G3, ix, c->IX2, c->IX3);
                    c->IX2 *= -1.0;
                    dm->LCI2.push_back(c);
                    dm->LCI3.push_back(c);
                    c->IX = 1;
                }
                if ((fabs(c->C21->C30->S20->G) < 1.0 / tH) && (fabs(c->C21->C30->S31->G) < 1.0 / tH)) {
                    ix = a90 / (a90 + a91);
                    IP(c->G2, c->G3, ix, c->IX2, c->IX3);
                    c->IX3 *= -1.0;
                    dm->LCI2.push_back(c);
                    dm->LCI3.push_back(c);
                    c->IX = 1;
                }
            }
        }
        if (c->IX > 0) {
            dm->LCI.push_back(c);
        }
        //
        //        //test
        //        foreach(Cell *c, dm->LFC){
        //            c->IX=0.0;
        //        }
    }
}
void NSSolver::IP(double g1, double g2, double ix, double& ix1, double& ix2)
{
    if (fabs(g1 * g2) > 1.0E-10) {
        ix1 = pow(fabs(ix * g2 / g1), 0.5);
    }
    ix1 = max(ix1, ix);
    ix1 = min(ix1, 1.0);
    ix2 = max(ix2, ix);
    ix2 = min(ix2, 1.0);
    ix1 -= 0.5;
    ix2 -= 0.5;
}
void NSSolver::InterfaceGradient(NSSolver::Domain* dm)
{
    foreach(Cell * c, dm->LCI)
    {
        c->G1 = 0.0;
        c->G2 = 0.0;
        c->G3 = 0.0;
    }
    foreach(Cell * c, dm->LCI1)
    {
        c->C10->G1 = 0.0;
        c->C11->G1 = 0.0;
    }
    foreach(Cell * c, dm->LCI2)
    {
        c->C20->G2 = 0.0;
        c->C21->G2 = 0.0;
    }
    foreach(Cell * c, dm->LCI3)
    {
        c->C30->G3 = 0.0;
        c->C31->G3 = 0.0;
    }
}
void NSSolver::DirectioFreePiecewiseLinear(NSSolver::Domain* dm)
{
    foreach(Cell * c, dm->LC)
    {
        c->B = 0.0;
    }
    foreach(Cell * c, dm->LFC)
    {
        double* q;
        double** qq;
        double*** qqq;
        double* x;
        double* x1;
        double** x2;
        double*** x3;
        double*** v;
        double qmax;
        double qmin;
        double qqqmax;
        double qqqmin;
        q = new double[3];
        qq = new double* [3];
        for (int i = 0; i < 3; i++) {
            qq[i] = new double[3];
        }
        qqq = new double** [3];
        for (int i = 0; i < 3; i++) {
            qqq[i] = new double* [3];
            for (int j = 0; j < 3; j++) {
                qqq[i][j] = new double[3];
            }
        }
        x = new double[4];
        x1 = new double[4];
        x2 = new double* [3];
        for (int i = 0; i < 3; i++) {
            x2[i] = new double[4];
        }
        x3 = new double** [3];
        for (int i = 0; i < 3; i++) {
            x3[i] = new double* [3];
            for (int j = 0; j < 3; j++) {
                x3[i][j] = new double[4];
            }
        }
        v = new double** [3];
        for (int i = 0; i < 3; i++) {
            v[i] = new double* [3];
            for (int j = 0; j < 3; j++) {
                v[i][j] = new double[4];
            }
        }
        DivideCell(*c, x1, x2, x3);
        for (int i3 = 0; i3 < 3; i3++) {
            for (int i2 = 0; i2 < 3; i2++) {
                for (int i1 = 0; i1 < 3; i1++) {
                    v[i1][i2][i3] = (x1[i1 + 1] - x1[i1]) * (x2[i1][i2 + 1] - x2[i1][i2]) * (x3[i1][i2][i3 + 1] - x3[i1][i2][i3]);
                }
            }
        }
        qmax = MaxQ(dm, c);
        qmin = MinQ(dm, c);
        if (c->IX == 0) {
            qqqmax = -1.0E10;
            qqqmin = +1.0E10;
            for (int i3 = 0; i3 < 3; i3++) {
                for (int i2 = 0; i2 < 3; i2++) {
                    for (int i1 = 0; i1 < 3; i1++) {
                        qqq[i1][i2][i3] = I(c, x[i1], x[i1 + 1], x2[i1][i2], x2[i1][i2 + 1], x3[i1][i2][i3], x3[i1][i2][i3 + 1]);
                        qqqmax = max(qqqmax, qqq[i1][i2][i3]);
                        qqqmin = min(qqqmin, qqq[i1][i2][i3]);
                        //                        printf("%d %d %d xx qqqmax=%f qqqmin%f\n",i1,i2,i3,qqqmax,qqqmin);
                    }
                }
            }
            //            printf("qqqmax=%f qqqmin%f\n",qqqmax,qqqmin);
            double r = 1.0;
            double r1 = 1.0;
            double r2 = 1.0;
            if (qqqmax > qmax) {
                r1 = (qmax - c->Q) / (qqqmax - c->Q);
            }
            if (qqqmin < qmin) {
                r2 = (qmin - c->Q) / (qqqmin - c->Q);
            }
            r = min(r1, r2);
            c->G1 *= r;
            c->G2 *= r;
            c->G3 *= r;
            for (int i3 = 0; i3 < 3; i3++) {
                for (int i2 = 0; i2 < 3; i2++) {
                    for (int i1 = 0; i1 < 3; i1++) {
                        qqq[i1][i2][i3] = I(c, x[i1], x[i1 + 1], x2[i1][i2], x2[i1][i2 + 1], x3[i1][i2][i3], x3[i1][i2][i3 + 1]);
                        qqqmax = max(qqqmax, qqq[i1][i2][i3]);
                        qqqmin = min(qqqmin, qqq[i1][i2][i3]);
                    }
                }
            }
            //            printf("next  qqqmax=%f qqqmin%f\n",qqqmax,qqqmin);
        } else {
            for (int i1 = 0; i1 < 4; i1++) {
                x[i1] = x1[i1];
            }
            for (int i1 = 0; i1 < 3; i1++) {
                q[i1] = FormFunction(x, c->IX1, c->Q, c->C10->Q, c->C11->Q, c->C10->IX, c->C11->IX, qmax, qmin)[i1];
            }
            for (int i1 = 0; i1 < 3; i1++) {
                for (int i2 = 0; i2 < 4; i2++) {
                    x[i2] = x2[i1][i2];
                }
                for (int i2 = 0; i2 < 3; i2++) {
                    qq[i1][i2] = FormFunction(x, c->IX2, q[i1], c->C20->Q, c->C21->Q, c->C20->IX, c->C21->IX, qmax, qmin)[i2];
                }
                for (int i2 = 0; i2 < 3; i2++) {
                    for (int i3 = 0; i3 < 4; i3++) {
                        x[i3] = x3[i1][i2][i3];
                    }
                    for (int i3 = 0; i3 < 3; i3++) {
                        qqq[i1][i2][i3] = FormFunction(x, c->IX3, qq[i1][i2], c->C30->Q, c->C31->Q, c->C30->IX, c->C31->IX, qmax, qmin)[i3];
                    }
                }
            }
        }
        //        printf("%d %s %d %d %d\n",ts,dm->Name.c_str(), c->I1, c->I2, c->I3);
        double vvv = 0.0;
        double qqqq = 0.0;
        for (int i3 = 0; i3 < 3; i3++) {
            vvv = 0.0;
            qqqq = 0.0;
            for (int i2 = 0; i2 < 3; i2++) {
                for (int i1 = 0; i1 < 3; i1++) {
                    vvv += v[i1][i2][i3];
                    qqqq += qqq[i1][i2][i3];
                    qqq[i1][i2][i3] *= v[i1][i2][i3];
                    //                    printf("v%f ",v[i1][i2][i3]);
                    //                    printf("qqqq%f ",qqq[i1][i2][i3]);
                    //                    printf("%f ",qqq[i1][i2][i3]);
                    //                    printf("%f ",q[i1]);
                    int i = (c->I1 + i1 - 1 + N1) % N1;
                    int j = (c->I2 + i2 - 1 + N2) % N2;
                    int k = (c->I3 + i3 - 1 + N3) % N3;
                    dm->AC[i][j][k].B += qqq[i1][i2][i3];
                    dm->AC[c->I1][c->I2][c->I3].B -= qqq[i1][i2][i3];
                }
                //                printf("v=%f\n",qqqq);
            }
            //            printf("\n");
        }

        delete[] q;
        for (int i = 0; i < 3; i++) {
            delete[] qq[i];
        }
        delete[] qq;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                delete[] qqq[i][j];
            }
            delete[] qqq[i];
        }
        delete[] qqq;
        delete[] x;
        delete[] x1;
        for (int i = 0; i < 3; i++) {
            delete[] x2[i];
        }
        delete[] x2;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                delete[] x3[i][j];
            }
            delete[] x3[i];
        }
        delete[] x3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                delete[] v[i][j];
            }
            delete[] v[i];
        }
        delete[] v;
    }
    foreach(Cell * c, dm->LFC)
    {
        c->Q += c->B;
    }
    CorrectionDivergenceErroro(dm);
}
void NSSolver::DivideCell(const NSSolver::Cell c, double* x1, double** x2, double*** x3)
{
    Cell c1;
    Cell c2;
    Cell c10;
    Cell c11;
    Cell c20;
    Cell c21;

    x1[0] = -0.5;
    x1[1] = max(F(c, 1, 0), -0.5);
    x1[2] = min(F(c, 1, 1), +0.5);
    x1[3] = +0.5;
    for (int i1 = 0; i1 < 3; i1++) {
        switch (i1) {
        case 0:
            c10 = *c.C10;
            c1 = c10;
            break;
        case 1:
            c1 = c;
            break;
        case 2:
            c11 = *c.C11;
            c1 = c11;
            break;
        default:
            break;
        }
        x2[i1][0] = -0.5;
        x2[i1][1] = max(F(c1, 2, 0), -0.5);
        x2[i1][2] = min(F(c1, 2, 1), +0.5);
        x2[i1][3] = +0.5;
        for (int i2 = 0; i2 < 3; i2++) {
            switch (i2) {
            case 0:
                c20 = *c.C20;
                c2 = c20;
                break;
            case 1:
                c2 = c1;
                break;
            case 2:
                c21 = *c.C21;
                c2 = c21;
                break;
            default:
                break;
            }
            x3[i1][i2][0] = -0.5;
            x3[i1][i2][1] = max(F(c2, 3, 0), -0.5);
            x3[i1][i2][2] = min(F(c2, 3, 1), +0.5);
            x3[i1][i2][3] = +0.5;
        }
    }
}
double NSSolver::F(const NSSolver::Cell c, int i, int j)
{
    double f10 = -0.5 - c.S10->V * dT / d1;
    double f11 = +0.5 - c.S11->V * dT / d1;
    if (i == 1) {
        if (j == 0) {
            return f10;
        }
        if (j == 1) {
            return f11;
        }
    }
    double D1 = f11 - f10;
    double f20 = -0.5 - c.S20->V * dT / d2 / D1;
    double f21 = +0.5 - c.S21->V * dT / d2 / D1;
    if (i == 2) {
        if (j == 0) {
            return f20;
        }
        if (j == 1) {
            return f21;
        }
    }
    double D2 = f21 - f20;
    double f30 = -0.5 - c.S30->V * dT / d3 / (D1 * D2);
    double f31 = +0.5 - c.S31->V * dT / d3 / (D1 * D2);
    if (i == 3) {
        if (j == 0) {
            return f30;
        }
        if (j == 1) {
            return f31;
        }
    }
    return 0.0;
}
double NSSolver::MaxQ(NSSolver::Domain* dm, NSSolver::Cell* c)
{
    double qmax = -1.0E10;
    for (int i3 = 0; i3 < 3; i3++) {
        for (int i2 = 0; i2 < 3; i2++) {
            for (int i1 = 0; i1 < 3; i1++) {
                int ii1 = (c->I1 + i1 - 1 + N1) % N1;
                int ii2 = (c->I2 + i2 - 1 + N2) % N2;
                int ii3 = (c->I3 + i3 - 1 + N3) % N3;
                qmax = max(qmax, dm->AC[ii1][ii2][ii3].Q);
            }
        }
    }
    return qmax;
}
double NSSolver::MinQ(NSSolver::Domain* dm, NSSolver::Cell* c)
{
    double qmin = +1.0E10;
    for (int i3 = 0; i3 < 3; i3++) {
        for (int i2 = 0; i2 < 3; i2++) {
            for (int i1 = 0; i1 < 3; i1++) {
                int ii1 = (c->I1 + i1 - 1 + N1) % N1;
                int ii2 = (c->I2 + i2 - 1 + N2) % N2;
                int ii3 = (c->I3 + i3 - 1 + N3) % N3;
                qmin = min(qmin, dm->AC[ii1][ii2][ii3].Q);
            }
        }
    }
    return qmin;
}
double NSSolver::I(NSSolver::Cell* c, double x10, double x11, double x20, double x21, double x30, double x31)
{
    return c->Q + 0.5 * (x10 + x11) * c->G1 * d1 + 0.5 * (x20 + x21) * c->G2 * d2 + 0.5 * (x30 + x31) * c->G3 * d3;
}
double* NSSolver::FormFunction(double* x, double ix, double aq, double q0, double q1, int ix0, int ix1, double qmax, double qmin)
{
    double* q = new double[3];

    if (((ix < x[0]) || (ix > x[3])) || ((ix0 != 0) && (ix1 != 0))) {
        for (int i = 0; i < 3; i++) {
            q[i] = aq;
        }
        return q;
    }
    if (ix < x[1]) {
        if (ix1 == 0) {
            q[1] = q1;
            q[2] = q1;
        } else if (ix0 == 0) {
            q[1] = QD(aq, q0, ix - x[0]);
            Limiter(q[1], qmax, qmin);
            q[2] = q[1];
        }
        q[0] = QD(aq, q[1], (x[2] - x[1]), q[2], (x[3] - x[2]));
        if (Limiter(q[0], qmax, qmin) == 0) {
            return q;
        } else {
            q[1] = QD(aq, q[0], (x[1] - x[0]), q[2], (x[3] - x[2]));
        }
        return q;
    } else if (ix < x[2]) {
        if (ix1 == 0) {
            q[2] = q1;
            q[0] = QD(aq, q1, x[3] - ix);
            Limiter(q[0], qmax, qmin);
        } else if (ix0 == 0) {
            q[0] = q0;
            q[2] = QD(aq, q0, ix - x[0]);
            Limiter(q[2], qmax, qmin);
        }
        q[1] = QD(aq, q[0], (x[1] - x[0]), q[2], (x[3] - x[2]));
        return q;
    } else {
        if (ix1 == 0) {
            q[0] = QD(aq, q1, x[3] - ix);
            Limiter(q[0], qmax, qmin);
            q[1] = q[0];
        } else if (ix0 == 0) {
            q[0] = q0;
            q[1] = q0;
        }
        q[2] = QD(aq, q[0], (x[1] - x[0]), q[1], (x[2] - x[1]));
        if (Limiter(q[2], qmax, qmin) == 0) {
            return q;
        } else {
            q[1] = QD(aq, q[0], (x[1] - x[0]), q[2], (x[3] - x[2]));
        }
        return q;
    }
    delete[] q;
}
double NSSolver::QD(double aq, double q0, double c0)
{
    double q1;
    double c1 = 1.0 - c0;
    if (c1 < 1.0E-10) {
        return aq;
    }
    q1 = (aq - q0 * c0) / c1;
    return q1;
}
double NSSolver::QD(double aq, double q0, double c0, double q1, double c1)
{
    double q2;
    double c2 = 1.0 - c0 - c1;
    if (c2 < 1.0E-10) {
        return aq;
    }
    q2 = (aq - q0 * c0 - q1 * c1) / c2;
    return q2;
}
int NSSolver::Limiter(double& q, double qmax, double qmin)
{
    if (q > qmax) {
        q = qmax;
        return 1;
    } else if (q < qmin) {
        q = qmin;
        return -1;
    }
    return 0;
}
void NSSolver::CorrectionDivergenceErroro(NSSolver::Domain* dm)
{
    foreach(Cell * c, dm->LFC)
    {
        c->Q += ((c->S11->V - c->S10->V) / d1 + (c->S21->V - c->S20->V) / d2 + (c->S31->V - c->S30->V) / d3) * c->Q * dT;
    }
}
void NSSolver::ClearFlux(NSSolver::Domain* dm)
{
    foreach(Cell * c, dm->LC)
    {
        c->S10->F = 0.0;
        c->S20->F = 0.0;
        c->S20->F = 0.0;
    }
}
void NSSolver::xDVofWE()
{
    xDV(&DFV);
}
void NSSolver::xDVofNS()
{
    foreach(Domain * dm, LDF)
    {
        xDV(dm);
    }
}
void NSSolver::xDV(NSSolver::Domain* dm)
{
    foreach(Cell * c, dm->LC)
    {
        c->B = c->Q * DV;
    }
}
void NSSolver::DivergenceofWE()
{
    Divergence(&DFV);
}
void NSSolver::DivergenceofNS()
{
    foreach(Domain * dm, LDF)
    {
        Divergence(dm);
    }
}
void NSSolver::Divergence(NSSolver::Domain* dm)
{
    foreach(Cell * c, dm->LC)
    {
        c->S10->F *= d2 * d3 * dT;
        c->S20->F *= d3 * d1 * dT;
        c->S30->F *= d1 * d2 * dT;
    }
    foreach(Cell * c, dm->LC)
    {
        c->B -= c->S11->F - c->S10->F + c->S21->F - c->S20->F + c->S31->F - c->S30->F;
    }
}
void NSSolver::dDVofWE()
{
    dDV(&DFV);
}
void NSSolver::dDV(NSSolver::Domain* dm)
{
    foreach(Cell * c, dm->LFC)
    {
        c->Q = c->B / DV;
    }
}
void NSSolver::xDR()
{
    foreach(Cell * c, DU1.LC)
    {
        c->Q *= 1.0 + DR * c->CFV->S10->Q;
    }
    foreach(Cell * c, DU2.LC)
    {
        c->Q *= 1.0 + DR * c->CFV->S20->Q;
    }
    foreach(Cell * c, DU3.LC)
    {
        c->Q *= 1.0 + DR * c->CFV->S30->Q;
    }
}
void NSSolver::Diffusion()
{
    foreach(Domain * dm, LDA)
    {
        Diffusion(dm);
    }
}
void NSSolver::Diffusion(NSSolver::Domain* dm)
{
    foreach(Cell * c, dm->LC)
    {
        c->S10->F -= (c->Q - c->C10->Q) / (d1 * Re);
        c->S20->F -= (c->Q - c->C20->Q) / (d2 * Re);
        c->S30->F -= (c->Q - c->C30->Q) / (d3 * Re);
    }
}
void NSSolver::Gravity()
{
    Gravity(&DU3);
}
void NSSolver::Gravity(NSSolver::Domain* dm)
{
    foreach(Cell * c, dm->LC)
    {
        c->B -= c->CFV->S30->Q * DV * dT / (Fn * Fn);
    }
}
void NSSolver::Pressure()
{
    dDV(&DFV);
    int iT = 0;
    double mD = 0;
    double aP = 0.0;
    for (int it = 0; it < MT; it++) {
        aP = 0.0;
        foreach(Cell * c, DPS.LFC)
        {
            aP += c->Q;
        }
        double lfcsize = DPS.LFC.size();
        aP /= lfcsize;
        foreach(Cell * c, DU1.LFC)
        {
            c->CU1->Q = (c->CU1->B - (c->CPS->Q - c->C10->CPS->Q) * d2 * d3 * dT) / DV;
        }
        foreach(Cell * c, DU2.LFC)
        {
            c->CU2->Q = (c->CU2->B - (c->CPS->Q - c->C20->CPS->Q) * d3 * d1 * dT) / DV;
        }
        foreach(Cell * c, DU3.LFC)
        {
            c->CU3->Q = (c->CU3->B - (c->CPS->Q - c->C30->CPS->Q) * d1 * d2 * dT) / DV;
        }
        foreach(Cell * c, DPS.LFC)
        {
            c->B = (c->CU1->C11->Q - c->CU1->Q) / d1 + (c->CU2->C21->Q - c->CU2->Q) / d2 + (c->CU3->C31->Q - c->CU3->Q) / d3;
            c->Q -= DM * c->B * (1 + DR * c->CFV->Q) + aP;
        }
        iT = it;
        mD = 0.0;
        foreach(Cell * c, DPS.LFC)
        {
            double d = fabs(c->B);
            if (d > mD) {
                mD = d;
            }
        }
        if (mD < ED) {
            cout << iT << endl;
            break;
        }
        if (it == MT - 1) {
            cout << "ts=" << ts << "//" << mD << endl;
        }
    }
}
//void NSSolver::plot()
//{
//    Gnuplot::set_GNUPlotPath("/opt/local/bin/");
//    Gnuplot g1;
//}
void NSSolver::DeleteDomain(NSSolver::Domain* dm, int n1, int n2, int n3)
{
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            delete[] dm->AC[i][j];
        }
        delete[] dm->AC[i];
    }
    delete[] dm->AC;
}
void NSSolver::DeleteGrids(NSSolver::Grids* gs, int n1, int n2, int n3)
{
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            delete[] gs->AG[i][j];
        }
        delete[] gs->AG[i];
    }
    delete[] gs->AG;
}