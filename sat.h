#ifndef __SAT_H_
#define __SAT_H_

#include <stdbool.h>
#include <string.h>
#include "frame.h"

// ------------- Functions declaration -------------

#if FRAME_NB_DIM == 2
// Test for intersection between 2D Frame 'that' and 2D Frame 'tho'
// Return true if the two Frames are intersecting, else false
bool SATTestIntersection2D(
  const Frame* const that, 
  const Frame* const tho);
#endif

#if FRAME_NB_DIM == 3
// Test for intersection between 3D Frame 'that' and 3D Frame 'tho'
// Return true if the two Frames are intersecting, else false
bool SATTestIntersection3D(
  const Frame* const that, 
  const Frame* const tho);
#endif

#endif
