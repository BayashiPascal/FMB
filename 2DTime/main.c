// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Include the FMB algorithm library
#include "fmb2dt.h"

// Main function
int main(int argc, char** argv) {

  // Create the two objects to be tested for intersection
  double origP2DTime[2] = {0.0, 0.0};
  double speedP2DTime[2] = {0.0, 0.0};

  // First component, Second component
  double compP2DTime[2][2] = {{1.0, 0.0}, {0.0, 1.0}};
  Frame2DTime P2DTime =
    Frame2DTimeCreateStatic(
      FrameCuboid,
      origP2DTime,
      speedP2DTime,
      compP2DTime);

  double origQ2DTime[2] = {0.0, 0.0};
  double speedQ2DTime[2] = {0.0, 0.0};
  double compQ2DTime[2][2] = {{1.0, 0.0}, {0.0, 1.0}};
  Frame2DTime Q2DTime =
    Frame2DTimeCreateStatic(
      FrameCuboid,
      origQ2DTime,
      speedQ2DTime,
      compQ2DTime);

  // Declare a variable to memorize the result of the intersection
  // detection
  AABB2DTime bdgBox2DTimeLocal;

  // Test for intersection between P and Q
  bool isIntersecting2DTime =
    FMBTestIntersection2DTime(
      &P2DTime,
      &Q2DTime,
      &bdgBox2DTimeLocal);

  // If the two objects are intersecting
  if (isIntersecting2DTime) {

    printf("Intersection detected in AABB ");

    // Export the local bounding box toward the real coordinates
    // system
    AABB2DTime bdgBox2DTime;
    Frame2DTimeExportBdgBox(
      &Q2DTime,
      &bdgBox2DTimeLocal,
      &bdgBox2DTime);

    // Clip with the AABB of 'Q2DTime' and 'P2DTime' to improve results
    for (int iAxis = 3;
         iAxis--;) {

      if (bdgBox2DTime.min[iAxis] < P2DTime.bdgBox.min[iAxis]) {

        bdgBox2DTime.min[iAxis] = P2DTime.bdgBox.min[iAxis];

      }

      if (bdgBox2DTime.max[iAxis] > P2DTime.bdgBox.max[iAxis]) {

        bdgBox2DTime.max[iAxis] = P2DTime.bdgBox.max[iAxis];

      }

      if (bdgBox2DTime.min[iAxis] < Q2DTime.bdgBox.min[iAxis]) {

        bdgBox2DTime.min[iAxis] = Q2DTime.bdgBox.min[iAxis];

      }

      if (bdgBox2DTime.max[iAxis] > Q2DTime.bdgBox.max[iAxis]) {

        bdgBox2DTime.max[iAxis] = Q2DTime.bdgBox.max[iAxis];

      }

    }

    AABB2DTimePrint(&bdgBox2DTime);
    printf("\n");

  // Else, the two objects are not intersecting
  } else {

    printf("No intersection.\n");

  }

  return 0;

}
