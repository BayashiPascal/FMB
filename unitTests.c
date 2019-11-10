// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Include FMB algorithm library
#include "fmb.h"

// Epslon for numerical precision
#define EPSILON 0.0001

// Helper structure to pass arguments to the UnitTest function
typedef struct {
  FrameType type;
  double orig[FRAME_NB_DIM];
  double comp[FRAME_NB_DIM][FRAME_NB_DIM];
} Param;

// Unit test function
// Takes two Frame definitions, the correct answer in term of
// intersection/no intersection and the correct bounding box
// Run the FMB intersection detection alogirhtm on the Frames
// and check against the correct results
void UnitTest(
        const Param paramP,
        const Param paramQ,
         const bool correctAnswer,
  const AABB* const correctBdgBox) {

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

  // Declare a variable to memorize the resulting bounding box
  AABB bdgBox;

  // Helper variables to loop on the pair (that, tho) and (tho, that)
  Frame* that = &P;
  Frame* tho = &Q;

  // Loop on pairs of Frames
  for (int iPair = 2;
       iPair--;) {

    // Display the tested frames
    FramePrint(that);
    printf("\nagainst\n");
    FramePrint(tho);
    printf("\n");

    // Run the FMB intersection test
    bool isIntersecting = 
      FMBTestIntersection(
        that, 
        tho, 
        &bdgBox);

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

        // Check the bounding box
        bool flag = true;
        for (int i = FRAME_NB_DIM;
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
          AABBPrint(correctBdgBox);
          printf("\n");
          printf("     Got : ");
          AABBPrint(&bdgBox);
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

// Main function
int main(int argc, char** argv) {

  // Declare two variables to memozie the arguments to the
  // Validation function
  Param paramP;
  Param paramQ;

  // Declare a variable to memorize the correct bounding box
  AABB correctBdgBox;

  // Execute the unit test on various cases

#if FRAME_NB_DIM == 3

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0, 0.0},
     .max = {1.0, 1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.5, 0.5, 0.5},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.5, 0.5},
     .max = {1.0, 1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {-0.5, -0.5, -0.5},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0, 0.0},
     .max = {0.5, 0.5, 0.5}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {1.5, 1.5, 1.5},
     .comp = 
       {{-1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, -1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.5, 0.5},
     .max = {1.0, 1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.5, 1.5, -1.5},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, -1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.5, 1.5, -1.5},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.5, -1.0},
     .max = {1.0, 1.0, -0.5}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {-1.0, -1.0, -1.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {-1.0, -1.0, -1.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, -0.5, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, -0.5, 0.0},
     .max = {1.0, 0.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameTetrahedron,
     .orig = {-1.0, -1.0, -1.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, -0.5, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {-1.0, -1.0, -1.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, -0.5, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, -0.5, 0.0},
     .max = {0.75, 0.0, 0.75}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameTetrahedron,
     .orig = {-1.0, -1.0, -1.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, -0.5, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param)
    {.type = FrameTetrahedron,
     .orig = {-0.5, -1.0, -0.5},
     .comp = 
       {{1.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, -0.5, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, -0.5, 0.0},
     .max = {0.5, -0.5 + 1.0 / 3.0, 0.5}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

#elif FRAME_NB_DIM == 2

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.5, 0.5},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.5},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {-0.5, -0.5},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.5, 0.5},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.25, -0.25},
     .comp = 
       {{0.5, 0.0},
        {0.0, 2.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.25, 0.0},
     .max = {0.75, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {-0.25, 0.25},
     .comp = 
       {{2.0, 0.0},
        {0.0, 0.5}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.25},
     .max = {1.0, 0.75}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 1.0},
        {-1.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {-0.5, -0.5},
     .comp = 
       {{1.0, 1.0},
        {-1.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0},
     .max = {0.5, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {1.5, 1.5},
     .comp = 
       {{1.0, -1.0},
        {-1.0, -1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {1.0, 0.0},
     .comp = 
       {{-1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.0},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {1.0, 0.5},
     .comp = 
       {{-0.5, 0.5},
        {-0.5, -0.5}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 1.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, -1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {1.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {2.0, -1.0},
     .comp = 
       {{0.0, 1.0},
        {-0.5, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {1.5, 0.5},
     .max = {1.5 + 0.5 / 3.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.5},
        {0.5, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {1.0, 1.0},
     .comp = 
       {{-0.5, -0.5},
        {0.0, -1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.25},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.5},
        {0.5, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {1.0, 2.0},
     .comp = 
       {{-0.5, -0.5},
        {0.0, -1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.75},
     .max = {1.0, 1.25}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.5},
        {0.5, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {1.0, 2.0},
     .comp = 
       {{-0.5, -0.5},
        {0.0, -1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.5},
     .max = {0.75, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.5},
        {0.5, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {1.0, 2.0},
     .comp = 
       {{-0.5, -0.5},
        {0.0, -1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5 + 1.0 / 3.0, 1.0},
     .max = {1.0, 1.0 + 1.0 / 3.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, -0.5},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0},
     .max = {0.5, 0.5}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.5, 0.5},
     .comp = 
       {{-0.5, 0.0},
        {0.0, -0.5}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, -0.5},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0},
     .max = {0.5, 0.5}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.5, 0.5},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {1.5, 1.5},
     .comp = 
       {{-1.5, 0.0},
        {0.0, -1.5}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.5},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {1.0, 1.0},
     .comp = 
       {{-1.0, 0.0},
        {0.0, -1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.5},
        {0.5, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {1.0, 1.0},
     .comp = 
       {{-0.5, -0.5},
        {0.0, -1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.5 - 1.0 / 6.0},
     .max = {1.0, 0.75}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.5},
        {0.5, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {1.0, 1.5},
     .comp = 
       {{-0.5, -0.5},
        {0.0, -1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

#else

  printf("Not implemented for dimension %d\n", FRAME_NB_DIM);
  exit(0);

#endif

  // If we reached here, it means all the unit tests succeed
  printf("All unit tests have succeed.\n");

  return 0;
}
