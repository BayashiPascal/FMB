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

// Main function
int main(int argc, char** argv) {

  // Create the two objects to be tested for intersection
  double origP2D[2] = {0.0, 0.0};
  double compP2D[2][2] = {

    {1.0, 0.0},  // First component
    {0.0, 1.0}   // Second component

  };
  Frame2D P2D =
    Frame2DCreateStatic(
      FrameCuboid,
      origP2D,
      compP2D);

  double origQ2D[2] = {0.0, 0.0};
  double compQ2D[2][2] = {{1.0, 0.0}, {0.0, 1.0}};
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
    for (
      int iAxis = 2;
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

  return 0;

}
