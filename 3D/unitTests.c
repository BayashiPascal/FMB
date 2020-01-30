// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Include the FMB algorithm library
#include "fmb3d.h"

// Epslon for numerical precision
#define EPSILON 0.0001

// Helper structure to pass arguments to the UnitTest function
typedef struct {

  FrameType type;
  double orig[3];
  double comp[3][3];

} Param3D;

// Unit test function
// Takes two Frame definitions, the correct answer in term of
// intersection/no intersection and the correct bounding box
// Run the FMB intersection detection alogirhtm on the Frames
// and check against the correct results
void UnitTest3D(
        const Param3D paramP,
        const Param3D paramQ,
         const bool correctAnswer,
  const AABB3D* const correctBdgBox) {

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

  // Declare a variable to memorize the resulting bounding box
  AABB3D bdgBoxLocal;

  // Helper variables to loop on the pair (that, tho) and (tho, that)
  Frame3D* that = &P;
  Frame3D* tho = &Q;

  // Loop on pairs of Frames
  for (int iPair = 2;
       iPair--;) {

    // Display the tested frames
    Frame3DPrint(that);
    printf("\nagainst\n");
    Frame3DPrint(tho);
    printf("\n");

    // Run the FMB intersection test
    bool isIntersecting =
      FMBTestIntersection3D(
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

        AABB3D bdgBox;
        Frame3DExportBdgBox(
          tho,
          &bdgBoxLocal,
          &bdgBox);

        for (int iAxis = 3;
             iAxis--;) {

          if (bdgBox.min[iAxis] < that->bdgBox.min[iAxis]) {

            bdgBox.min[iAxis] = that->bdgBox.min[iAxis];

          }

          if (bdgBox.max[iAxis] > that->bdgBox.max[iAxis]) {

            bdgBox.max[iAxis] = that->bdgBox.max[iAxis];

          }

          if (bdgBox.min[iAxis] < tho->bdgBox.min[iAxis]) {

            bdgBox.min[iAxis] = tho->bdgBox.min[iAxis];

          }

          if (bdgBox.max[iAxis] > tho->bdgBox.max[iAxis]) {

            bdgBox.max[iAxis] = tho->bdgBox.max[iAxis];

          }

        }

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
          AABB3DPrint(&bdgBox);
          printf("\n");

        // Else, the bounding box wasn't the expected one
        } else {

          // Display information
          printf("Failed\n");
          printf("Expected : ");
          AABB3DPrint(correctBdgBox);
          printf("\n");
          printf("     Got : ");
          AABB3DPrint(&bdgBox);
          printf("\n");

          // Terminate the unit tests
          exit(0);

        }

      // Else the Frames were not intersected,
      // no need to check the bounding box
      } else {

        // Display information
        printf(" Succeed (no inter)\n");

      }

    }

    printf("\n");

    // Flip the pair of Frames
    that = &Q;
    tho = &P;

  }

}

void Test3D(void) {

  // Declare two variables to memozie the arguments to the
  // Validation function
  Param3D paramP;
  Param3D paramQ;

  // Declare a variable to memorize the correct bounding box
  AABB3D correctBdgBox;

  // Execute the unit test on various cases

  // ----------------------------
  paramP = (Param3D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0, 0.0},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  paramQ = (Param3D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0, 0.0},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  correctBdgBox = (AABB3D) {

    .min = {0.0, 0.0, 0.0},
    .max = {1.0, 1.0, 1.0}

    };
  UnitTest3D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param3D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0, 0.0},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  paramQ = (Param3D) {

    .type = FrameCuboid,
    .orig = {0.5, 0.5, 0.5},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  correctBdgBox = (AABB3D) {

    .min = {0.5, 0.5, 0.5},
    .max = {1.0, 1.0, 1.0}

    };
  UnitTest3D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param3D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0, 0.0},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  paramQ = (Param3D) {

    .type = FrameCuboid,
    .orig = {-0.5, -0.5, -0.5},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  correctBdgBox = (AABB3D) {

    .min = {0.0, 0.0, 0.0},
    .max = {0.5, 0.5, 0.5}

    };
  UnitTest3D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param3D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0, 0.0},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  paramQ = (Param3D) {

    .type = FrameCuboid,
    .orig = {1.5, 1.5, 1.5},
    .comp =
      {{-1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0, -1.0}}

    };
  correctBdgBox = (AABB3D) {

    .min = {0.5, 0.5, 0.5},
    .max = {1.0, 1.0, 1.0}

    };
  UnitTest3D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param3D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0, 0.0},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  paramQ = (Param3D) {

    .type = FrameCuboid,
    .orig = {0.5, 1.5, -1.5},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  UnitTest3D(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param3D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0, 0.0},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, -1.0}}

    };
  paramQ = (Param3D) {

    .type = FrameCuboid,
    .orig = {0.5, 1.5, -1.5},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  correctBdgBox = (AABB3D) {

    .min = {0.5, 0.5, -1.0},
    .max = {1.0, 1.0, -0.5}

    };
  UnitTest3D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param3D) {

    .type = FrameCuboid,
    .orig = {-1.01, -1.01, -1.01},
    .comp =
      {{1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}}

    };
  paramQ = (Param3D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0, 0.0},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  UnitTest3D(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param3D) {

    .type = FrameCuboid,
    .orig = {-1.0, -1.0, -1.0},
    .comp =
      {{1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}}

    };
  paramQ = (Param3D){

    .type = FrameCuboid,
    .orig = {0.0, -0.5, 0.0},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  correctBdgBox = (AABB3D) {

    .min = {0.0, -0.5, 0.0},
    .max = {1.0, 0.0, 1.0}

    };
  UnitTest3D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param3D){

    .type = FrameTetrahedron,
    .orig = {-1.0, -1.0, -1.0},
    .comp =
      {{1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}}

    };
  paramQ = (Param3D){

    .type = FrameCuboid,
    .orig = {0.0, -0.5, 0.0},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  UnitTest3D(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param3D){

    .type = FrameCuboid,
    .orig = {-1.0, -1.0, -1.0},
    .comp =
      {{1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}}

    };
  paramQ = (Param3D){

    .type = FrameTetrahedron,
    .orig = {0.0, -0.5, 0.0},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  correctBdgBox = (AABB3D) {

    .min = {0.0, -0.5, 0.0},
    .max = {0.75, 0.0, 0.75}

    };
  UnitTest3D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param3D) {

    .type = FrameTetrahedron,
    .orig = {-1.0, -1.0, -1.0},
    .comp =
      {{1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}}

    };
  paramQ = (Param3D) {

    .type = FrameTetrahedron,
    .orig = {0.0, -0.5, 0.0},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  UnitTest3D(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param3D){

    .type = FrameTetrahedron,
    .orig = {-0.5, -1.0, -0.5},
    .comp =
      {{1.0, 0.0, 0.0}, {1.0, 1.0, 1.0}, {0.0, 0.0, 1.0}}

    };
  paramQ = (Param3D){

    .type = FrameTetrahedron,
    .orig = {0.0, -0.5, 0.0},
    .comp =
      {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}

    };
  correctBdgBox = (AABB3D) {

    .min = {0.0, -0.5, 0.0},
    .max = {0.5, -1.0 / 3.0, 0.5},

    };
  UnitTest3D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // If we reached here, it means all the unit tests succeed
  printf("All unit tests 3D have succeed.\n");

}

// Main function
int main(int argc, char** argv) {

  Test3D();

  return 0;

}
