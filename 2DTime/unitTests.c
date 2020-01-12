// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Include the FMB algorithm library
#include "fmb2dt.h"

// Epslon for numerical precision
#define EPSILON 0.0001

// Helper structure to pass arguments to the UnitTest function
typedef struct {
  FrameType type;
  double orig[2];
  double comp[2][2];
  double speed[2];
} Param2DTime;

// Unit test function
// Takes two Frame definitions, the correct answer in term of
// intersection/no intersection and the correct bounding box
// Run the FMB intersection detection alogirhtm on the Frames
// and check against the correct results
void UnitTest2DTime(
        const Param2DTime paramP,
        const Param2DTime paramQ,
         const bool correctAnswer,
  const AABB2DTime* const correctBdgBox) {

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

  // Declare a variable to memorize the resulting bounding box
  AABB2DTime bdgBoxLocal;

  // Helper variables to loop on the pair (that, tho) and (tho, that)
  Frame2DTime* that = &P;
  Frame2DTime* tho = &Q;

  // Loop on pairs of Frames
  for (int iPair = 2;
       iPair--;) {

    // Display the tested frames
    Frame2DTimePrint(that);
    printf("\nagainst\n");
    Frame2DTimePrint(tho);
    printf("\n");

    // Run the FMB intersection test
    bool isIntersecting = 
      FMBTestIntersection2DTime(
        that, 
        tho, 
        &bdgBoxLocal);

    // If the test hasn't given the expected answer about intersection
    if (isIntersecting != correctAnswer) {

      // Display information about the failure
      printf(" Failed\n");
      printf("Expected : ");
      if (correctAnswer == false)
        printf("no ");
      printf("intersection\n");
      printf("Got : ");
      if (isIntersecting == false)
        printf("no ");
      printf("intersection\n");
      exit(0);

    // Else, the test has given the expected answer about intersection
    } else {

      // If the Frames were intersecting
      if (isIntersecting == true) {

        AABB2DTime bdgBox;
        Frame2DTimeExportBdgBox(
          tho, 
          &bdgBoxLocal, 
          &bdgBox);
        // Check the bounding box
        bool flag = true;
        for (int i = 3;
             i--;) {

          if (bdgBox.min[i] > correctBdgBox->min[i] + EPSILON ||
              bdgBox.max[i] < correctBdgBox->max[i] - EPSILON) {

            flag = false;

          }

        }

        // If the bounding box is the expected one
        if (flag == true) {

          // Display information
          printf("Succeed\n");

        // Else, the bounding box wasn't the expected one
        } else {

          // Display information
          printf("Failed\n");
          printf("Expected : ");
          AABB2DTimePrint(correctBdgBox);
          printf("\n");
          printf("     Got : ");
          AABB2DTimePrint(&bdgBox);
          printf("\n");

          // Terminate the unit tests
          exit(0);

        }

      // Else the Frames were not intersected,
      // no need to check the bounding box
      } else {

        // Display information
        printf(" Succeed\n");

      }

    }
    printf("\n");

    // Flip the pair of Frames
    that = &Q;
    tho = &P;

  }
  
}

void Test2DTime(void) {

  // Declare two variables to memozie the arguments to the
  // Validation function
  Param2DTime paramP;
  Param2DTime paramQ;

  // Declare a variable to memorize the correct bounding box
  AABB2DTime correctBdgBox;

  // Execute the unit test on various cases

  // ----------------------------
  paramP = (Param2DTime)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}},
     .speed = {0.0, 0.0}
    };
  paramQ = (Param2DTime)
    {.type = FrameCuboid,
     .orig = {-1.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}},
     .speed = {-1.0, 0.0}
    };
  UnitTest2DTime(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param2DTime)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}},
     .speed = {0.0, 0.0}
    };
  paramQ = (Param2DTime)
    {.type = FrameCuboid,
     .orig = {-1.01, -1.01},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}},
     .speed = {1.0, 0.0}
    };
  UnitTest2DTime(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param2DTime)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}},
     .speed = {0.0, 0.0}
    };
  paramQ = (Param2DTime)
    {.type = FrameCuboid,
     .orig = {-1.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}},
     .speed = {1.0, 0.0}
    };
  correctBdgBox = (AABB2DTime) 
    {.min = {0.0, 0.0, 0.0},
     .max = {1.0, 1.0, 1.0}
    };
  UnitTest2DTime(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2DTime)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}},
     .speed = {0.0, 0.0}
    };
  paramQ = (Param2DTime)
    {.type = FrameCuboid,
     .orig = {-1.0, 0.25},
     .comp = 
       {{0.5, 0.0},
        {0.0, 0.5}},
     .speed = {4.0, 0.0}
    };
  correctBdgBox = (AABB2DTime) 
    {.min = {0.0, 0.25, 0.125},
     .max = {1.0, 0.75, 0.5}
    };
  UnitTest2DTime(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2DTime)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}},
     .speed = {0.0, 0.0}
    };
  paramQ = (Param2DTime)
    {.type = FrameCuboid,
     .orig = {0.25, -1.0},
     .comp = 
       {{0.5, 0.0},
        {0.0, 0.5}},
     .speed = {0.0, 4.0}
    };
  correctBdgBox = (AABB2DTime) 
    {.min = {0.25, 0.0, 0.125},
     .max = {0.75, 1.0, 0.5}
    };
  UnitTest2DTime(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2DTime)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}},
     .speed = {0.0, 0.0}
    };
  paramQ = (Param2DTime)
    {.type = FrameCuboid,
     .orig = {0.9, -1.0},
     .comp = 
       {{0.5, 0.0},
        {0.0, 0.5}},
     .speed = {0.0, 4.0}
    };
  correctBdgBox = (AABB2DTime) 
    {.min = {0.9, 0.0, 0.125},
     .max = {1.0, 1.0, 0.5}
    };
  UnitTest2DTime(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // If we reached here, it means all the unit tests succeed
  printf("All unit tests 2DTime have succeed.\n");

}

// Main function
int main(int argc, char** argv) {

  Test2DTime();

  return 0;
}
