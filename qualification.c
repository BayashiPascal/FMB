// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

// Include FMB and SAT algorithm library
#include "fmb.h"
#include "sat.h"

// Espilon for numerical precision
#define EPSILON 0.0001
// Range of values for the random generation of Frames
#define RANGE_AXIS 100.0
// Nb of run
#define NB_RUNS 10
// Nb of tests per run
#define NB_TESTS 1000000
// Nb of times the test is run on one pair of frame, used to 
// slow down the processus and be able to measure time
#if FRAME_NB_DIM == 3
  #define NB_REPEAT 800
#elif FRAME_NB_DIM == 2
  #define NB_REPEAT 1500
#endif

// Helper macro to generate random number in [0.0, 1.0]
#define rnd() (double)(rand())/(double)(RAND_MAX)

// Helper structure to pass arguments to the Qualification function
typedef struct {
  FrameType type;
  double orig[FRAME_NB_DIM];
  double comp[FRAME_NB_DIM][FRAME_NB_DIM];
} Param;

// Global variables to count nb of tests resulting in intersection
// and no intersection, and min/max/total time of execution for each
double minInter;
double maxInter;
double sumInter;
unsigned long countInter;
double minNoInter;
double maxNoInter;
double sumNoInter;
unsigned long countNoInter;

// Qualification function
// Takes two Frame definition as input, run the intersection test on 
// them with FMB and SAT, and measure the time of execution of each
void Qualification(
        const Param paramP,
        const Param paramQ) {

  // Create the two Frames
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

  // Helper variables to loop on the pair (that, tho) and (tho, that)
  Frame* that = &P;
  Frame* tho = &Q;

  // Loop on pairs of Frames
  for (int iPair = 2;
       iPair--;) {

    // Declare an array to memorize the results of the repeated
    // test on the same pair,
    // to prevent optimization from the compiler to remove the for loop
    bool isIntersectingFMB[NB_REPEAT] = {false};

    // Start measuring time
    struct timeval start;
    gettimeofday(&start, NULL);

    // Run the FMB intersection test
    for (int i = NB_REPEAT;
         i--;) {

      isIntersectingFMB[i] = 
        FMBTestIntersection(
          that, 
          tho, 
          NULL);
    }

    // Stop measuring time
    struct timeval stop;
    gettimeofday(&stop, NULL);

    // Calculate the delay of execution
    unsigned long deltausFMB = 0;
    if (stop.tv_sec < start.tv_sec) {
      printf("time warps, try again\n");
      exit(0);
    }
    if (stop.tv_sec > start.tv_sec + 1) {
      printf("deltausFMB >> 1s, decrease NB_REPEAT\n");
      exit(0);
    }
    if (stop.tv_usec < start.tv_usec) {
      deltausFMB = stop.tv_sec - start.tv_sec;
      deltausFMB += stop.tv_usec + 1000000 - start.tv_usec;
    } else {
      deltausFMB = stop.tv_usec - start.tv_usec;
    }

    // Declare an array to memorize the results of the repeated
    // test on the same pair,
    // to prevent optimization from the compiler to remove the for loop
    bool isIntersectingSAT[NB_REPEAT] = {false};

    // Start measuring time
    gettimeofday(&start, NULL);

    // Run the FMB intersection test
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

    // Stop measuring time
    gettimeofday(&stop, NULL);

    // Calculate the delay of execution
    unsigned long deltausSAT = 0;
    if (stop.tv_sec < start.tv_sec) {
      printf("time warps, try again\n");
      exit(0);
    }
    if (stop.tv_sec > start.tv_sec + 1) {
      printf("deltausSAT >> 1s, decrease NB_REPEAT\n");
      exit(0);
    }
    if (stop.tv_usec < start.tv_usec) {
      deltausSAT = stop.tv_sec - start.tv_sec;
      deltausSAT += stop.tv_usec + 1000000 - start.tv_usec;
    } else {
      deltausSAT = stop.tv_usec - start.tv_usec;
    }

    // If the delays are greater than 10ms
    if (deltausFMB >= 10 && deltausSAT >= 10) {

      // If FMB and SAT disagrees
      if (isIntersectingFMB[0] != isIntersectingSAT[0]) {

        printf("Qualification has failed\n");
        FramePrint(that);
        printf(" against ");
        FramePrint(tho);
        printf("\n");
        printf("FMB : ");
        if (isIntersectingFMB == false)
          printf("no ");
        printf("intersection\n");
        printf("SAT : ");
        if (isIntersectingSAT == false)
          printf("no ");
        printf("intersection\n");

        // Stop the qualification test
        exit(0);

      }

      // Get the ratio of execution time
      double ratio = ((double)deltausFMB) / ((double)deltausSAT);

      // If the Frames intersect
      if (isIntersectingSAT[0] == true) {

        // Update the counters
        if (countInter == 0) {

          minInter = ratio;
          maxInter = ratio;

        } else {

          if (minInter > ratio)
            minInter = ratio;
          if (maxInter < ratio)
            maxInter = ratio;

        }
        sumInter += ratio;
        ++countInter;

      // Else, the Frames do not intersect
      } else {

        // Update the counters
        if (countNoInter == 0) {

          minNoInter = ratio;
          maxNoInter = ratio;

        } else {

          if (minNoInter > ratio)
            minNoInter = ratio;
          if (maxNoInter < ratio)
            maxNoInter = ratio;

        }
        sumNoInter += ratio;
        ++countNoInter;

      }

    // Else, if time of execution for FMB was less than a 10ms
    } else if (deltausFMB < 10) {

      printf("deltausFMB < 10ms, increase NB_REPEAT\n");
      exit(0);

    // Else, if time of execution for SAT was less than a 10ms
    } else if (deltausSAT < 10) {

      printf("deltausSAT < 10ms, increase NB_REPEAT\n");
      exit(0);

    }

    // Flip the pair of Frames
    that = &Q;
    tho = &P;

  }

}

// Main function
int main(int argc, char** argv) {

  // Initialise the random generator
  srandom(time(NULL));

  // Loop on runs
  for (int iRun = 0;
       iRun < NB_RUNS;
       ++iRun) {

    // Initialize counters
    minInter = 0.0;
    maxInter = 0.0;
    sumInter = 0.0;
    countInter = 0;
    minNoInter = 0.0;
    maxNoInter = 0.0;
    sumNoInter = 0.0;
    countNoInter = 0;

    // Declare two variables to memozie the arguments to the
    // Qualification function
    Param paramP;
    Param paramQ;

    // Loop on the number of tests
    for (unsigned long iTest = NB_TESTS;
         iTest--;) {

      // Create two random Frame definitions
      Param* param = &paramP;
      for (int iParam = 2;
           iParam--;) {

        // 50% chance of being a Cuboid or a Tetrahedron
        if (rnd() < 0.5)
          param->type = FrameCuboid;
        else
          param->type = FrameTetrahedron;

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

      // Calculate the determinant of the Frames' components matrix
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

      // If the determinants are not null, ie the Frame are not degenerate
      if (fabs(detP) > EPSILON && fabs(detQ) > EPSILON) {

        // Run the validation on the two Frames
        Qualification(
          paramP,
          paramQ);

      }

    }

    // Display the results
    if (iRun == 0) {

      printf("ratio (timeFMB / timeSAT)\n");
      printf("run\tcountInter\tcountNoInter\t");
      printf("minInter\tavgInter\tmaxInter\t");
      printf("minNoInter\tavgNoInter\tmaxNoInter\t");
      printf("minTotal\tavgTotal\tmaxTotal\n");

    }

    printf("%d\t%lu\t%lu\t", iRun, countInter, countNoInter);

    double avgInter = sumInter / (double)countInter;
    printf("%f\t%f\t%f\t", minInter, avgInter, maxInter);

    double avgNoInter = sumNoInter / (double)countNoInter;
    printf("%f\t%f\t%f\t", minNoInter, avgNoInter, maxNoInter);

    double avg = 
      (sumInter + sumNoInter) / (double)(countInter + countNoInter);
    printf("%f\t%f\t%f\n", 
      (minNoInter < minInter ? minNoInter : minInter), 
      avg,
      (maxNoInter > maxInter ? maxNoInter : maxInter));

  }

  return 0;

}
