// Include standard libraries
#include <stdlib.h>
#include <stdio.h>

// Include FMB algorithm library
#include "fmb.h"

// Main function
int main(int argc, char** argv) {

  // Declare variables to memorize objects to be tested for collision
  // and the result of the collision detection algorithm
  Frame P;
  Frame Q;
  AABB bdgBoxIntersec;

  // Create the two objects to be tested for collision
  P.type = FrameParallelepiped;
  P.orig[0]= 0.0; P.orig[1]= 0.0; P.orig[2]= 0.0;
  P.comp[0][0]= 1.0; P.comp[0][1]= 0.0; P.comp[0][2]= 0.0;
  P.comp[1][0]= 0.0; P.comp[1][1]= 1.0; P.comp[1][2]= 0.0;
  P.comp[2][0]= 0.0; P.comp[2][1]= 0.0; P.comp[2][2]= 1.0;
  FrameUpdateBdgBox(P);
 
  Q.type = FrameTetrahedron;
  Q.orig[0]= 0.0; Q.orig[1]= 0.0; Q.orig[2]= 0.0;
  Q.comp[0][0]= 1.0; Q.comp[0][1]= 0.0; Q.comp[0][2]= 0.0;
  Q.comp[1][0]= 0.0; Q.comp[1][1]= 1.0; Q.comp[1][2]= 0.0;
  Q.comp[2][0]= 0.0; Q.comp[2][1]= 0.0; Q.comp[2][2]= 1.0;
  FrameUpdateBdgBox(Q);

  // Test for collision between P and Q
  boolean isColliding = TestCollisionBetween(&P, &Q, &bdgBoxIntersec);

  // If the two objects are colliding
  if (isColliding) {

    printf("Collision detected in area ");
    AABBPrint(&bdgBoxIntersec);
    printf("\n");

  // Else, the two objects are not colliding
  } else {

    printf("No collision.")

  }
  
  return 0;
}
