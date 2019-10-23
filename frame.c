#include "frame.h"

// ------------- Macros -------------

#define EPSILON 0.001

// ------------- Functions declaration -------------

// Power function for integer base and exponent
// Return 'base' ^ 'exp'
int powi(
           int base,
  unsigned int exp);

// ------------- Functions implementation -------------

// Update the inverse components of the Frame 'that'
void FrameUpdateInv(Frame* const that) {

  // Shortcuts
  double** tc  = (double**)(that->comp);
  
  double (*tic)[FRAME_NB_DIM] = that->invComp;

  #if FRAME_NB_DIM == 2

    double det = tc[0][0] * tc[1][1] - tc[1][0] * tc[0][1];
    if (fabs(det) < EPSILON) {
      fprintf(stderr, 
        "FrameUpdateInv: det == 0.0\n");
      exit(1)
    }

    tic[0][0] = tc[1][1] / det;
    tic[0][1] = -1.0 * tc[0][1] / det;
    tic[1][0] = -1.0 * tc[1][0] / det;
    tic[1][1] = tc[0][0] / det;

  #elif FRAME_NB_DIM == 3

    // Update the inverse components
    double det = 
      tc[0][0] * (tc[1][1] * tc[2][2]- tc[1][2] * tc[2][1]) -
      tc[1][0] * (tc[0][1] * tc[2][2]- tc[0][2] * tc[2][1]) +
      tc[2][0] * (tc[0][1] * tc[1][2]- tc[0][2] * tc[1][1]);
    if (fabs(det) < EPSILON) {
      fprintf(stderr, 
        "FrameUpdateInv: det == 0.0\n");
      exit(1);
    }

    tic[0][0] = (tc[1][1]* tc[2][2]- tc[2][1]* tc[1][2]) / det;
    tic[0][1] = (tc[2][1]* tc[0][2]- tc[2][2]* tc[0][1]) / det;
    tic[0][2] = (tc[0][1]* tc[1][2]- tc[0][2]* tc[1][1]) / det;
    tic[1][0] = (tc[2][0]* tc[1][2]- tc[2][2]* tc[1][0]) / det;
    tic[1][1] = (tc[0][0]* tc[2][2]- tc[2][0]* tc[0][2]) / det;
    tic[1][2] = (tc[0][2]* tc[1][0]- tc[1][2]* tc[0][0]) / det;
    tic[2][0] = (tc[1][0]* tc[2][1]- tc[2][0]* tc[1][1]) / det;
    tic[2][1] = (tc[0][1]* tc[2][0]- tc[2][1]* tc[0][0]) / det;
    tic[2][2] = (tc[0][0]* tc[1][1]- tc[1][0]* tc[0][1]) / det;

  #else

    fprintf(stderr, 
      "FrameUpdateInv: unimplemented for %d dimension(s)\n",
      FRAME_NB_DIM);
    exit(1)

  #endif

}

// Project the Frame 'Q' in the Frame 'P' 's coordinates system and 
// memorize the result in the Frame 'Qp'
void FrameImportFrame(
  const Frame* const P, 
  const Frame* const Q, 
        Frame* const Qp) {

  // Shortcuts
  const double*  qo  = Q->orig;
        double*  qpo = Qp->orig;
  const double*  po  = P->orig;

  const double  (*pi)[FRAME_NB_DIM] = P->invComp;
        double (*qpc)[FRAME_NB_DIM] = Qp->comp;
  const double  (*qc)[FRAME_NB_DIM] = Q->comp;

  // Calculate the projection
  double v[FRAME_NB_DIM];
  for (int i = FRAME_NB_DIM;
       i--;) {

    v[i] = qo[i] - po[i];

  }
  
  for (int i = FRAME_NB_DIM;
       i--;) {

    qpo[i] = 0.0;

    for (int j = FRAME_NB_DIM;
         j--;) {

      qpo[i] += pi[j][i] * v[j];
      qpc[j][i] = 0.0;

      for (int k = FRAME_NB_DIM;
           k--;) {

        qpc[j][i] += pi[k][i] * qc[k][j];

      }
    }
  }
}

// Export the AABB 'bdgBox' from 'that' 's coordinates system to
// the real coordinates system and update 'bdgBox' with the resulting
// AABB
void FrameExportBdgBox(
  const Frame* const that,
   const AABB* const bdgBox,
         AABB* const bdgBoxProj) {

  // Shortcuts
  const double*  to    = that->orig;
  const double*  bbmi  = bdgBox->min;
  const double*  bbma  = bdgBox->max;
        double*  bbpmi = bdgBoxProj->min;
        double*  bbpma = bdgBoxProj->max;

  const double  (*tc)[FRAME_NB_DIM] = that->comp;

  // Initialise the coordinates of the result AABB with the projection
  // of the origin of the AABB in argument
  for (int i = FRAME_NB_DIM;
       i--;) {

    bbpmi[i] = bbpma[i] = to[i];

    for (int j = FRAME_NB_DIM;
         j--;) {

      double v = tc[j][i] * bbmi[j];
      bbpmi[i] += v;
      bbpma[i] += v;

    }
  }

  // Loop on vertices of the AABB
  // skip the first vertex which is the origin already computed above
  int nbVertices = powi(2, FRAME_NB_DIM);
  for (int iVertex = nbVertices;
       iVertex-- && iVertex;) {
    
    // Declare a variable to memorize the coordinates of the vertex in
    // 'that' 's coordinates system
    double v[FRAME_NB_DIM];
    
    // Calculate the coordinates of the vertex in
    // 'that' 's coordinates system
    for (int i = FRAME_NB_DIM;
         i--;) {

      v[i] = ((iVertex & (1 << i)) ? bbma[i] : bbmi[i]);

    }
    
    // Declare a variable to memorize the projected coordinates
    double w[FRAME_NB_DIM];
    
    // Project the vertex to real coordinates system
    for (int i = FRAME_NB_DIM;
         i--;) {

      w[i] = to[i];

      for (int j = FRAME_NB_DIM;
           j--;) {

        w[i] += tc[j][i] * v[j];

      }
    }

    // Update the coordinates of the result AABB
    for (int i = FRAME_NB_DIM; 
         i--;) {

      if (bbpmi[i] > w[i]) {

        bbpmi[i] = w[i];

      }
      if (bbpma[i] < w[i]) {

        bbpma[i] = w[i];

      }
    }
  }

}

// Print the AABB 'that' on stdout
// Output format is (min[0], min[1], ...)-(max[0], max[1], ...)
void AABBPrint(const AABB* const that) {
  
  printf("(");
  for (int i = 0;
       i < FRAME_NB_DIM;
       ++i) {

    printf("%f", that->min[i]);
    if (i < FRAME_NB_DIM - 1)
      printf(",");

  }
  printf(")-(");
  for (int i = 0;
       i < FRAME_NB_DIM;
       ++i) {

    printf("%f", that->max[i]);
    if (i < FRAME_NB_DIM - 1)
      printf(",");

  }
  printf(")");
  
}

// Power function for integer base and exponent
// Return 'base' ^ 'exp'
int powi(
           int base,
  unsigned int exp) {

    int res = 1;
    for (;
         exp;
         --exp) {

      res *= base; 

    }
    return res;
}
