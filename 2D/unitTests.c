/*
    FMB algorithm implementation to perform intersection detection of pairs of static/dynamic cuboid/tetrahedron in 2D/3D by using the Fourier-Motzkin elimination method
    Copyright (C) 2020  Pascal Baillehache bayashipascal@gmail.com
    https://github.com/BayashiPascal/FMB

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Include the FMB algorithm library
#include "fmb2d.h"

// Epsilon for numerical precision
#define EPSILON 0.0001

// Helper structure to pass arguments to the UnitTest function
typedef struct {

  FrameType type;
  double orig[2];
  double comp[2][2];

} Param2D;

// Unit test function
// Takes two Frame definitions, the correct answer in term of
// intersection/no intersection and the correct bounding box
// Run the FMB intersection detection algorihtm on the Frames
// and check against the correct results
void UnitTest2D(
  const Param2D paramP,
  const Param2D paramQ,
  const bool correctAnswer,
  const AABB2D* const correctBdgBox) {

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

  // Declare a variable to memorize the resulting bounding box
  AABB2D bdgBoxLocal;

  // Helper variables to loop on the pair (that, tho) and (tho, that)
  Frame2D* that = &P;
  Frame2D* tho = &Q;

  // Loop on pairs of Frames
  for (
    int iPair = 2;
    iPair--;) {

    // Display the tested frames
    Frame2DPrint(that);
    printf("\nagainst\n");
    Frame2DPrint(tho);
    printf("\n");

    // Run the FMB intersection test
    bool isIntersecting =
      FMBTestIntersection2D(
        that,
        tho,
        &bdgBoxLocal);

    // If the test hasn't given the expected answer about intersection
    if (isIntersecting != correctAnswer) {

      // Display information about the failure
      printf(" Failed\n");
      printf("Expected : ");
      if (correctAnswer == false) printf("no ");
      printf("intersection\n");
      printf("Got : ");
      if (isIntersecting == false) printf("no ");
      printf("intersection\n");
      exit(0);

    // Else, the test has given the expected answer about intersection
    } else {

      // If the Frames were intersecting
      if (isIntersecting == true) {

        AABB2D bdgBox;
        Frame2DExportBdgBox(
          tho,
          &bdgBoxLocal,
          &bdgBox);

        for (
          int iAxis = 2;
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
        for (
          int i = 2;
          i--;) {

          if (
            bdgBox.min[i] > correctBdgBox->min[i] + EPSILON ||
            bdgBox.max[i] < correctBdgBox->max[i] - EPSILON) {

            flag = false;

          }

        }

        // If the bounding box is the expected one
        if (flag == true) {

          // Display information
          printf("Succeed\n");
          AABB2DPrint(&bdgBox);
          printf("\n");

        // Else, the bounding box wasn't the expected one
        } else {

          // Display information
          printf("Failed\n");
          printf("Expected : ");
          AABB2DPrint(correctBdgBox);
          printf("\n");
          printf("     Got : ");
          AABB2DPrint(&bdgBox);

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

void Test2D(void) {

  // Declare two variables to memorize the arguments to the
  // Validation function
  Param2D paramP;
  Param2D paramQ;

  // Declare a variable to memorize the correct bounding box
  AABB2D correctBdgBox;

  // Execute the unit test on various cases
  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.0, 0.0},
    .max = {1.0, 1.0}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.5, 0.5},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.5, 0.5},
    .max = {1.0, 1.0}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {-0.5, -0.5},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.5, 0.5},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  UnitTest2D(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.25, -0.25},
    .comp = {{0.5, 0.0}, {0.0, 2.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.25, 0.0},
    .max = {0.75, 1.0}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameCuboid,
    .orig = {-0.25, 0.25},
    .comp = {{2.0, 0.0}, {0.0, 0.5}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.0, 0.25},
    .max = {1.0, 0.75}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 1.0}, {-1.0, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.0, 0.0},
    .max = {1.0, 1.0}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {-0.5, -0.5},
    .comp = {{1.0, 1.0}, {-1.0, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.0, 0.0},
    .max = {0.5, 1.0}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {1.5, 1.5},
    .comp = {{1.0, -1.0}, {-1.0, -1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameCuboid,
    .orig = {1.0, 0.0},
    .comp = {{-1.0, 0.0}, {0.0, 1.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.5, 0.0},
    .max = {1.0, 1.0}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {1.0, 0.5},
    .comp = {{-0.5, 0.5}, {-0.5, -0.5}}

  };
  paramQ = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 1.0},
    .comp = {{1.0, 0.0}, {0.0, -1.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.0, 0.0},
    .max = {1.0, 1.0}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.0}, {1.0, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameCuboid,
    .orig = {2.0, -1.0},
    .comp = {{0.0, 1.0}, {-0.5, 1.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {1.5, 0.5},
    .max = {1.5 + 0.5 / 3.0, 1.0}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.5}, {0.5, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameCuboid,
    .orig = {1.0, 1.0},
    .comp = {{-0.5, -0.5}, {0.0, -1.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.5, 0.25},
    .max = {1.0, 1.0}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.5}, {0.5, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameCuboid,
    .orig = {1.0, 2.0},
    .comp = {{-0.5, -0.5}, {0.0, -1.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.5, 0.75},
    .max = {1.0, 1.25}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameTetrahedron,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.5}, {0.5, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameCuboid,
    .orig = {1.0, 2.0},
    .comp = {{-0.5, -0.5}, {0.0, -1.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.5, 0.5},
    .max = {0.75, 1.0}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.5}, {0.5, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameTetrahedron,
    .orig = {1.0, 2.0},
    .comp = {{-0.5, -0.5}, {0.0, -1.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.5 + 1.0 / 3.0, 1.0},
    .max = {1.0, 1.0 + 1.0 / 3.0}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameTetrahedron,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.0, 0.0},
    .max = {1.0, 1.0}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameTetrahedron,
    .orig = {0.0, -0.5},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.0, 0.0},
    .max = {0.5, 0.5}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.5, 0.5},
    .comp = {{-0.5, 0.0}, {0.0, -0.5}}

  };
  paramQ = (Param2D) {

    .type = FrameTetrahedron,
    .orig = {0.0, -0.5},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.0, 0.0},
    .max = {0.5, 0.5}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.5, 0.5},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameTetrahedron,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  UnitTest2D(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameCuboid,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameTetrahedron,
    .orig = {1.5, 1.5},
    .comp = {{-1.5, 0.0}, {0.0, -1.5}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.5, 0.5},
    .max = {1.0, 1.0}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameTetrahedron,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.0}, {0.0, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameTetrahedron,
    .orig = {1.01, 1.01},
    .comp = {{-1.0, 0.0}, {0.0, -1.0}}

  };
  UnitTest2D(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameTetrahedron,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.5}, {0.5, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameTetrahedron,
    .orig = {1.0, 1.0},
    .comp = {{-0.5, -0.5}, {0.0, -1.0}}

  };
  correctBdgBox = (AABB2D) {

    .min = {0.5, 0.5 - 1.0 / 6.0},
    .max = {1.0, 0.75}

  };
  UnitTest2D(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param2D) {

    .type = FrameTetrahedron,
    .orig = {0.0, 0.0},
    .comp = {{1.0, 0.5}, {0.5, 1.0}}

  };
  paramQ = (Param2D) {

    .type = FrameTetrahedron,
    .orig = {1.01, 1.5},
    .comp = {{-0.5, -0.5}, {0.0, -1.0}}

  };
  UnitTest2D(
    paramP,
    paramQ,
    false,
    NULL);

  // If we reached here, it means all the unit tests succeed
  printf("All unit tests 2D have succeed.\n");

}

// Main function
int main(int argc, char** argv) {

  Test2D();

  return 0;

}
