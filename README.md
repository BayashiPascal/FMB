# The FMB Algorithm

## Introduction

This repository includes the C implementation of the FMB algorithm, which can be used to perform intersection detection of pairs of static/dynamic cuboid/tetrahedron in 2D/3D by using the Fourier-Motzkin elimination method. Results introduced in the documentation show that the FMB algorithm can be on average up to 4.8 times faster than the SAT algorithm.

The implementation is divided into 4 folders, 2D for the 2D static case, 2DTime for the 2D dynamic case, 3D for the 3D static case, and 3DTime for the 3D dynamic case. Each case can handle pairs of cuboid and/or tetrahedron. The cuboid and tetrahedron implementation's code is in the Frame folder. The SAT algorithm implementation's code, used for performance comparison, is in the SAT folder.

Each of the 4 cases' folder include the header and body of the FMB algorithm implementation for that case, a minimal example of how to use it, and the code for the validation and qualification of the algorithm.

## Article

The article about this work is available as a pdflatex generated PDF file in the Article folder.

## Example of use

Below are examples of how to use the FMB algorithm in the 3D static and dynamic case.

### 3D static case

```
// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Include the FMB algorithm library
#include "fmb3d.h"

// Main function
int main(int argc, char** argv) {

  // Create the two Frames to be tested for intersection
  double origP3D[3] = {0.0, 0.0, 0.0};
  double compP3D[3][3] = {

    {1.0, 0.0, 0.0},  // First component
    {0.0, 1.0, 0.0},  // Second component
    {0.0, 0.0, 1.0}   // Third component

  };
  Frame3D P3D =
    Frame3DCreateStatic(
      FrameTetrahedron,
      origP3D,
      compP3D);

  double origQ3D[3] = {0.0, 0.0, 0.0};
  double compQ3D[3][3] = {

    {1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0}

  };
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
    for (
      int iAxis = 3;
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

  return 0;

}
```

### 3D dynamic case

```
// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Include the FMB algorithm library
#include "fmb3dt.h"

// Main function
int main(int argc, char** argv) {

  // Create the two Frames to be tested for intersection
  double origP3DTime[3] = {0.0, 0.0, 0.0};
  double speedP3DTime[3] = {0.0, 0.0, 0.0};
  double compP3DTime[3][3] = {

    {1.0, 0.0, 0.0},  // First component
    {0.0, 1.0, 0.0},  // Second component
    {0.0, 0.0, 1.0}   // Third component

  };
  Frame3DTime P3DTime =
    Frame3DTimeCreateStatic(
      FrameCuboid,
      origP3DTime,
      speedP3DTime,
      compP3DTime);

  double origQ3DTime[3] = {0.0, 0.0, 0.0};
  double speedQ3DTime[3] = {0.0, 0.0, 0.0};
  double compQ3DTime[3][3] =
    {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
  Frame3DTime Q3DTime =
    Frame3DTimeCreateStatic(
      FrameCuboid,
      origQ3DTime,
      speedQ3DTime,
      compQ3DTime);

  // Declare a variable to memorize the result of the intersection
  // detection
  AABB3DTime bdgBox3DTimeLocal;

  // Test for intersection between P and Q
  bool isIntersecting3DTime =
    FMBTestIntersection3DTime(
      &P3DTime,
      &Q3DTime,
      &bdgBox3DTimeLocal);

  // If the two objects are intersecting
  if (isIntersecting3DTime) {

    printf("Intersection detected in AABB ");

    // Export the local bounding box toward the real coordinates
    // system
    AABB3DTime bdgBox3DTime;
    Frame3DTimeExportBdgBox(
      &Q3DTime,
      &bdgBox3DTimeLocal,
      &bdgBox3DTime);

    // Clip with the AABB of 'Q3DTime' and 'P3DTime' to improve results
    for (
      int iAxis = 3;
      iAxis--;) {

      if (bdgBox3DTime.min[iAxis] < P3DTime.bdgBox.min[iAxis]) {

        bdgBox3DTime.min[iAxis] = P3DTime.bdgBox.min[iAxis];

      }

      if (bdgBox3DTime.max[iAxis] > P3DTime.bdgBox.max[iAxis]) {

        bdgBox3DTime.max[iAxis] = P3DTime.bdgBox.max[iAxis];

      }

      if (bdgBox3DTime.min[iAxis] < Q3DTime.bdgBox.min[iAxis]) {

        bdgBox3DTime.min[iAxis] = Q3DTime.bdgBox.min[iAxis];

      }

      if (bdgBox3DTime.max[iAxis] > Q3DTime.bdgBox.max[iAxis]) {

        bdgBox3DTime.max[iAxis] = Q3DTime.bdgBox.max[iAxis];

      }

    }

    AABB3DTimePrint(&bdgBox3DTime);
    printf("\n");

  // Else, the two objects are not intersecting
  } else {

    printf("No intersection.\n");

  }

  return 0;

}
```

## Performance

For the 2D static case:

* FMB is on average 1.3 times slower than SAT to detect intersection and non intersection between Tetrahedrons.
* FMB is on average 1.9 times slower than SAT to detect intersection between a Tetrahedron and a Cuboid, and 1.3 times slower to detect non intersection.
* FMB is on average 2.5 times slower than SAT to detect intersection between Cuboids, and 1.2 times slower to detect non intersection.

FMB is then on average always slower (from 2.5 times to 1.2 times) than SAT whatever the combination of Tetrahedron and Cuboid and the percentage of intersection. 

For the 3D static case:

* FMB is on average 4.2 times faster than SAT to detect intersection between Tetrahedrons, and 1.1 times faster to detect non intersection.
* FMB is on average 2.4 times faster than SAT to detect intersection between a Tetrahedron and a Cuboid, and 1.5 times faster to detect non intersection.
* FMB is on average 1.4 times faster than SAT to detect intersection between Cuboids, and 2.0 times faster to detect non intersection.

FMB is then on average always faster (from 4.2 times to 1.1 times) than SAT whatever the combination of Tetrahedron and Cuboid and the percentage of intersection. 

For the 2D dynamic case:

* FMB is on average 1.7 times slower than SAT to detect intersection between Tetrahedrons, and 1.5 times slower to detect non intersection.
* FMB is on average 2.2 times slower than SAT to detect intersection between a Tetrahedron and a Cuboid, and 1.5 times slower to detect non intersection.
* FMB is on average 3.1 times slower than SAT to detect intersection between Cuboids, and 1.5 times slower to detect non intersection.

FMB is then on average always slower (from 3.1 times to 1.5 times) than SAT whatever the combination of Tetrahedron and Cuboid and the percentage of intersection. 

For the 3D dynamic case:

* FMB is on average 1.8 times faster than SAT to detect intersection between Tetrahedrons, and 1.05 times faster to detect non intersection.
* FMB is on average 1.4 times slower than SAT to detect intersection between a Tetrahedron and a Cuboid, and 1.2 times faster to detect non intersection.
* FMB is on average 2.6 times slower than SAT to detect intersection between Cuboids, and 1.4 times faster to detect non intersection.

FMB is then on average always faster (from 1.8 times to 1.05 times) than SAT for a set of Tetrahedron, and faster than SAT for a combination of Tetrahedrons and Cuboids containining less than around 35\% of intersection, or a combination of Cuboids containining less than around 15\% of intersection. 
