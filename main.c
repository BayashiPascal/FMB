// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Include FMB algorithm library
#include "fmb2d.h"
#include "fmb3d.h"

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
  AABB3D bdgBox3D;

  // Test for intersection between P and Q
  bool isIntersecting3D = 
    FMBTestIntersection3D(
      &P3D, 
      &Q3D, 
      &bdgBox3D);

  // If the two objects are intersecting
  if (isIntersecting3D) {

    printf("Intersection detected in AABB ");
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
  AABB2D bdgBox2D;

  // Test for intersection between P and Q
  bool isIntersecting2D = 
    FMBTestIntersection2D(
      &P2D, 
      &Q2D, 
      &bdgBox2D);

  // If the two objects are intersecting
  if (isIntersecting2D) {

    printf("Intersection detected in AABB ");
    AABB2DPrint(&bdgBox2D);
    printf("\n");

  // Else, the two objects are not intersecting
  } else {

    printf("No intersection.\n");

  }




  return 0;
}
