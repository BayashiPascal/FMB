#ifndef __FRAME_H_
#define __FRAME_H_

// ------------- Includes -------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// ------------- Macros -------------

#define FRAME_NB_DIM 3

// ------------- Enumerations -------------

typedef enum {
  FrameParallelepiped,
  FrameTetrahedron
} FrameType;

// ------------- Data structures -------------

// Axis aligned bounding box structure
typedef struct {

  double min[FRAME_NB_DIM];
  double max[FRAME_NB_DIM];

} AABB;

// Axis unaligned parallelepiped and tetrahedron structure
typedef struct {

  FrameType type;
  double orig[FRAME_NB_DIM];
  double comp[FRAME_NB_DIM][FRAME_NB_DIM];

  // AABB of the frame
  AABB bdgbox;

  // Inverted components used during computation
  double invComp[FRAME_NB_DIM][FRAME_NB_DIM];

} Frame;

// ------------- Functions declaration -------------

// Print the AABB 'that' on stdout
// Output format is (min[0], min[1], min[2])-(max[0], max[1], max[2])
void AABBPrint(const AABB* const that);

// Update the inverse components of the Frame 'that'
void FrameUpdateInv(Frame* const that);

// Project the Frame 'Q' in the Frame 'P' 's coordinates system and 
// memorize the result in the Frame 'Qp'
void FrameImportFrame(
  const Frame* const P, 
  const Frame* const Q, 
        Frame* const Qp);

// Export the AABB 'bdgBox' from 'that' 's coordinates system to
// the real coordinates system and update 'bdgBox' with the resulting
// AABB
void FrameExportBdgBox(
  const Frame* const that,
   const AABB* const bdgBox,
         AABB* const bdgBoxProj);

#endif
