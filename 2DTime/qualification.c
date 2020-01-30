// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

// Include FMB and SAT algorithm library
#include "fmb2dt.h"
#include "sat.h"

// Epsilon to detect degenerated triangles
#define EPSILON 0.1
// Range of values for the random generation of Frames
#define RANGE_AXIS 100.0
// Nb of run
#define NB_RUNS 9
// Nb of tests per run
#define NB_TESTS 500000
// Nb of times the test is run on one pair of frame, used to
// slow down the processus and be able to measure time
#define NB_REPEAT_2D 1500

// Helper macro to generate random number in [0.0, 1.0]
#define rnd() (double)(rand())/(double)(RAND_MAX)

// Helper structure to pass arguments to the Qualification function
typedef struct {
  FrameType type;
  double orig[2];
  double comp[2][2];
  double speed[2];
} Param2DTime;

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
void Qualification2DDynamic(
        const Param2DTime paramP,
        const Param2DTime paramQ) {

  // Create the two Frames
  Frame2DTime P =
    Frame2DTimeCreateStatic(
      paramP.type,
      paramP.orig,
      paramP.speed,
      paramP.comp);
 
  Frame2DTime Q =
    Frame2DTimeCreateStatic(
      paramQ.type,
      paramQ.orig,
      paramQ.speed,
      paramQ.comp);

  // Helper variables to loop on the pair (that, tho) and (tho, that)
  Frame2DTime* that = &P;
  Frame2DTime* tho = &Q;

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
        FMBTestIntersection2DTime(
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
        SATTestIntersection2DTime(
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
        Frame2DTimePrint(that);
        printf(" against ");
        Frame2DTimePrint(tho);
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

void Qualify2DDynamic(void) {

  // Initialise the random generator
  srandom(time(NULL));

  // Open the files to save the results
  FILE* fp = fopen("../Results/qualification2DTime.txt", "w");
  FILE* fpCC = fopen("../Results/qualification2DTimeCC.txt", "w");
  FILE* fpCT = fopen("../Results/qualification2DTimeCT.txt", "w");
  FILE* fpTC = fopen("../Results/qualification2DTimeTC.txt", "w");
  FILE* fpTT = fopen("../Results/qualification2DTimeTT.txt", "w");

  // Loop on runs
  for (int iRun = 0;
       iRun < NB_RUNS;
       ++iRun) {

    // Ratio intersection/no intersection for the displayed results
    double ratioInter = 0.1 + 0.8 * (double)iRun / (double)(NB_RUNS - 1);

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

    // Declare two variables to memorize the arguments to the
    // Qualification function
    Param2DTime paramP;
    Param2DTime paramQ;

    // Loop on the number of tests
    for (unsigned long iTest = NB_TESTS;
         iTest--;) {

      // Create two random Frame definitions
      Param2DTime* param = &paramP;
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
          param->speed[iAxis] = -RANGE_AXIS + 2.0 * rnd() * RANGE_AXIS;

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
        Qualification2DDynamic(
          paramP,
          paramQ);

      }

    }

    // Display the results
    if (iRun == 0) {

      fprintf(fp, "percPairInter,");
      fprintf(fp, "countInterTo,countNoInterTo,");
      fprintf(fp, "minInterTo,avgInterTo,maxInterTo,");
      fprintf(fp, "minNoInterTo,avgNoInterTo,maxNoInterTo,");
      fprintf(fp, "minTotalTo,avgTotalTo,maxTotalTo\n");

      fprintf(fpCC, "percPairInter,");
      fprintf(fpCC, "countInterCC,countNoInterCC,");
      fprintf(fpCC, "minInterCC,avgInterCC,maxInterCC,");
      fprintf(fpCC, "minNoInterCC,avgNoInterCC,maxNoInterCC,");
      fprintf(fpCC, "minTotalCC,avgTotalCC,maxTotalCC\n");

      fprintf(fpCT, "percPairInter,");
      fprintf(fpCT, "countInterCT,countNoInterCT,");
      fprintf(fpCT, "minInterCT,avgInterCT,maxInterCT,");
      fprintf(fpCT, "minNoInterCT,avgNoInterCT,maxNoInterCT,");
      fprintf(fpCT, "minTotalCT,avgTotalCT,maxTotalCT\n");

      fprintf(fpTC, "percPairInter,");
      fprintf(fpTC, "countInterTC,countNoInterTC,");
      fprintf(fpTC, "minInterTC,avgInterTC,maxInterTC,");
      fprintf(fpTC, "minNoInterTC,avgNoInterTC,maxNoInterTC,");
      fprintf(fpTC, "minTotalTC,avgTotalTC,maxTotalTC\n");

      fprintf(fpTT, "percPairInter,");
      fprintf(fpTT, "countInterTT,countNoInterTT,");
      fprintf(fpTT, "minInterTT,avgInterTT,maxInterTT,");
      fprintf(fpTT, "minNoInterTT,avgNoInterTT,maxNoInterTT,");
      fprintf(fpTT, "minTotalTT,avgTotalTT,maxTotalTT\n");

    }

    fprintf(fp, "%.1f,", ratioInter);
    fprintf(fp, "%lu,%lu,", countInter, countNoInter);
    double avgInter = sumInter / (double)countInter;
    fprintf(fp, "%f,%f,%f,", minInter, avgInter, maxInter);
    double avgNoInter = sumNoInter / (double)countNoInter;
    fprintf(fp, "%f,%f,%f,", minNoInter, avgNoInter, maxNoInter);
    double avg =
      ratioInter * avgInter + (1.0 - ratioInter) * avgNoInter;
    fprintf(fp, "%f,%f,%f",
      (minNoInter < minInter ? minNoInter : minInter),
      avg,
      (maxNoInter > maxInter ? maxNoInter : maxInter));
    if (iRun < NB_RUNS - 1) {
      fprintf(fp, "\n");
    }

    fprintf(fpCC, "%.1f,", ratioInter);
    fprintf(fpCC, "%lu,%lu,", countInterCC, countNoInterCC);
    double avgInterCC = sumInterCC / (double)countInterCC;
    fprintf(fpCC, "%f,%f,%f,", minInterCC, avgInterCC, maxInterCC);
    double avgNoInterCC = sumNoInterCC / (double)countNoInterCC;
    fprintf(fpCC, "%f,%f,%f,", minNoInterCC, avgNoInterCC, maxNoInterCC);
    double avgCC =
      ratioInter * avgInterCC + (1.0 - ratioInter) * avgNoInterCC;
    fprintf(fpCC, "%f,%f,%f",
      (minNoInterCC < minInterCC ? minNoInterCC : minInterCC),
      avgCC,
      (maxNoInterCC > maxInterCC ? maxNoInterCC : maxInterCC));
    if (iRun < NB_RUNS - 1) {
      fprintf(fpCC, "\n");
    }

    fprintf(fpCT, "%.1f,", ratioInter);
    fprintf(fpCT, "%lu,%lu,", countInterCT, countNoInterCT);
    double avgInterCT = sumInterCT / (double)countInterCT;
    fprintf(fpCT, "%f,%f,%f,", minInterCT, avgInterCT, maxInterCT);
    double avgNoInterCT = sumNoInterCT / (double)countNoInterCT;
    fprintf(fpCT, "%f,%f,%f,", minNoInterCT, avgNoInterCT, maxNoInterCT);
    double avgCT =
      ratioInter * avgInterCT + (1.0 - ratioInter) * avgNoInterCT;
    fprintf(fpCT, "%f,%f,%f",
      (minNoInterCT < minInterCT ? minNoInterCT : minInterCT),
      avgCT,
      (maxNoInterCT > maxInterCT ? maxNoInterCT : maxInterCT));
    if (iRun < NB_RUNS - 1) {
      fprintf(fpCT, "\n");
    }

    fprintf(fpTC, "%.1f,", ratioInter);
    fprintf(fpTC, "%lu,%lu,", countInterTC, countNoInterTC);
    double avgInterTC = sumInterTC / (double)countInterTC;
    fprintf(fpTC, "%f,%f,%f,", minInterTC, avgInterTC, maxInterTC);
    double avgNoInterTC = sumNoInterTC / (double)countNoInterTC;
    fprintf(fpTC, "%f,%f,%f,", minNoInterTC, avgNoInterTC, maxNoInterTC);
    double avgTC =
      ratioInter * avgInterTC + (1.0 - ratioInter) * avgNoInterTC;
    fprintf(fpTC, "%f,%f,%f",
      (minNoInterTC < minInterTC ? minNoInterTC : minInterTC),
      avgTC,
      (maxNoInterTC > maxInterTC ? maxNoInterTC : maxInterTC));
    if (iRun < NB_RUNS - 1) {
      fprintf(fpTC, "\n");
    }

    fprintf(fpTT, "%.1f,", ratioInter);
    fprintf(fpTT, "%lu,%lu,", countInterTT, countNoInterTT);
    double avgInterTT = sumInterTT / (double)countInterTT;
    fprintf(fpTT, "%f,%f,%f,", minInterTT, avgInterTT, maxInterTT);
    double avgNoInterTT = sumNoInterTT / (double)countNoInterTT;
    fprintf(fpTT, "%f,%f,%f,", minNoInterTT, avgNoInterTT, maxNoInterTT);
    double avgTT =
      ratioInter * avgInterTT + (1.0 - ratioInter) * avgNoInterTT;
    fprintf(fpTT, "%f,%f,%f",
      (minNoInterTT < minInterTT ? minNoInterTT : minInterTT),
      avgTT,
      (maxNoInterTT > maxInterTT ? maxNoInterTT : maxInterTT));
    if (iRun < NB_RUNS - 1) {
      fprintf(fpTT, "\n");
    }

  }

  // Close the files
  fclose(fp);
  fclose(fpCC);
  fclose(fpCT);
  fclose(fpTC);
  fclose(fpTT);
 
}

int main(int argc, char** argv) {

  Qualify2DDynamic();

  return 0;
}
