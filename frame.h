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
  FrameCuboid,
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
  AABB bdgBox;
  // Inverted components used during computation
  double invComp[FRAME_NB_DIM][FRAME_NB_DIM];
} Frame;

// ------------- Functions declaration -------------

// Print the AABB 'that' on stdout
// Output format is (min[0], min[1], min[2])-(max[0], max[1], max[2])
void AABBPrint(const AABB* const that);

// Print the Frame 'that' on stdout
// Output format is (orig[0], orig[1], orig[2])
// (comp[0][0], comp[0][1], comp[0][2])
// (comp[1][0], comp[1][1], comp[1][2])
// (comp[2][0], comp[2][1], comp[2][2])
void FramePrint(const Frame* const that);

// Create a static Frame structure of FrameType 'type',
// at position 'orig' with components 'comp' ([iComp][iAxis])
Frame FrameCreateStatic(
  const FrameType type,
     const double orig[FRAME_NB_DIM],
     const double comp[FRAME_NB_DIM][FRAME_NB_DIM]);

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

// Power function for integer base and exponent
// Return 'base' ^ 'exp'
int powi(
           int base,
  unsigned int exp);

#endif
