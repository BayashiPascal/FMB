#include "fmb.h"

// ------------- Macros -------------

// Return 1.0 if v is positive, -1.0 if v is negative, 0.0 else
#define sgn(v) (((0.0 < (v)) ? 1.0 : 0.0) - (((v) < 0.0) ? 1.0 : 0.0))


void ElimVar( const int indexVar, const double* A, const double* N, const int& NbIneq, const int& NbVar, double* Ap, double* Np, int& NbIneqp) {

  int i,j,k,l;

  NbIneqp=0;
  for ( i=0; i<NbIneq; ++i ) {

    if ( fabs( A[i*NbVar+indexVar])<EPSILON ) {

      l=0;
      for ( j=0; j<NbVar; ++j ) {

        if ( j!=indexVar ) {

          Ap[NbIneqp*( NbVar-1 )+l]=A[i*NbVar+j];
          ++l;

        }

      }
      Np[NbIneqp]=N[i];
      ++NbIneqp;

    }

  }

  for ( i=0; i<NbIneq-1; ++i ) {

    for ( j=i+1; j<NbIneq; ++j ) {

      if ( ( sgn( A[i*NbVar+indexVar]) != sgn( A[j*NbVar+indexVar]) ) && fabs( A[i*NbVar+indexVar])>EPSILON && fabs( A[j*NbVar+indexVar])>EPSILON ) {

        l=0;
        for ( k=0; k<NbVar; ++k ) {

          if ( k!=indexVar ) {

            Ap[NbIneqp*( NbVar-1 )+l]=A[i*NbVar+k]/fabs( A[i*NbVar+indexVar])+A[j*NbVar+k]/fabs( A[j*NbVar+indexVar]);
            ++l;

          }

        }
        Np[NbIneqp]=N[i]/fabs( A[i*NbVar+indexVar])+N[j]/fabs( A[j*NbVar+indexVar]);
        ++NbIneqp;

      }

    }

  }

}

bool ConfirmSolution( AABB& Co, Frame& Q ) {

  double x;
  double y;
  double z;
  double val;
  int i, a, b ,c;


  for ( a=0; a<2; ++a ) {

    for ( b=0; b<2; ++b ) {

      for ( c=0; c<2; ++c ) {

        x=(double)(1-a);
        y=(double)(1-b);
        z=(double)(1-c);
		val=0.0;
		
        for ( i=0; i<3; ++i ) {

	        val+=Q.A[i]+(x*Co.Min[0]+(1.0-x)*Co.Max[0])*Q.U[i]+(y*Co.Min[1]+(1.0-y)*Co.Max[1])*Q.V[i]+(z*Co.Min[2]+(1.0-z)*Co.Max[2])*Q.W[i];

        }

		if ( val<1.0 ) {
			return true;
		}

      }

    }

  }

	return false;	

}

bool TestTrivialCases ( double* A, double* N, int NbEq, int NbParam ) {
	int i,j;
	double val;
	int sp, n;
	
	for (i=0; i<NbEq; ++i) {
		val=0.0;
		sp=1;
		n=0;
		for (j=0; j<NbParam; ++j) {
			if (fabs(A[i*NbParam+j])<EPSILON) {
				n++;
			} else {
				if (A[i*NbParam+j]<0.0) {
					sp=0;
					val+=A[i*NbParam+j];
				}
			}
		}
		if (n!=NbParam) {
			if (sp==1 && N[i]<0.0) {
				return false;
			}
			if (N[i]<val) {
				return false;
			}
		}
	}
	return true;
}

bool TestCollisionBetween( Frame& P, Frame& Q, AABB& Co , AABB& Cop ) {

  int i;
  double N[100];
  double A[300];
  double Ap[2000];
  double Np[1000];
  double App[1000];
  double Npp[1000];
  int NbEq;
  int Nb;
  int Nbp;
  Frame Qp;

  NbEq=6;

  ChangeBasis( P, Q, Qp );

  for ( i=0; i<3; ++i ) {

    N[2*i]=1.0-Qp.A[i];
    A[6*i]=Qp.U[i]; 
    A[6*i+1]=Qp.V[i]; 
    A[6*i+2]=Qp.W[i];
    N[2*i+1]=Qp.A[i];
    A[6*i+3]=-1.0*Qp.U[i]; 
    A[6*i+4]=-1.0*Qp.V[i]; 
    A[6*i+5]=-1.0*Qp.W[i];

  }

  for ( i=0; i<3; ++i ) {
    N[NbEq]=1.0;
    A[NbEq*3]=0.0;
    A[NbEq*3+1]=0.0;
    A[NbEq*3+2]=0.0;
    A[NbEq*3+i]=1.0;
    NbEq++;    
    N[NbEq]=0.0;
    A[NbEq*3]=0.0;
    A[NbEq*3+1]=0.0;
    A[NbEq*3+2]=0.0;
    A[NbEq*3+i]=-1.0;
    NbEq++;
  }
  
  if ( Q.type==1 ) {

    N[NbEq]=1.0;
    A[NbEq*3]=1.0;
    A[NbEq*3+1]=1.0;
    A[NbEq*3+2]=1.0;
    NbEq++;

  }

  if ( P.type==1 ) {

    N[NbEq]=1.0-Qp.A[0]-Qp.A[1]-Qp.A[2];
    A[NbEq*3]=Qp.U[0]+Qp.V[0]+Qp.W[0];
    A[NbEq*3+1]=Qp.U[1]+Qp.V[1]+Qp.W[1];
    A[NbEq*3+2]=Qp.U[2]+Qp.V[2]+Qp.W[2];
    NbEq++;

  }

	if (TestTrivialCases ( A, N, NbEq, 3 )==false) {
		return false;
	}	

  ElimVar( 0, A, N, NbEq, 3, Ap, Np, Nb );

	if (TestTrivialCases ( Ap, Np, Nb, 2 )==false) {
		return false;
	}	
  ElimVar( 0, Ap, Np, Nb, 2, App, Npp, Nbp );

	if (TestTrivialCases ( App, Npp, Nbp, 1 )==false) {
		return false;
	}	
  GetBound( Co, Nbp, App, Npp, 2 );

  if ( Co.Min[2]>=Co.Max[2]) {

    return false;

  }

  ElimVar( 1, Ap, Np, Nb, 2, App, Npp, Nbp );
	if (TestTrivialCases ( App, Npp, Nbp, 1 )==false) {
		return false;
	}	
  GetBound( Co, Nbp, App, Npp, 1 );

  if ( Co.Min[1]>=Co.Max[1]) {

    return false;

  }

  ElimVar( 1, A, N, NbEq, 3, Ap, Np, Nb );
	if (TestTrivialCases ( Ap, Np, Nb, 2 )==false) {
		return false;
	}	
  ElimVar( 1, Ap, Np, Nb, 2, App, Npp, Nbp );
	if (TestTrivialCases ( App, Npp, Nbp, 1 )==false) {
		return false;
	}	
  GetBound( Co, Nbp, App, Npp, 0 );

  if ( Co.Min[0]>=Co.Max[0]) {

    return false;

  }

	if ( Q.type==1 ) {
		if ( Co.Min[0]+Co.Min[1]+Co.Min[2]>=1.0 ) {
			return false;
		}
	}

	if ( P.type==1 && ConfirmSolution( Co, Qp )==false ) {
		return false;
	}

  FrameExportBdgBox( Q, Co, Cop );

  for ( i=0; i<3; ++i ) {

    if ( Cop.Min[i]<P.BdgArea.Min[i]) {

      Cop.Min[i]=P.BdgArea.Min[i];

    }
    if ( Co.Max[i]>P.BdgArea.Max[i]) {

      Cop.Max[i]=P.BdgArea.Max[i];

    }

  }

  return true;
}

void GetBound(
  AABB& Co, 
  const int Nb, 
  double* A, 
  double *N, 
  int indexVar) {

  int i;

  Co.Min[indexVar]=0.0;
  Co.Max[indexVar]=1.0;

  for ( i=0; i<Nb; ++i ) {
    
    if ( fabs( A[i])>EPSILON ) {

      N[i]=N[i]/fabs( A[i]);

    }

  }
  for ( i=0; i<Nb; ++i ) {

    if ( A[i]>EPSILON ) {

      if ( Co.Max[indexVar]>N[i]) {

        Co.Max[indexVar]=N[i];

      }

    }
    if ( A[i]<-1.0*EPSILON ) {

      if ( Co.Min[indexVar]<-1.0*N[i]) {

        Co.Min[indexVar]=-1.0*N[i];

      }

    }

  }

}

