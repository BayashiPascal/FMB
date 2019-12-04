// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

// Include FMB and SAT algorithm library
#include "fmb2d.h"
#include "fmb3d.h"
#include "sat.h"

// Espilon for numerical precision
#define EPSILON 0.0001
// Range of values for the random generation of Frames
#define RANGE_AXIS 100.0
// Nb of run
#define NB_RUNS 1
// Nb of tests per run
#define NB_TESTS 1000000
// Nb of times the test is run on one pair of frame, used to 
// slow down the processus and be able to measure time
#define NB_REPEAT_3D 800
#define NB_REPEAT_2D 1500

// Helper macro to generate random number in [0.0, 1.0]
#define rnd() (double)(rand())/(double)(RAND_MAX)

// Helper structure to pass arguments to the Qualification function
typedef struct {
  FrameType type;
  double orig[2];
  double comp[2][2];
} Param2D;
typedef struct {
  FrameType type;
  double orig[3];
  double comp[3][3];
} Param3D;

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

double minInterCC;
double maxInterCC;
double sumInterCC;
unsigned long countInterCC;
double minNoInterCC;
double maxNoInterCC;
double sumNoInterCC;
unsigned long countNoInterCC;

double minInterCT;
double maxInterCT;
double sumInterCT;
unsigned long countInterCT;
double minNoInterCT;
double maxNoInterCT;
double sumNoInterCT;
unsigned long countNoInterCT;

double minInterTC;
double maxInterTC;
double sumInterTC;
unsigned long countInterTC;
double minNoInterTC;
double maxNoInterTC;
double sumNoInterTC;
unsigned long countNoInterTC;

double minInterTT;
double maxInterTT;
double sumInterTT;
unsigned long countInterTT;
double minNoInterTT;
double maxNoInterTT;
double sumNoInterTT;
unsigned long countNoInterTT;

// Qualification function
// Takes two Frame definition as input, run the intersection test on 
// them with FMB and SAT, and measure the time of execution of each
void Qualification2DStatic(
        const Param2D paramP,
        const Param2D paramQ) {

  // Create the two Frames
  Frame2D P = 
    Frame2DCreateStatic(
      paramP.type,
      paramP.orig,
      paramP.comp);
  
  Frame2D Q = 
    Frame2DCreateStatic(
      paramQ.type,
      paramQ.orig,
      paramQ.comp);

  // Helper variables to loop on the pair (that, tho) and (tho, that)
  Frame2D* that = &P;
  Frame2D* tho = &Q;

  // Loop on pairs of Frames
  for (int iPair = 2;
       iPair--;) {

    // Declare an array to memorize the results of the repeated
    // test on the same pair,
    // to prevent optimization from the compiler to remove the for loop
    bool isIntersectingFMB[NB_REPEAT_2D] = {false};

    // Start measuring time
    struct timeval start;
    gettimeofday(&start, NULL);

    // Run the FMB intersection test
    for (int i = NB_REPEAT_2D;
         i--;) {

      isIntersectingFMB[i] = 
        FMBTestIntersection2D(
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
    bool isIntersectingSAT[NB_REPEAT_2D] = {false};

    // Start measuring time
    gettimeofday(&start, NULL);

    // Run the FMB intersection test
    for (int i = NB_REPEAT_2D;
         i--;) {

      isIntersectingSAT[i] = 
        SATTestIntersection2D(
          that, 
          tho);

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
        Frame2DPrint(that);
        printf(" against ");
        Frame2DPrint(tho);
        printf("\n");
        printf("FMB : ");
        if (isIntersectingFMB[0] == false)
          printf("no ");
        printf("intersection\n");
        printf("SAT : ");
        if (isIntersectingSAT[0] == false)
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

        if (paramP.type == FrameCuboid && 
            paramQ.type == FrameCuboid) {

          if (countInterCC == 0) {

            minInterCC = ratio;
            maxInterCC = ratio;

          } else {

            if (minInterCC > ratio)
              minInterCC = ratio;
            if (maxInterCC < ratio)
              maxInterCC = ratio;

          }
          sumInterCC += ratio;
          ++countInterCC;

        } else if (paramP.type == FrameCuboid && 
                   paramQ.type == FrameTetrahedron) {

          if (countInterCT == 0) {

            minInterCT = ratio;
            maxInterCT = ratio;

          } else {

            if (minInterCT > ratio)
              minInterCT = ratio;
            if (maxInterCT < ratio)
              maxInterCT = ratio;

          }
          sumInterCT += ratio;
          ++countInterCT;

        } else if (paramP.type == FrameTetrahedron && 
                   paramQ.type == FrameCuboid) {

          if (countInterTC == 0) {

            minInterTC = ratio;
            maxInterTC = ratio;

          } else {

            if (minInterTC > ratio)
              minInterTC = ratio;
            if (maxInterTC < ratio)
              maxInterTC = ratio;

          }
          sumInterTC += ratio;
          ++countInterTC;

        } else if (paramP.type == FrameTetrahedron && 
                   paramQ.type == FrameTetrahedron) {
              
          if (countInterTT == 0) {

            minInterTT = ratio;
            maxInterTT = ratio;

          } else {

            if (minInterTT > ratio)
              minInterTT = ratio;
            if (maxInterTT < ratio)
              maxInterTT = ratio;

          }
          sumInterTT += ratio;
          ++countInterTT;

        }

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

        if (paramP.type == FrameCuboid && 
            paramQ.type == FrameCuboid) {

          if (countNoInterCC == 0) {

            minNoInterCC = ratio;
            maxNoInterCC = ratio;

          } else {

            if (minNoInterCC > ratio)
              minNoInterCC = ratio;
            if (maxNoInterCC < ratio)
              maxNoInterCC = ratio;

          }
          sumNoInterCC += ratio;
          ++countNoInterCC;

        } else if (paramP.type == FrameCuboid && 
                   paramQ.type == FrameTetrahedron) {

          if (countNoInterCT == 0) {

            minNoInterCT = ratio;
            maxNoInterCT = ratio;

          } else {

            if (minNoInterCT > ratio)
              minNoInterCT = ratio;
            if (maxNoInterCT < ratio)
              maxNoInterCT = ratio;

          }
          sumNoInterCT += ratio;
          ++countNoInterCT;

        } else if (paramP.type == FrameTetrahedron && 
                   paramQ.type == FrameCuboid) {

          if (countNoInterTC == 0) {

            minNoInterTC = ratio;
            maxNoInterTC = ratio;

          } else {

            if (minNoInterTC > ratio)
              minNoInterTC = ratio;
            if (maxNoInterTC < ratio)
              maxNoInterTC = ratio;

          }
          sumNoInterTC += ratio;
          ++countNoInterTC;

        } else if (paramP.type == FrameTetrahedron && 
                   paramQ.type == FrameTetrahedron) {

          if (countNoInterTT == 0) {

            minNoInterTT = ratio;
            maxNoInterTT = ratio;

          } else {

            if (minNoInterTT > ratio)
              minNoInterTT = ratio;
            if (maxNoInterTT < ratio)
              maxNoInterTT = ratio;

          }
          sumNoInterTT += ratio;
          ++countNoInterTT;

        } 
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

void Qualification3DStatic(
        const Param3D paramP,
        const Param3D paramQ) {

  // Create the two Frames
  Frame3D P = 
    Frame3DCreateStatic(
      paramP.type,
      paramP.orig,
      paramP.comp);
  
  Frame3D Q = 
    Frame3DCreateStatic(
      paramQ.type,
      paramQ.orig,
      paramQ.comp);

  // Helper variables to loop on the pair (that, tho) and (tho, that)
  Frame3D* that = &P;
  Frame3D* tho = &Q;

  // Loop on pairs of Frames
  for (int iPair = 2;
       iPair--;) {

    // Declare an array to memorize the results of the repeated
    // test on the same pair,
    // to prevent optimization from the compiler to remove the for loop
    bool isIntersectingFMB[NB_REPEAT_3D] = {false};

    // Start measuring time
    struct timeval start;
    gettimeofday(&start, NULL);

    // Run the FMB intersection test
    for (int i = NB_REPEAT_3D;
         i--;) {

      isIntersectingFMB[i] = 
        FMBTestIntersection3D(
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
    bool isIntersectingSAT[NB_REPEAT_3D] = {false};

    // Start measuring time
    gettimeofday(&start, NULL);

    // Run the FMB intersection test
    for (int i = NB_REPEAT_3D;
         i--;) {

      isIntersectingSAT[i] = 
        SATTestIntersection3D(
          that, 
          tho);

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
        Frame3DPrint(that);
        printf(" against ");
        Frame3DPrint(tho);
        printf("\n");
        printf("FMB : ");
        if (isIntersectingFMB[0] == false)
          printf("no ");
        printf("intersection\n");
        printf("SAT : ");
        if (isIntersectingSAT[0] == false)
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

        if (paramP.type == FrameCuboid && 
            paramQ.type == FrameCuboid) {

          if (countInterCC == 0) {

            minInterCC = ratio;
            maxInterCC = ratio;

          } else {

            if (minInterCC > ratio)
              minInterCC = ratio;
            if (maxInterCC < ratio)
              maxInterCC = ratio;

          }
          sumInterCC += ratio;
          ++countInterCC;

        } else if (paramP.type == FrameCuboid && 
                   paramQ.type == FrameTetrahedron) {

          if (countInterCT == 0) {

            minInterCT = ratio;
            maxInterCT = ratio;

          } else {

            if (minInterCT > ratio)
              minInterCT = ratio;
            if (maxInterCT < ratio)
              maxInterCT = ratio;

          }
          sumInterCT += ratio;
          ++countInterCT;

        } else if (paramP.type == FrameTetrahedron && 
                   paramQ.type == FrameCuboid) {

          if (countInterTC == 0) {

            minInterTC = ratio;
            maxInterTC = ratio;

          } else {

            if (minInterTC > ratio)
              minInterTC = ratio;
            if (maxInterTC < ratio)
              maxInterTC = ratio;

          }
          sumInterTC += ratio;
          ++countInterTC;

        } else if (paramP.type == FrameTetrahedron && 
                   paramQ.type == FrameTetrahedron) {
              
          if (countInterTT == 0) {

            minInterTT = ratio;
            maxInterTT = ratio;

          } else {

            if (minInterTT > ratio)
              minInterTT = ratio;
            if (maxInterTT < ratio)
              maxInterTT = ratio;

          }
          sumInterTT += ratio;
          ++countInterTT;

        }

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

        if (paramP.type == FrameCuboid && 
            paramQ.type == FrameCuboid) {

          if (countNoInterCC == 0) {

            minNoInterCC = ratio;
            maxNoInterCC = ratio;

          } else {

            if (minNoInterCC > ratio)
              minNoInterCC = ratio;
            if (maxNoInterCC < ratio)
              maxNoInterCC = ratio;

          }
          sumNoInterCC += ratio;
          ++countNoInterCC;

        } else if (paramP.type == FrameCuboid && 
                   paramQ.type == FrameTetrahedron) {

          if (countNoInterCT == 0) {

            minNoInterCT = ratio;
            maxNoInterCT = ratio;

          } else {

            if (minNoInterCT > ratio)
              minNoInterCT = ratio;
            if (maxNoInterCT < ratio)
              maxNoInterCT = ratio;

          }
          sumNoInterCT += ratio;
          ++countNoInterCT;

        } else if (paramP.type == FrameTetrahedron && 
                   paramQ.type == FrameCuboid) {

          if (countNoInterTC == 0) {

            minNoInterTC = ratio;
            maxNoInterTC = ratio;

          } else {

            if (minNoInterTC > ratio)
              minNoInterTC = ratio;
            if (maxNoInterTC < ratio)
              maxNoInterTC = ratio;

          }
          sumNoInterTC += ratio;
          ++countNoInterTC;

        } else if (paramP.type == FrameTetrahedron && 
                   paramQ.type == FrameTetrahedron) {

          if (countNoInterTT == 0) {

            minNoInterTT = ratio;
            maxNoInterTT = ratio;

          } else {

            if (minNoInterTT > ratio)
              minNoInterTT = ratio;
            if (maxNoInterTT < ratio)
              maxNoInterTT = ratio;

          }
          sumNoInterTT += ratio;
          ++countNoInterTT;

        }
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

void Qualify2DStatic(void) {

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

    minInterCC = 0.0;
    maxInterCC = 0.0;
    sumInterCC = 0.0;
    countInterCC = 0;
    minNoInterCC = 0.0;
    maxNoInterCC = 0.0;
    sumNoInterCC = 0.0;
    countNoInterCC = 0;

    minInterCT = 0.0;
    maxInterCT = 0.0;
    sumInterCT = 0.0;
    countInterCT = 0;
    minNoInterCT = 0.0;
    maxNoInterCT = 0.0;
    sumNoInterCT = 0.0;
    countNoInterCT = 0;

    minInterTC = 0.0;
    maxInterTC = 0.0;
    sumInterTC = 0.0;
    countInterTC = 0;
    minNoInterTC = 0.0;
    maxNoInterTC = 0.0;
    sumNoInterTC = 0.0;
    countNoInterTC = 0;

    minInterTT = 0.0;
    maxInterTT = 0.0;
    sumInterTT = 0.0;
    countInterTT = 0;
    minNoInterTT = 0.0;
    maxNoInterTT = 0.0;
    sumNoInterTT = 0.0;
    countNoInterTT = 0;

    // Declare two variables to memozie the arguments to the
    // Qualification function
    Param2D paramP;
    Param2D paramQ;

    // Loop on the number of tests
    for (unsigned long iTest = NB_TESTS;
         iTest--;) {

      // Create two random Frame definitions
      Param2D* param = &paramP;
      for (int iParam = 2;
           iParam--;) {

        // 50% chance of being a Cuboid or a Tetrahedron
        if (rnd() < 0.5)
          param->type = FrameCuboid;
        else
          param->type = FrameTetrahedron;

        for (int iAxis = 2;
             iAxis--;) {

          param->orig[iAxis] = -RANGE_AXIS + 2.0 * rnd() * RANGE_AXIS;

          for (int iComp = 2;
               iComp--;) {

            param->comp[iComp][iAxis] = 
              -RANGE_AXIS + 2.0 * rnd() * RANGE_AXIS;

          }

        }
        
        param = &paramQ;

      }

      // Calculate the determinant of the Frames' components matrix

      double detP = 
        paramP.comp[0][0] * paramP.comp[1][1] - 
        paramP.comp[1][0] * paramP.comp[0][1];

      double detQ = 
        paramQ.comp[0][0] * paramQ.comp[1][1] - 
        paramQ.comp[1][0] * paramQ.comp[0][1];

      // If the determinants are not null, ie the Frame are not degenerate
      if (fabs(detP) > EPSILON && fabs(detQ) > EPSILON) {

        // Run the validation on the two Frames
        Qualification2DStatic(
          paramP,
          paramQ);

      }

    }

    // Display the results
    if (iRun == 0) {

      printf("ratio (timeFMB / timeSAT)\n");
      printf("run\t");
      printf("countInter\tcountNoInter\t");
      printf("minInter\tavgInter\tmaxInter\t");
      printf("minNoInter\tavgNoInter\tmaxNoInter\t");
      printf("minTotal\tavgTotal\tmaxTotal\t");

      printf("countInterCC\tcountNoInterCC\t");
      printf("minInterCC\tavgInterCC\tmaxInterCC\t");
      printf("minNoInterCC\tavgNoInterCC\tmaxNoInterCC\t");
      printf("minTotalCC\tavgTotalCC\tmaxTotalCC\t");

      printf("countInterCT\tcountNoInterCT\t");
      printf("minInterCT\tavgInterCT\tmaxInterCT\t");
      printf("minNoInterCT\tavgNoInterCT\tmaxNoInterCT\t");
      printf("minTotalCT\tavgTotalCT\tmaxTotalCT\t");

      printf("countInterTC\tcountNoInterTC\t");
      printf("minInterTC\tavgInterTC\tmaxInterTC\t");
      printf("minNoInterTC\tavgNoInterTC\tmaxNoInterTC\t");
      printf("minTotalTC\tavgTotalTC\tmaxTotalTC\t");

      printf("countInterTT\tcountNoInterTT\t");
      printf("minInterTT\tavgInterTT\tmaxInterTT\t");
      printf("minNoInterTT\tavgNoInterTT\tmaxNoInterTT\t");
      printf("minTotalTT\tavgTotalTT\tmaxTotalTT\n");

    }

    printf("%d\t", iRun);

    printf("%lu\t%lu\t", countInter, countNoInter);
    double avgInter = sumInter / (double)countInter;
    printf("%f\t%f\t%f\t", minInter, avgInter, maxInter);
    double avgNoInter = sumNoInter / (double)countNoInter;
    printf("%f\t%f\t%f\t", minNoInter, avgNoInter, maxNoInter);
    double avg = 
      (sumInter + sumNoInter) / (double)(countInter + countNoInter);
    printf("%f\t%f\t%f\t", 
      (minNoInter < minInter ? minNoInter : minInter), 
      avg,
      (maxNoInter > maxInter ? maxNoInter : maxInter));

    printf("%lu\t%lu\t", countInterCC, countNoInterCC);
    double avgInterCC = sumInterCC / (double)countInterCC;
    printf("%f\t%f\t%f\t", minInterCC, avgInterCC, maxInterCC);
    double avgNoInterCC = sumNoInterCC / (double)countNoInterCC;
    printf("%f\t%f\t%f\t", minNoInterCC, avgNoInterCC, maxNoInterCC);
    double avgCC = 
      (sumInterCC + sumNoInterCC) / (double)(countInterCC + countNoInterCC);
    printf("%f\t%f\t%f\t", 
      (minNoInterCC < minInterCC ? minNoInterCC : minInterCC), 
      avgCC,
      (maxNoInterCC > maxInterCC ? maxNoInterCC : maxInterCC));

    printf("%lu\t%lu\t", countInterCT, countNoInterCT);
    double avgInterCT = sumInterCT / (double)countInterCT;
    printf("%f\t%f\t%f\t", minInterCT, avgInterCT, maxInterCT);
    double avgNoInterCT = sumNoInterCT / (double)countNoInterCT;
    printf("%f\t%f\t%f\t", minNoInterCT, avgNoInterCT, maxNoInterCT);
    double avgCT = 
      (sumInterCT + sumNoInterCT) / (double)(countInterCT + countNoInterCT);
    printf("%f\t%f\t%f\t", 
      (minNoInterCT < minInterCT ? minNoInterCT : minInterCT), 
      avgCT,
      (maxNoInterCT > maxInterCT ? maxNoInterCT : maxInterCT));

    printf("%lu\t%lu\t", countInterTC, countNoInterTC);
    double avgInterTC = sumInterTC / (double)countInterTC;
    printf("%f\t%f\t%f\t", minInterTC, avgInterTC, maxInterTC);
    double avgNoInterTC = sumNoInterTC / (double)countNoInterTC;
    printf("%f\t%f\t%f\t", minNoInterTC, avgNoInterTC, maxNoInterTC);
    double avgTC = 
      (sumInterTC + sumNoInterTC) / (double)(countInterTC + countNoInterTC);
    printf("%f\t%f\t%f\t", 
      (minNoInterTC < minInterTC ? minNoInterTC : minInterTC), 
      avgTC,
      (maxNoInterTC > maxInterTC ? maxNoInterTC : maxInterTC));

    printf("%lu\t%lu\t", countInterTT, countNoInterTT);
    double avgInterTT = sumInterTT / (double)countInterTT;
    printf("%f\t%f\t%f\t", minInterTT, avgInterTT, maxInterTT);
    double avgNoInterTT = sumNoInterTT / (double)countNoInterTT;
    printf("%f\t%f\t%f\t", minNoInterTT, avgNoInterTT, maxNoInterTT);
    double avgTT = 
      (sumInterTT + sumNoInterTT) / (double)(countInterTT + countNoInterTT);
    printf("%f\t%f\t%f\n", 
      (minNoInterTT < minInterTT ? minNoInterTT : minInterTT), 
      avgTT,
      (maxNoInterTT > maxInterTT ? maxNoInterTT : maxInterTT));

  }

}

void Qualify3DStatic(void) {

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

    minInterCC = 0.0;
    maxInterCC = 0.0;
    sumInterCC = 0.0;
    countInterCC = 0;
    minNoInterCC = 0.0;
    maxNoInterCC = 0.0;
    sumNoInterCC = 0.0;
    countNoInterCC = 0;

    minInterCT = 0.0;
    maxInterCT = 0.0;
    sumInterCT = 0.0;
    countInterCT = 0;
    minNoInterCT = 0.0;
    maxNoInterCT = 0.0;
    sumNoInterCT = 0.0;
    countNoInterCT = 0;

    minInterTC = 0.0;
    maxInterTC = 0.0;
    sumInterTC = 0.0;
    countInterTC = 0;
    minNoInterTC = 0.0;
    maxNoInterTC = 0.0;
    sumNoInterTC = 0.0;
    countNoInterTC = 0;

    minInterTT = 0.0;
    maxInterTT = 0.0;
    sumInterTT = 0.0;
    countInterTT = 0;
    minNoInterTT = 0.0;
    maxNoInterTT = 0.0;
    sumNoInterTT = 0.0;
    countNoInterTT = 0;

    // Declare two variables to memozie the arguments to the
    // Qualification function
    Param3D paramP;
    Param3D paramQ;

    // Loop on the number of tests
    for (unsigned long iTest = NB_TESTS;
         iTest--;) {

      // Create two random Frame definitions
      Param3D* param = &paramP;
      for (int iParam = 2;
           iParam--;) {

        // 50% chance of being a Cuboid or a Tetrahedron
        if (rnd() < 0.5)
          param->type = FrameCuboid;
        else
          param->type = FrameTetrahedron;

        for (int iAxis = 3;
             iAxis--;) {

          param->orig[iAxis] = -RANGE_AXIS + 2.0 * rnd() * RANGE_AXIS;

          for (int iComp = 3;
               iComp--;) {

            param->comp[iComp][iAxis] = 
              -RANGE_AXIS + 2.0 * rnd() * RANGE_AXIS;

          }

        }
        
        param = &paramQ;

      }

      // Calculate the determinant of the Frames' components matrix
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

      // If the determinants are not null, ie the Frame are not degenerate
      if (fabs(detP) > EPSILON && fabs(detQ) > EPSILON) {

        // Run the validation on the two Frames
        Qualification3DStatic(
          paramP,
          paramQ);

      }

    }

    // Display the results
    if (iRun == 0) {

      printf("ratio (timeFMB / timeSAT)\n");
      printf("run\t");
      printf("countInter\tcountNoInter\t");
      printf("minInter\tavgInter\tmaxInter\t");
      printf("minNoInter\tavgNoInter\tmaxNoInter\t");
      printf("minTotal\tavgTotal\tmaxTotal\t");

      printf("countInterCC\tcountNoInterCC\t");
      printf("minInterCC\tavgInterCC\tmaxInterCC\t");
      printf("minNoInterCC\tavgNoInterCC\tmaxNoInterCC\t");
      printf("minTotalCC\tavgTotalCC\tmaxTotalCC\t");

      printf("countInterCT\tcountNoInterCT\t");
      printf("minInterCT\tavgInterCT\tmaxInterCT\t");
      printf("minNoInterCT\tavgNoInterCT\tmaxNoInterCT\t");
      printf("minTotalCT\tavgTotalCT\tmaxTotalCT\t");

      printf("countInterTC\tcountNoInterTC\t");
      printf("minInterTC\tavgInterTC\tmaxInterTC\t");
      printf("minNoInterTC\tavgNoInterTC\tmaxNoInterTC\t");
      printf("minTotalTC\tavgTotalTC\tmaxTotalTC\t");

      printf("countInterTT\tcountNoInterTT\t");
      printf("minInterTT\tavgInterTT\tmaxInterTT\t");
      printf("minNoInterTT\tavgNoInterTT\tmaxNoInterTT\t");
      printf("minTotalTT\tavgTotalTT\tmaxTotalTT\n");

    }

    printf("%d\t", iRun);

    printf("%lu\t%lu\t", countInter, countNoInter);
    double avgInter = sumInter / (double)countInter;
    printf("%f\t%f\t%f\t", minInter, avgInter, maxInter);
    double avgNoInter = sumNoInter / (double)countNoInter;
    printf("%f\t%f\t%f\t", minNoInter, avgNoInter, maxNoInter);
    double avg = 
      (sumInter + sumNoInter) / (double)(countInter + countNoInter);
    printf("%f\t%f\t%f\t", 
      (minNoInter < minInter ? minNoInter : minInter), 
      avg,
      (maxNoInter > maxInter ? maxNoInter : maxInter));

    printf("%lu\t%lu\t", countInterCC, countNoInterCC);
    double avgInterCC = sumInterCC / (double)countInterCC;
    printf("%f\t%f\t%f\t", minInterCC, avgInterCC, maxInterCC);
    double avgNoInterCC = sumNoInterCC / (double)countNoInterCC;
    printf("%f\t%f\t%f\t", minNoInterCC, avgNoInterCC, maxNoInterCC);
    double avgCC = 
      (sumInterCC + sumNoInterCC) / (double)(countInterCC + countNoInterCC);
    printf("%f\t%f\t%f\t", 
      (minNoInterCC < minInterCC ? minNoInterCC : minInterCC), 
      avgCC,
      (maxNoInterCC > maxInterCC ? maxNoInterCC : maxInterCC));

    printf("%lu\t%lu\t", countInterCT, countNoInterCT);
    double avgInterCT = sumInterCT / (double)countInterCT;
    printf("%f\t%f\t%f\t", minInterCT, avgInterCT, maxInterCT);
    double avgNoInterCT = sumNoInterCT / (double)countNoInterCT;
    printf("%f\t%f\t%f\t", minNoInterCT, avgNoInterCT, maxNoInterCT);
    double avgCT = 
      (sumInterCT + sumNoInterCT) / (double)(countInterCT + countNoInterCT);
    printf("%f\t%f\t%f\t", 
      (minNoInterCT < minInterCT ? minNoInterCT : minInterCT), 
      avgCT,
      (maxNoInterCT > maxInterCT ? maxNoInterCT : maxInterCT));

    printf("%lu\t%lu\t", countInterTC, countNoInterTC);
    double avgInterTC = sumInterTC / (double)countInterTC;
    printf("%f\t%f\t%f\t", minInterTC, avgInterTC, maxInterTC);
    double avgNoInterTC = sumNoInterTC / (double)countNoInterTC;
    printf("%f\t%f\t%f\t", minNoInterTC, avgNoInterTC, maxNoInterTC);
    double avgTC = 
      (sumInterTC + sumNoInterTC) / (double)(countInterTC + countNoInterTC);
    printf("%f\t%f\t%f\t", 
      (minNoInterTC < minInterTC ? minNoInterTC : minInterTC), 
      avgTC,
      (maxNoInterTC > maxInterTC ? maxNoInterTC : maxInterTC));

    printf("%lu\t%lu\t", countInterTT, countNoInterTT);
    double avgInterTT = sumInterTT / (double)countInterTT;
    printf("%f\t%f\t%f\t", minInterTT, avgInterTT, maxInterTT);
    double avgNoInterTT = sumNoInterTT / (double)countNoInterTT;
    printf("%f\t%f\t%f\t", minNoInterTT, avgNoInterTT, maxNoInterTT);
    double avgTT = 
      (sumInterTT + sumNoInterTT) / (double)(countInterTT + countNoInterTT);
    printf("%f\t%f\t%f\n", 
      (minNoInterTT < minInterTT ? minNoInterTT : minInterTT), 
      avgTT,
      (maxNoInterTT > maxInterTT ? maxNoInterTT : maxInterTT));

  }

}

int main(int argc, char** argv) {

  printf("===== 2D static ======\n");
  Qualify2DStatic();
  printf("\n");
  printf("===== 3D static ======\n");
  Qualify3DStatic();

  return 0;
}
