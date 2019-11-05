// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Include FMB algorithm library
#include "fmb.h"

// Main function
int main(int argc, char** argv) {

  // Create the two objects to be tested for intersection
#if FRAME_NB_DIM == 3
  double origP[FRAME_NB_DIM] = {0.0, 0.0, 0.0};
  double compP[FRAME_NB_DIM][FRAME_NB_DIM] = {
    {1.0, 0.0, 0.0},  // First component
    {0.0, 1.0, 0.0},  // Second component
    {0.0, 0.0, 1.0}}; // Third component
#elif FRAME_NB_DIM == 2
  double origP[FRAME_NB_DIM] = {-52.403292,24.255395};
  double compP[FRAME_NB_DIM][FRAME_NB_DIM] = {
    {-80.262977,22.602069},  // First component
    {10.091794,-2.842689}}; // Second component
#else
  printf("Not implemented for dimension %d\n", FRAME_NB_DIM);
  exit(0);
#endif
  Frame P = 
    FrameCreateStatic(
      FrameTetrahedron,
      origP,
      compP);
 
#if FRAME_NB_DIM == 3
  double origQ[FRAME_NB_DIM] = {0.5, 0.5, 0.5};
  double compQ[FRAME_NB_DIM][FRAME_NB_DIM] = {
    {2.0, 0.0, 0.0},
    {0.0, 2.0, 0.0},
    {0.0, 0.0, 2.0}};
#elif FRAME_NB_DIM == 2
  double origQ[FRAME_NB_DIM] = {-61.696434,-13.325351};
  double compQ[FRAME_NB_DIM][FRAME_NB_DIM] = {
    {52.745199,67.810595},
    {-71.784526,-53.887880}};
#else
  printf("Not implemented for dimension %d\n", FRAME_NB_DIM);
  exit(0);
#endif
  Frame Q = 
    FrameCreateStatic(
      FrameTetrahedron,
      origQ,
      compQ);

  // Declare a variable to memorize the result of the intersection
  // detection
  AABB bdgBox;

  // Test for intersection between P and Q
  bool isIntersecting = FMBTestIntersection(&P, &Q, &bdgBox);

  // If the two objects are intersecting
  if (isIntersecting) {

    printf("Intersection detected in AABB ");
    AABBPrint(&bdgBox);
    printf("\n");

  // Else, the two objects are not intersecting
  } else {

    printf("No intersection.\n");

  }
  
  return 0;
}
