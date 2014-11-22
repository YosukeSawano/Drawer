//
//  NSSolver.h
//  MyCFD
//
//  Created by 澤野 陽介 on 2014/03/05.
//  Copyright (c) 2014年 澤野 陽介. All rights reserved.
//

#ifndef __MyCFD__NSSolver__
#define __MyCFD__NSSolver__

#include <iostream>
#include <list>
#include "Vertices.h"

class NSSolver{
public:
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
        Wave,
		Exchange
    };
    enum RDistribution RD;
    struct Surface{
        double Q;
        double V;
        double F;
        double G;
    };
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
        Surface *S10;
        Surface *S11;
        Surface *S20;
        Surface *S21;
        Surface *S30;
        Surface *S31;
        Surface ss10;
        Surface ss11;
        Surface ss20;
        Surface ss21;
        Surface ss30;
        Surface ss31;
        Cell *C10;
        Cell *C11;
        Cell *C20;
        Cell *C21;
        Cell *C30;
        Cell *C31;
        Cell *CPS;
        Cell *CU1;
        Cell *CU2;
        Cell *CU3;
        Cell *CFV;
    };
    struct DBoundary{
        Cell *CD;
    };
    struct NBoundary{
        Cell *CF;
        Cell *CB;
    };
    struct Domain{
        std::string Name;
		int timestep;
        double N1;
        double N2;
        double N3;
        Cell ***AC;
        std::list<Cell*>LC;
        std::list<Cell*>LFC;
        std::list<Cell*>LCI;
        std::list<Cell*>LCI1;
        std::list<Cell*>LCI2;
        std::list<Cell*>LCI3;
        std::list<DBoundary*>LDBC;
        std::list<NBoundary*>LNBC;
    };
    struct Grids{
        std::string Name;
        Domain D;
        int P;
        int Ts;
        int I1;
        int I2;
        int I3;
        int N1;
        int N2;
        int N3;
        double Q;
        Grids ***AG;
        std::list<Grids*>LG;
    };
    Domain DPS;
    Domain DU1;
    Domain DU2;
    Domain DU3;
    Domain DFV;
    std::list<Domain*>LDA;
    std::list<Domain*>LDV;
    std::list<Domain*>LDF;
    Grids GPS;
    Grids GU1;
    Grids GU2;
    Grids GU3;
    Grids GFV;
    std::list<Grids>LGA;
    NBoundary bN;
    DBoundary bD;
	bool bMakeDomain;
    void plot();
    void SetIniN();
    void SetIniW();
    void SetDomain();
    void SetGridsofNS();
    void SetGridsofWE();
    void SetFluidCellofWE();
    void SetFluidCellofNS();
    void SetInitialConditionofRho();
    void SetVelocity();
    void AverageSurface();
    void SetSurfaceVelosity();
    void MakeDomain(std::string Name, Domain *dm,int n1,int n2,int n3);
    void DeleteDomain(Domain *dm, int n1, int n2, int n3);
    void MakeListofDomain();
    void MakeGrids(std::string Name, Grids *gs,Domain *dm,int p, int n10,int n11, int n20, int n21, int n30, int n31);
    void DeleteGrids(Grids *gs, int n1,int n2, int n3);
    void MakeListofGrids();
    void ConnectDomains();
    void SetBoundary();
    void FluidCell(Domain *dm,int IF10, int IF11, int IF20, int IF21, int IF30, int IF31);
    void BNeumann(Domain *dm, int IB3, int IF3, enum plane ID);
    void BDirichlet(Domain *dm, int ID3, enum plane ID);
    void NonSlip();
    void ImposeNC(Domain *dm);
    void ImposeDC(Domain *dm);
    void ImposeZC(Domain *dm);
    void SetZero();
    void SetZero(Domain *dm);
    void WritetoConsoleofWE();
    void WritetoConsole(Domain *dm);
    void AdvectionofWE();
    void AdvectionofNS();
    void Advection(Domain *dm);
    void Center(Domain *dm);
    void Upwind(Domain *dm);
    void Quick(Domain *dm);
    void Gradient(Domain *dm);
    void PieceWiseLinear(Domain *dm);
    void InterfaceLocation(Domain *dm);
    void InterfaceGradient(Domain *dm);
    void DirectioFreePiecewiseLinear(Domain *dm);
    void DivideCell(const Cell c, double*x1, double**x2, double***x3);
    double F(const Cell c,int i, int j);
    double MaxQ(Domain *dm, Cell *c);
    double MinQ(Domain *dm, Cell *c);
    double I(Cell *c, double x10, double x11, double x20, double x21, double x30, double x31);
    void IP(double g1, double g2, double ix, double &ix1, double &ix2);
    double* FormFunction(double *x,double ix, double aq, double q0, double q1, int ix0, int ix1, double qmax, double qmin);
    double QD(double aq, double q0, double c0);
    double QD(double aq, double q0, double c0, double q1, double c1);
    int Limiter(double &q, double qmax, double qmin);
    void CorrectionDivergenceErroro(Domain *dm);
    void ClearFlux(Domain *dm);
    void xDVofWE();
    void xDVofNS();
    void xDV(Domain *dm);
    void DivergenceofWE();
    void DivergenceofNS();
    void Divergence(Domain *dm);
    void dDVofWE();
    void dDV(Domain *dm);
    void xDR();
    void xDR(Domain *dm);
    void Diffusion();
    void Diffusion(Domain *dm);
    void Gravity();
    void Gravity(Domain *dm);
    void Pressure();
	
    NSSolver();
    ~NSSolver();
	void SetConditions();
    void nssolver(std::list<Vertices> &ldm);
    void WETest();
	
	void OutputDomains(std::list<Domain> &OutDomains);
	void OutputDomains(Vertices &vertices);
	void PushbackVertsObj(Vertices &vertex);
};

#endif /* defined(__MyCFD__NSSolver__) */
