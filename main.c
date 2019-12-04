// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Include FMB algorithm library
#include "fmb2d.h"
#include "fmb3d.h"
#include "fmb2dt.h"
#include "fmb3dt.h"

// Main function
int main(int argc, char** argv) {

  // ----------- 3D -------------

  // Create the two objects to be tested for intersection
  double origP3D[3] = {0.0, 0.0, 0.0};
  double compP3D[3][3] = {
    {1.0, 0.0, 0.0},  // First component
    {0.0, 1.0, 0.0},  // Second component
    {0.0, 0.0, 1.0}}; // Third component
  Frame3D P3D = 
    Frame3DCreateStatic(
      FrameTetrahedron,
      origP3D,
      compP3D);
 
  double origQ3D[3] = {0.5, 0.5, 0.5};
  double compQ3D[3][3] = {
    {2.0, 0.0, 0.0},
    {0.0, 2.0, 0.0},
    {0.0, 0.0, 2.0}};
  Frame3D Q3D = 
    Frame3DCreateStatic(
      FrameTetrahedron,
      origQ3D,
      compQ3D);

  // Declare a variable to memorize the result of the intersection
  // detection
  AABB3D bdgBox3DLocal;

  // Test for intersection between P and Q
  bool isIntersecting3D = 
    FMBTestIntersection3D(
      &P3D, 
      &Q3D, 
      &bdgBox3DLocal);

  // If the two objects are intersecting
  if (isIntersecting3D) {

    printf("Intersection detected in AABB ");

    // Export the local bounding box toward the real coordinates
    // system
    AABB3D bdgBox3D;
    Frame3DExportBdgBox(
      &Q3D, 
      &bdgBox3DLocal, 
      &bdgBox3D);

    // Clip with the AABB of 'Q3D' and 'P3D' to improve results
    for (int iAxis = 2; 
         iAxis--;) {

      if (bdgBox3D.min[iAxis] < P3D.bdgBox.min[iAxis]) {

        bdgBox3D.min[iAxis] = P3D.bdgBox.min[iAxis];

      }
      if (bdgBox3D.max[iAxis] > P3D.bdgBox.max[iAxis]) {

        bdgBox3D.max[iAxis] = P3D.bdgBox.max[iAxis];

      }

      if (bdgBox3D.min[iAxis] < Q3D.bdgBox.min[iAxis]) {

        bdgBox3D.min[iAxis] = Q3D.bdgBox.min[iAxis];

      }
      if (bdgBox3D.max[iAxis] > Q3D.bdgBox.max[iAxis]) {

        bdgBox3D.max[iAxis] = Q3D.bdgBox.max[iAxis];

      }

    }

    AABB3DPrint(&bdgBox3D);
    printf("\n");

  // Else, the two objects are not intersecting
  } else {

    printf("No intersection.\n");

  }


  // ----------- 2D -------------

  // Create the two objects to be tested for intersection
  double origP2D[2] = {0.0, 0.0};
  double compP2D[2][2] = {
    {1.0, 0.0},  // First component
    {0.0, 1.0}}; // Second component
  Frame2D P2D = 
    Frame2DCreateStatic(
      FrameCuboid,
      origP2D,
      compP2D);
 
  double origQ2D[2] = {0.0,0.0};
  double compQ2D[2][2] = {
    {1.0, 1.0},
    {-1.0, 1.0}};
  Frame2D Q2D = 
    Frame2DCreateStatic(
      FrameCuboid,
      origQ2D,
      compQ2D);

  // Declare a variable to memorize the result of the intersection
  // detection
  AABB2D bdgBox2DLocal;

  // Test for intersection between P and Q
  bool isIntersecting2D = 
    FMBTestIntersection2D(
      &P2D, 
      &Q2D, 
      &bdgBox2DLocal);

  // If the two objects are intersecting
  if (isIntersecting2D) {

    printf("Intersection detected in AABB ");

    // Export the local bounding box toward the real coordinates
    // system
    AABB2D bdgBox2D;
    Frame2DExportBdgBox(
      &Q2D, 
      &bdgBox2DLocal, 
      &bdgBox2D);

    // Clip with the AABB of 'Q2D' and 'P2D' to improve results
    for (int iAxis = 2; 
         iAxis--;) {

      if (bdgBox2D.min[iAxis] < P2D.bdgBox.min[iAxis]) {

        bdgBox2D.min[iAxis] = P2D.bdgBox.min[iAxis];

      }
      if (bdgBox2D.max[iAxis] > P2D.bdgBox.max[iAxis]) {

        bdgBox2D.max[iAxis] = P2D.bdgBox.max[iAxis];

      }

      if (bdgBox2D.min[iAxis] < Q2D.bdgBox.min[iAxis]) {

        bdgBox2D.min[iAxis] = Q2D.bdgBox.min[iAxis];

      }
      if (bdgBox2D.max[iAxis] > Q2D.bdgBox.max[iAxis]) {

        bdgBox2D.max[iAxis] = Q2D.bdgBox.max[iAxis];

      }

    }

    AABB2DPrint(&bdgBox2D);
    printf("\n");

  // Else, the two objects are not intersecting
  } else {

    printf("No intersection.\n");

  }

  // ----------- 2DTime -------------

  // Create the two objects to be tested for intersection
  double origP2DTime[2] = {0.0, 0.0};
  double speedP2DTime[2] = {0.0, 0.0};
  double compP2DTime[2][2] = {
    {1.0, 0.0},  // First component
    {0.0, 1.0}}; // Second component
  Frame2DTime P2DTime = 
    Frame2DTimeCreateStatic(
      FrameCuboid,
      origP2DTime,
      speedP2DTime,
      compP2DTime);
 
  double origQ2DTime[2] = {-1.0,0.0};
  double speedQ2DTime[2] = {1.0,0.0};
  double compQ2DTime[2][2] = {
    {1.0, 0.0},
    {0.0, 1.0}};
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
    for (int iAxis = 2; 
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
