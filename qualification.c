// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>


// Include FMB and SAT algorithm library
#include "fmb.h"
#include "sat.h"

#define EPSILON 0.0001
#define RANGE_AXIS 100.0
#define NB_TESTS 100
#define NB_REPEAT 100

#define rnd() (float)(rand())/(float)(RAND_MAX)

typedef struct {
  FrameType type;
  double orig[FRAME_NB_DIM];
  double comp[FRAME_NB_DIM][FRAME_NB_DIM];
} Param;

double sum = 0.0;
unsigned long count = 0;

void Qualification(
        const Param paramP,
        const Param paramQ) {

  Frame P = 
    FrameCreateStatic(
      paramP.type,
      paramP.orig,
      paramP.comp);
  
  Frame Q = 
    FrameCreateStatic(
      paramQ.type,
      paramQ.orig,
      paramQ.comp);

  Frame* that = &P;
  Frame* tho = &Q;
  
  for (int iTest = 0;
       iTest < 2;
       ++iTest) {

    struct timeval start;
    gettimeofday(&start, NULL);

    bool isIntersectingFMB[NB_REPEAT] = {false};
    for (int i = NB_REPEAT;
         i--;) {

      isIntersectingFMB[i] = 
        FMBTestIntersection(
          that, 
          tho, 
          NULL);
    }

    struct timeval stop;
    gettimeofday(&stop, NULL);
    double deltausFMB = (double)(stop.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL);

    bool isIntersectingSAT[NB_REPEAT] = {false};
    for (int i = NB_REPEAT;
         i--;) {

#if FRAME_NB_DIM == 2

    isIntersectingSAT[i] = 
      SATTestIntersection2D(
        that, 
        tho);

#elif FRAME_NB_DIM == 3

    isIntersectingSAT[i] = 
      SATTestIntersection3D(
        that, 
        tho);

#else

  printf("Not implemented for dimension %d\n", FRAME_NB_DIM);
  exit(0);

#endif
    }

    gettimeofday(&stop, NULL);
    double deltausSAT = (double)(stop.tv_usec - start.tv_usec);

    sum += deltausFMB / deltausSAT;
    ++count;

    for (int i = NB_REPEAT;
         i--;) {

      if (isIntersectingFMB[i] != isIntersectingSAT[i]) {
        printf("Validation has failed\n");
        FramePrint(that);
        printf(" against ");
        FramePrint(tho);
        printf("\n");
        printf("FMB : ");
        if (isIntersectingFMB == false) {
          printf("no ");
        }
        printf("intersection\n");
        printf("SAT : ");
        if (isIntersectingSAT == false) {
          printf("no ");
        }
        printf("intersection\n");
        exit(0);
      }

    }

    that = &Q;
    tho = &P;

  }

}

// Main function
int main(int argc, char** argv) {

  srandom(time(NULL));
  Param paramP;
  Param paramQ;

  for (unsigned long iTest = NB_TESTS;
       iTest--;) {

    Param* param = &paramP;
    for (int iParam = 2;
         iParam--;) {

      if (rnd() < 0.5) {

        param->type = FrameCuboid;

      } else {

        param->type = FrameTetrahedron;

      }
      for (int iAxis = FRAME_NB_DIM;
           iAxis--;) {

        param->orig[iAxis] = -RANGE_AXIS + 2.0 * rnd() * RANGE_AXIS;

        for (int iComp = FRAME_NB_DIM;
             iComp--;) {

          param->comp[iComp][iAxis] = 
            -RANGE_AXIS + 2.0 * rnd() * RANGE_AXIS;

        }

      }
      
      param = &paramQ;

    }

#if FRAME_NB_DIM == 2

    double detP = 
      paramP.comp[0][0] * paramP.comp[1][1] - 
      paramP.comp[1][0] * paramP.comp[0][1];

    double detQ = 
      paramQ.comp[0][0] * paramQ.comp[1][1] - 
      paramQ.comp[1][0] * paramQ.comp[0][1];

#elif FRAME_NB_DIM == 3

    double detP = 
      paramP.comp[0][0] * (paramP.comp[1][1] * paramP.comp[2][2]-
      paramP.comp[1][2] * paramP.comp[2][1]) -
      paramP.comp[1][0] * (paramP.comp[0][1] * paramP.comp[2][2]- 
      paramP.comp[0][2] * paramP.comp[2][1]) +
      paramP.comp[2][0] * (paramP.comp[0][1] * paramP.comp[1][2]- 
      paramP.comp[0][2] * paramP.comp[1][1]);

    double detQ = 
      paramQ.comp[0][0] * (paramQ.comp[1][1] * paramQ.comp[2][2]-
      paramQ.comp[1][2] * paramQ.comp[2][1]) -
      paramQ.comp[1][0] * (paramQ.comp[0][1] * paramQ.comp[2][2]- 
      paramQ.comp[0][2] * paramQ.comp[2][1]) +
      paramQ.comp[2][0] * (paramQ.comp[0][1] * paramQ.comp[1][2]- 
      paramQ.comp[0][2] * paramQ.comp[1][1]);

#else

  printf("Not implemented for dimension %d\n", FRAME_NB_DIM);
  exit(0);

#endif

    if (fabs(detP) > EPSILON && fabs(detQ) > EPSILON) {

      Qualification(
        paramP,
        paramQ);

    }

  }

  double avg = sum / (double)count;
  printf("avg(timeFMB / timeSAT) = %f\n", avg);

  return 0;
}
